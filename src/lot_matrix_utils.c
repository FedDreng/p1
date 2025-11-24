//
// Created by fed on 11/20/25.
//
#include "parking_lots_matrixs_utils.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

lot parkingGrid[GRID_HEIGHT][GRID_WIDTH];

/*
#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif


int countFiles(const char *folderPath) {
    int count = 0;

#ifdef _WIN32
    WIN32_FIND_DATA data;
    char searchPath[260];
    sprintf(searchPath, "%s\\*.*", folderPath);

    HANDLE hFind = FindFirstFile(searchPath, &data);
    if (hFind == INVALID_HANDLE_VALUE) return -1;

    do {
        if (!(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            count++;
    } while (FindNextFile(hFind, &data));

    FindClose(hFind);

#else
    DIR *dir = opendir(folderPath);
    if (dir == NULL) return -1;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue; // skip . and ..
        count++;
    }
    closedir(dir);
#endif

    return count;
}


/*
void readParkingLotFile(char *filename) {

    printf("countfiles: %d\n", countFiles("../assets/parkingLots"));

    char buffer[256];
    FILE *fp = fopen("../assets/parkingLots/parkingGrid.txt", "r");

    if (fp == NULL) {
        printf("retard");
    }

    // Read and print each line
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    printf("\n");

}
*/
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
  // Draw parking grid manually (so we can add occupied colors)
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {

      // set the current lot to be the same as the grid we work with right now
      lot *currentLot = &parkingGrid[y][x];

      // creating the Rectangle for the spot / Cell
      Rectangle rect = {x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE};

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

//
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