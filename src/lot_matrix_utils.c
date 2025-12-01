//
// Created by fed on 11/20/25.
//
#include "Licenseplate.h"
#include "parking_lots_matrixs_utils.h"
#include "raylib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

lot parkingGrid[GRID_HEIGHT][GRID_WIDTH];
int isAssigned = 0;

void readParkingLotFile(char *filename) {

  // int countedFiles = countFiles("../assets/parkingLots");

  int countedFiles = 0;

  const char *path = "../assets/parkingLots";
  struct dirent *entry;

  char *files[10]; // current cap of files is 10, increase if needed.

  DIR *dir = opendir(path);
  if (dir == NULL) {
    perror("opendir");
  }

  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue; // skip these
    }

    printf("%s\n", entry->d_name);
    files[countedFiles] = strdup(entry->d_name);
    countedFiles++;
  }
  closedir(dir);

  printf("Chose what parking lot you want to use: \n");

  for (int i = 0; i < countedFiles; i++) {
    printf("%d, %s \n", i + 1, files[i]);
  }

  scanf("%d", &countedFiles);
  countedFiles -= 1;

  // printf("countfiles: %d\n", countedFiles);

  char chosenFileName[256];

  // char chosenFileName[] = "../assets/parkingLots/volBollingTheHomie.txt";
  printf("fywaaah, %d", countedFiles);
  sprintf(chosenFileName, "../assets/parkingLots/%s", files[countedFiles]);

  FILE *fp = fopen(chosenFileName, "r");

  if (fp == NULL) {
    printf("retard");
  }

  printf("\n");
  char buffer[256];

  // Read and print each line
  while (fgets(buffer, sizeof(buffer), fp) != NULL) {
    printf("%s", buffer);

    // if (buffer[0] == 'c') {
    //   printf("among us");
    // }
  }

  printf("\n\n");

  // første gang laver vi funktionen for at se størrelsen af parking lot

  int ch;

  int h = 0, k = 0;

  rewind(fp);

  while ((ch = fgetc(fp)) != EOF) {

    if (ch == '\n') {
      k++;
      h = 0;
    } else if (ch == ' ') {
      h--;
    }

    h++;
  }
  h--;

  printf("x: %d, y: %d ", h, k);

  // derefter assigner vi

  ParkingType designLot[h][k];

  // int i = 0;
  printf("\n\n");

  /*for (int i = 0; i <= k; i++) {
    //vertical loop
    for (int y = 0; y < h; y++) {
      printf("(x row)");
      if () {

      }
    }

    printf("\n");
  }*/

  rewind(fp);
  h = 0, k = 0;

  printf("\n\n");

  while ((ch = fgetc(fp)) != EOF) {

    // printf("inside loop yo");

    if (ch == ' ') {
      continue;
    }

    if (ch == 'r') {
      printf(" r ");
      designLot[h][k] = road;
      printf("ROAD ROAD ROAD ROAD");
      h++;
    } else if (ch == '\n') {
      printf("\n");
      k++;
      h = 0;
    } else if (ch == 'o') {
      printf(" o ");
      designLot[h][k] = obstacle;
      h++;
    }

    if (ch == '\n') {
      printf("\n");
    }

    printf(" (h: %d, k: %d) ", h, k);
  }

  // husk, starter fra 0... shiii - VIRKER STADIG IK, BLIR GAL

  printf("\ndesign lot given: %d\n", designLot[1][1]);
  printf("design lot given: %d\n", designLot[1][3]);
}

void createParkingLotGrid() {

  /*printf("How many files do you have?");
  int filesP;
  scanf("%d", &filesP);

  //string

  for (int i = 0; i < filesP; i++) {
      printf("Write name of file 1 (must not contain spaces)");
      //scanf("" *s);  todo: string which inputs to readParkingLotFile func;
      //readParkingLotFile(string);
  }*/

  // setting the layout for the grid we are working with right now it is this
  // (parkingGrid)
  ParkingType design[GRID_HEIGHT][GRID_WIDTH] = {
      {road, road, road, road, road, road, road},
      {road, obstacle, obstacle, obstacle, obstacle, obstacle, obstacle},
      {road, handicaped, parking_bay, parking_bay, parking_bay, parking_bay,
       parking_bay},
      {road, road, road, road, road, road, road},
      {road, parking_bay, parking_bay, parking_bay, parking_bay, parking_bay,
       parking_bay},
      {road, obstacle, obstacle, obstacle, obstacle, obstacle, obstacle},
      {road, road, road, road, road, road, road}};

  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      parkingGrid[y][x].x = x;
      parkingGrid[y][x].y = y;
      parkingGrid[y][x].type = design[y][x];

      // setting out the parking lot size depending on type identifier
      // (enum type)
      if (design[y][x] == parking_bay) {
        parkingGrid[y][x].allowed =
            (carSize){true, true, true}; // small/med/large
      } else if (design[y][x] == handicaped) {
        parkingGrid[y][x].allowed = (carSize){false, true, true}; // med/large
      } else if (design[y][x] == EV) {
        parkingGrid[y][x].allowed = (carSize){true, true, false}; // small/med
      } else {
        parkingGrid[y][x].allowed =
            (carSize){false, false,
                      false}; // no car is able to park here (obstacle, road)
      }
      // parkingGrid[y][x].lot_size = (carSize){false, true, false};

      // setting the cell to being available
      parkingGrid[y][x].occupied = FALSE;
    }
  }
}

// Return a color depending on the identifier, this is used in the raylib
// parkingGrid printer "showParkingGridRayLib"
Color getParkingColor(ParkingType t) {
  switch (t) {
  case road:
    return GRAY;
  case obstacle:
    return RED;
  case handicaped:
    return BLUE;
  case parking_bay:
    return GREEN;
  case EV:
    return YELLOW;
  default:
    return WHITE;
  }
}

void showParkingGridRayLib() {
  int gridWidth = GRID_WIDTH * CELL_SIZE;
  int gridHeight = GRID_HEIGHT * CELL_SIZE;

  int offsetX = (GetScreenWidth() - gridWidth) / 2;
  int offsetY = ((GetScreenHeight() - gridHeight) / 2) +
                60 / 2; // offset Y + the height of the navbar 60 / 2

  // Draw parking grid manually (so we can add occupied colors)
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {

      // set the current lot to be the same as the grid we work with right now
      lot *currentLot = &parkingGrid[y][x];

      // creating the Rectangle for the spot / Cell
      Rectangle rect = {offsetX + x * CELL_SIZE, offsetY + y * CELL_SIZE,
                        CELL_SIZE, CELL_SIZE};

      // check if the parking lot is occupied
      if (currentLot->occupied)
        DrawRectangleRec(rect, DARKGRAY);
      else
        // print the color for the specific identifier
        DrawRectangleRec(rect, getParkingColor(currentLot->type));
      // set the border to black
      DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, BLACK);
    }
  }
}

/*
carSize setCarSize() {
  carSize type;

  if (strcasecmp(car_size, "Hatchback")) {
    printf("small");
  }
  if (strcasecmp(car_size, "Sudan")) {
    printf("medium");
  }
  if (strcasecmp(car_size, "SUV")) {
    printf("Large");
  }

  return type;
}
*/
// Function to check if a car can fit in a given parking lot
boolean canFit(carSize car, lot *currentLot) {

  // If the lot is already occupied, the car cannot fit
  if (currentLot->occupied)
    return FALSE;

  // Check if the lot type is one of the allowed types
  // Only parking_bay, handicaped, or EV types are valid for parking
  if (currentLot->type != parking_bay && currentLot->type != handicaped &&
      currentLot->type != EV)
    return FALSE;

  // Small cars can fit anywhere regardless of lot size
  if (car.is_small)
    return TRUE;

  // Medium cars can fit in medium or large lots
  if (car.is_medium &&
      (currentLot->allowed.is_medium || currentLot->allowed.is_large))
    return TRUE;

  // Large cars can only fit in large lots
  if (car.is_large && currentLot->allowed.is_large)
    return TRUE;

  // If none of the above conditions are met, the car cannot fit
  return FALSE;
}
