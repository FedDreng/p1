#include "preferences.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *fuel_type = NULL;
char *car_size = NULL;

// Define layout
const float margin = 20;
const float boxWidth = 500;
const float boxHeight = 50;

char *inputLicenseplate();

// Platform-specific includes
#ifdef USE_CURL_CMD
#include <curl/curl.h>
#endif

#ifdef USE_WINHTTP
#include <windows.h>
#include <winhttp.h>
#endif

// JSON parsing (same for all platforms)
char *cutUpJson(const char *json, const char *key) {
  char buffer[256];
  char *pos = strstr(json, key);
  if (!pos) return NULL;

  pos = strchr(pos, ':');
  if (!pos) return NULL;
  pos++;

  while (*pos == ' ' || *pos == '"') pos++;

  int i = 0;
  while (*pos && *pos != '"' && *pos != ',' && *pos != '}') {
    buffer[i++] = *pos++;
  }
  buffer[i] = '\0';

  return strdup(buffer);
}

#ifdef USE_CURL_CMD
// libcurl memory structure
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
#endif

int licenseplate(void) {
  char *plate = currentUser.licensePlate;
  if (!plate) {
    fprintf(stderr, "error reading plate\n");
    return 1;
  }

  char url[512];
  snprintf(url, sizeof(url), "https://v1.motorapi.dk/vehicles?registration_number=%s", plate);

#ifdef USE_CURL_CMD
  CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Could not init curl\n");
        return 1;
    }

    struct Memory chunk = {0};
    chunk.response = malloc(1);
    chunk.size = 0;

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
#ifdef DEBUG
        fuel_type = "El";
        car_size = "Hatchback";
#else
        fuel_type = cutUpJson(chunk.response, "fuel_type");
        car_size = cutUpJson(chunk.response, "chassis_type");
#endif
        printf("Size: %s\n", car_size);
        printf("Fuel Type: %s\n", fuel_type);
        printf("plate: %s\n", plate);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(chunk.response);
#endif

#ifdef USE_WINHTTP
  HINTERNET hSession = WinHttpOpen(L"LicensePlateApp/1.0",
                                     WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                     WINHTTP_NO_PROXY_NAME,
                                     WINHTTP_NO_PROXY_BYPASS, 0);
    if (!hSession) {
        fprintf(stderr, "WinHttpOpen failed\n");
        return 1;
    }

    URL_COMPONENTS urlComp;
    memset(&urlComp, 0, sizeof(urlComp));
    urlComp.dwStructSize = sizeof(urlComp);

    wchar_t wHost[256], wPath[256];
    mbstowcs(wHost, "v1.motorapi.dk", sizeof(wHost)/sizeof(wHost[0]));
    mbstowcs(wPath, plate, sizeof(wPath)/sizeof(wPath[0]));

    HINTERNET hConnect = WinHttpConnect(hSession, wHost, INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        fprintf(stderr, "WinHttpConnect failed\n");
        WinHttpCloseHandle(hSession);
        return 1;
    }

    wchar_t wRequest[512];
    swprintf(wRequest, 512, L"/vehicles?registration_number=%hs", plate);

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", wRequest,
                                            NULL, WINHTTP_NO_REFERER,
                                            WINHTTP_DEFAULT_ACCEPT_TYPES,
                                            WINHTTP_FLAG_SECURE);
    if (!hRequest) {
        fprintf(stderr, "WinHttpOpenRequest failed\n");
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return 1;
    }

#ifdef DEBUG
    WinHttpAddRequestHeaders(hRequest, L"X-AUTH-TOKEN: Here-Should-The-Key-Be", -1L, WINHTTP_ADDREQ_FLAG_ADD);
#else
    WinHttpAddRequestHeaders(hRequest, L"X-AUTH-TOKEN: qvlm33g45y7sv9sm3qe7rgp1llkg54gq", -1L, WINHTTP_ADDREQ_FLAG_ADD);
#endif

    if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                            WINHTTP_NO_REQUEST_DATA, 0, 0, 0) ||
        !WinHttpReceiveResponse(hRequest, NULL)) {
        fprintf(stderr, "WinHttp request failed\n");
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        return 1;
    }

    DWORD dwSize = 0;
    WinHttpQueryDataAvailable(hRequest, &dwSize);

    char *response = malloc(dwSize + 1);
    DWORD dwRead = 0;
    WinHttpReadData(hRequest, response, dwSize, &dwRead);
    response[dwRead] = '\0';

#ifdef DEBUG
    fuel_type = "El";
    car_size = "Hatchback";
#else
    fuel_type = cutUpJson(response, "fuel_type");
    car_size = cutUpJson(response, "chassis_type");
#endif

    printf("Size: %s\n", car_size);
    printf("Fuel Type: %s\n", fuel_type);
    printf("plate: %s\n", plate);

    free(response);
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
#endif

  return 0;
}
