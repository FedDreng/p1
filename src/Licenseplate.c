#include "preferences.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *fuel_type = NULL;
char *car_size = NULL;

#if defined(_WIN32) || defined(USE_WINHTTP)
#include <windows.h>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

int licenseplate(void) {
  char *plate = currentUser.licensePlate;
  if (!plate) {
    fprintf(stderr, "Error: license plate not set\n");
    return 1;
  }

  HINTERNET hSession = WinHttpOpen(L"LicensePlateApp/1.0",
                                   WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                   WINHTTP_NO_PROXY_NAME,
                                   WINHTTP_NO_PROXY_BYPASS, 0);
  if (!hSession) return 1;

  HINTERNET hConnect = WinHttpConnect(hSession,
                                      L"v1.motorapi.dk",
                                      INTERNET_DEFAULT_HTTPS_PORT, 0);
  if (!hConnect) {
    WinHttpCloseHandle(hSession);
    return 1;
  }

  wchar_t urlPath[256];
  swprintf(urlPath, 256, L"/vehicles?registration_number=%S", plate);

  HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET",
                                          urlPath, NULL,
                                          WINHTTP_NO_REFERER,
                                          WINHTTP_DEFAULT_ACCEPT_TYPES,
                                          WINHTTP_FLAG_SECURE);
  if (!hRequest) {
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return 1;
  }

  // Set headers
  const wchar_t *headers = L"X-AUTH-TOKEN: Here-Should-The-Key-Be";
  WinHttpAddRequestHeaders(hRequest, headers, -1L, WINHTTP_ADDREQ_FLAG_ADD);

  if (!WinHttpSendRequest(hRequest,
                          WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                          WINHTTP_NO_REQUEST_DATA, 0,
                          0, 0)) {
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return 1;
  }

  if (!WinHttpReceiveResponse(hRequest, NULL)) {
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return 1;
  }

  DWORD bytesAvailable = 0;
  WinHttpQueryDataAvailable(hRequest, &bytesAvailable);

  char *response = malloc(bytesAvailable + 1);
  DWORD bytesRead = 0;
  WinHttpReadData(hRequest, response, bytesAvailable, &bytesRead);
  response[bytesRead] = '\0';

  // TODO: parse JSON manually like before
  fuel_type = "El";   // placeholder
  car_size = "Hatchback";

  printf("Size: %s\n", car_size);
  printf("Fuel Type: %s\n", fuel_type);
  printf("Plate: %s\n", plate);

  free(response);
  WinHttpCloseHandle(hRequest);
  WinHttpCloseHandle(hConnect);
  WinHttpCloseHandle(hSession);

  return 0;
}

#elif defined(__linux__) || defined(__APPLE__) || defined(USE_LIBCURL)
#include <curl/curl.h>

struct Memory {
    char *response;
    size_t size;
};

size_t write_callback(void *data, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;
    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if (!ptr) return 0;
    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = '\0';
    return realsize;
}

char *cutUpJson(const char *json, const char *key) {
    char buffer[256];
    char *pos = strstr(json, key);
    if (!pos) return NULL;
    pos = strchr(pos, ':');
    if (!pos) return NULL;
    pos++;
    while (*pos == ' ' || *pos == '"') pos++;
    int i = 0;
    while (*pos && *pos != '"' && *pos != ',' && *pos != '}') buffer[i++] = *pos++;
    buffer[i] = '\0';
    return strdup(buffer);
}

int licenseplate(void) {
    char *plate = currentUser.licensePlate;
    if (!plate) {
        fprintf(stderr, "Error: license plate not set\n");
        return 1;
    }

    char url[256];
    snprintf(url, sizeof(url),
             "https://v1.motorapi.dk/vehicles?registration_number=%s", plate);

    CURL *curl = curl_easy_init();
    if (!curl) return 1;

    struct Memory chunk = { malloc(1), 0 };
    struct curl_slist *headers = NULL;
#ifdef DEBUG
    headers = curl_slist_append(headers, "X-AUTH-TOKEN: Here-Should-The-Key-Be");
#else
    headers = curl_slist_append(headers, "X-AUTH-TOKEN: qvlm33g45y7sv9sm3qe7rgp1llkg54gq");
#endif

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl request failed: %s\n", curl_easy_strerror(res));
    } else {
        fuel_type = cutUpJson(chunk.response, "fuel_type");
        car_size = cutUpJson(chunk.response, "chassis_type");
        printf("Size: %s\n", car_size);
        printf("Fuel Type: %s\n", fuel_type);
        printf("Plate: %s\n", plate);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(chunk.response);
    return 0;
}

#else
#error "Unsupported platform for licenseplate()"
#endif
