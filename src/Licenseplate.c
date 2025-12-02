#include "navbar.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

char *fuel_type = NULL;
char *car_size = NULL;

// Define layout
const float margin = 20;
const float boxWidth = 500;
const float boxHeight = 50;

char *inputLicenseplate();

// COMMENTS HER CHRISTIAN :)
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

// Extracts the value of a JSON key from a JSON string.
// NOTE: String-based parser assuming the JSON format is stable.

char *cutUpJson(const char *json, const char *key) {
  char buffer[256];
  char *pos = strstr(json, key);
  if (!pos)
    return NULL;

  pos = strchr(pos, ':');
  if (!pos)
    return NULL;
  pos++;

  while (*pos == ' ' || *pos == '"')
    pos++;

  int i = 0;
  while (*pos && *pos != '"' && *pos != ',' && *pos != '}') {
    buffer[i++] = *pos++;
  }
  buffer[i] = '\0';

  return strdup(buffer); // <-- allocate new memory
}

// SKRIV GERNE COMMENTS HER CHRISTIAN :)
int licenseplate(void) {
  /*
  char plate[64];
  if (scanf("%64s", plate) != 1) {
    fprintf(stderr, "error reading plate\n");
    return 1;
  }
  */
  char *plate = "AB26654"; // static licenseplate

  if (plate == NULL) {
    fprintf(stderr, "error reading plate\n");
    return 1;
  }

  char url[256];
  snprintf(url, sizeof(url),
           "https://v1.motorapi.dk/vehicles?registration_number=%s", plate);

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
#endif

#ifndef DEBUG
  headers = curl_slist_append(
      headers, "X-AUTH-TOKEN:  qvlm33g45y7sv9sm3qe7rgp1llkg54gq");
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
    // fuel_type = "El";
    fuel_type = "El";
    car_size = "Hatchback";

#endif
#ifndef DEBUG
    printf("Size: %s\n", cutUpJson(chunk.response, "chassis_type"));
    printf("Fuel Type: %s\n", cutUpJson(chunk.response, "fuel_type"));

    fuel_type = cutUpJson(chunk.response, "fuel_type");
    car_size = cutUpJson(chunk.response, "chassis_type");

    // printf("Response:\n%s\n", chunk.response); // prints the hole json output
#endif
    printf("Size: %s\n", car_size);
    printf("Fuel Type: %s\n", fuel_type);
  }

  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
  free(chunk.response);

  return 0;
}
/*
char *inputLicenseplate() {

  static char textBuffer[16] = {0}; // persistent buffer
  static Rectangle inputBounds = {20, 150, boxWidth, boxHeight};

  // Draw outer box
  DrawRectangleRoundedLines(
      (Rectangle){inputBounds.x - margin / 2, inputBounds.y - margin / 2,
                  inputBounds.width + margin, inputBounds.height + margin},
      0.2f, 8, LIGHTGRAY);

  GuiSetStyle(DEFAULT, TEXT_SIZE, 10 + GuiGetFont().baseSize * 2);

  // Textbox
  GuiTextBox(inputBounds, textBuffer, sizeof(textBuffer), true);

  // Confirm button
  if (GuiButton((Rectangle){50, 250, 200, 50}, "Add")) {
    selectedElement = ChooseSize;
    GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize);
    return textBuffer; // return pointer to the license plate
  }

  return NULL;
}
*/
