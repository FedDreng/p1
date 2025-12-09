#include "preferences.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG
// Global variables to hold car data retrieved from the API
char *fuel_type = NULL;
char *car_size = NULL;

// Windows-specific code using WinHTTP
#if defined(_WIN32) || defined(USE_WINHTTP)
#include <windows.h>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib") // Link with the WinHTTP library

// Simple manual JSON parser for Windows
char *cutUpJson(const char *json, const char *key) {
  char buffer[256];
  char *pos = strstr(json, key); // Find key
  if (!pos)
    return NULL;

  pos = strchr(pos, ':'); // Move to colon
  if (!pos)
    return NULL;
  pos++;

  // Skip spaces and quotes
  while (*pos && (*pos == ' ' || *pos == '"'))
    pos++;

  // Copy characters until quote, comma, or closing brace
  int i = 0;
  while (*pos && *pos != '"' && *pos != ',' && *pos != '}') {
    buffer[i++] = *pos++;
    if (i >= 255)
      break; // Prevent overflow
  }
  buffer[i] = '\0';

  return _strdup(buffer); // Windows version of strdup
}

int licenseplate(void) {
  // Get the license plate from the current user
  char *plate = currentUser.licensePlate;
  if (!plate) {
    fprintf(stderr, "Error: license plate not set\n");
    return 1; // Exit if no license plate is set
  }

  // Open a WinHTTP session
  HINTERNET hSession =
      WinHttpOpen(L"LicensePlateApp/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                  WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
  if (!hSession)
    return 1;

  // Connect to the API server
  HINTERNET hConnect = WinHttpConnect(hSession, L"v1.motorapi.dk",
                                      INTERNET_DEFAULT_HTTPS_PORT, 0);
  if (!hConnect) {
    WinHttpCloseHandle(hSession);
    return 1;
  }

  // Build the API request path with the license plate
  wchar_t urlPath[256];
  swprintf(urlPath, 256, L"/vehicles?registration_number=%S", plate);

  // Open an HTTP GET request
  HINTERNET hRequest =
      WinHttpOpenRequest(hConnect, L"GET", urlPath, NULL, WINHTTP_NO_REFERER,
                         WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
  if (!hRequest) {
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return 1;
  }

  // Set the authentication token header
#ifdef DEBUG
  const wchar_t *headers = L"X-AUTH-TOKEN: Here-Should-The-Key-Be";
#else
  const wchar_t *headers = L"X-AUTH-TOKEN: qvlm33g45y7sv9sm3qe7rgp1llkg54gq"
#endif

  WinHttpAddRequestHeaders(hRequest, headers, -1L, WINHTTP_ADDREQ_FLAG_ADD);

  // Send the request
  if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                          WINHTTP_NO_REQUEST_DATA, 0, 0, 0)) {
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return 1;
  }

  // Receive the response
  if (!WinHttpReceiveResponse(hRequest, NULL)) {
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return 1;
  }

  // Query the amount of available data
  DWORD bytesAvailable = 0;
  WinHttpQueryDataAvailable(hRequest, &bytesAvailable);

  // Allocate memory for the response
  char *response = malloc(bytesAvailable + 1);
  DWORD bytesRead = 0;
  WinHttpReadData(hRequest, response, bytesAvailable, &bytesRead);
  response[bytesRead] = '\0'; // Null-terminate the response string

  // Extract car info from JSON
#ifdef DEBUG
  fuel_type = "El";
  car_size = "Small";
#else
  fuel_type = cutUpJson(chunk.response, "fuel_type");
  car_size = cutUpJson(chunk.response, "chassis_type");
#endif

  // Print results
  printf("Size: %s\n", car_size);
  printf("Fuel Type: %s\n", fuel_type);
  printf("Plate: %s\n", plate);

  // Clean up
  free(response);
  WinHttpCloseHandle(hRequest);
  WinHttpCloseHandle(hConnect);
  WinHttpCloseHandle(hSession);

  return 0;
}

#elif defined(__linux__) || defined(__APPLE__) || defined(USE_LIBCURL)
#include <curl/curl.h>

// Struct to hold response data dynamically
struct Memory {
  char *response;
  size_t size;
};

// Callback for libcurl to write incoming data
size_t write_callback(void *data, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct Memory *mem = (struct Memory *)userp;

  // Resize memory to hold new data
  char *ptr = realloc(mem->response, mem->size + realsize + 1);
  if (!ptr)
    return 0; // Failure
  mem->response = ptr;

  // Copy new data to the buffer
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = '\0'; // Null-terminate
  return realsize;
}

// Simple JSON parser to extract a value for a given key
char *cutUpJson(const char *json, const char *key) {
  char buffer[256];
  char *pos = strstr(json, key); // Find key
  if (!pos)
    return NULL;
  pos = strchr(pos, ':'); // Move to value
  if (!pos)
    return NULL;
  pos++;
  while (*pos == ' ' || *pos == '"')
    pos++; // Skip spaces/quotes
  int i = 0;
  while (*pos && *pos != '"' && *pos != ',' && *pos != '}')
    buffer[i++] = *pos++;
  buffer[i] = '\0';
  return strdup(buffer); // Return value as string
}

int licenseplate(void) {
  // Get user's license plate
  char *plate = currentUser.licensePlate;
  if (!plate) {
    fprintf(stderr, "Error: license plate not set\n");
    return 1;
  }

  // Build API URL
  char url[256];
  snprintf(url, sizeof(url),
           "https://v1.motorapi.dk/vehicles?registration_number=%s", plate);

  // Initialize CURL
  CURL *curl = curl_easy_init();
  if (!curl)
    return 1;

  struct Memory chunk = {malloc(1), 0}; // Buffer for response
  struct curl_slist *headers = NULL;

#ifdef DEBUG
  headers = curl_slist_append(headers, "X-AUTH-TOKEN: Here-Should-The-Key-Be");
#else
  headers = curl_slist_append(headers,
                              "X-AUTH-TOKEN: qvlm33g45y7sv9sm3qe7rgp1llkg54gq");
#endif

  // Set CURL options
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

  // Perform request
  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    fprintf(stderr, "curl request failed: %s\n", curl_easy_strerror(res));
  } else {
#ifdef DEBUG
    fuel_type = "El";
    car_size = "Small";
#else
    // Extract car info from JSON
    fuel_type = cutUpJson(chunk.response, "fuel_type");
    car_size = cutUpJson(chunk.response, "chassis_type");
#endif
    printf("Size: %s\n", car_size);
    printf("Fuel Type: %s\n", fuel_type);
    printf("Plate: %s\n", plate);
  }

  // Clean up
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
  free(chunk.response);
  return 0;
}

#else
#error "Unsupported platform for licenseplate()"
#endif
