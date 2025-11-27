#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct Memory {
    char *response;
    size_t size;
};

size_t write_callback(void *data, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;

    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if (ptr == NULL) {

        return 0;
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = '\0';

    return realsize;
}

int main(void) {
    char plate[64];
    printf("Enter the license plate: ");
    if (scanf("%63s", plate) != 1) {
        fprintf(stderr, "Error reading plate\n");
        return 1;
    }


    char url[256];
    snprintf(url, sizeof(url), "https://v1.motorapi.dk/vehicles?registration_number=%s", plate);

    CURL *curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Could not init curl\n");
        return 1;
    }

    struct Memory chunk = {0};
    chunk.response = malloc(1);
    chunk.size = 0;

    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "X-AUTH-TOKEN: e2dgkm06h8lu71nfan1dcwituloghgug");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl request failed: %s\n", curl_easy_strerror(res));
    } else {
        printf("Response:\n%s\n", chunk.response);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(chunk.response);

    return 0;
}