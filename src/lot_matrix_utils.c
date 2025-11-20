//
// Created by fed on 11/20/25.
//


#include "parking_lots_matrixs_utils.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

lot parkingGrid[GRID_HEIGHT][GRID_WIDTH];




void readParkingLotFile(char *filename) {

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


    ParkingType design[GRID_HEIGHT][GRID_WIDTH] = {
        {road, road, road, road, road, road, road},
        {road, obstacle, obstacle, obstacle, obstacle, obstacle, obstacle},
        {road, handicaped, parking_bay, parking_bay, parking_bay, parking_bay, parking_bay},
        {road, road, road, road, road, road, road},
        {road, parking_bay, parking_bay, parking_bay, parking_bay, parking_bay, parking_bay},
        {road, obstacle, obstacle, obstacle, obstacle, obstacle, obstacle},
        {road, road, road, road, road, road, road}};

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            parkingGrid[y][x].x = x;
            parkingGrid[y][x].y = y;
            parkingGrid[y][x].type = design[y][x];

            parkingGrid[y][x].lot_size = (carSize){false, true, false};
        }
    }
}

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
    const int cell = 50; // cell size in pixels

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {

            Rectangle rect = {x * cell, y * cell, cell, cell};
            DrawRectangleRec(rect, getParkingColor(parkingGrid[y][x].type));
            DrawRectangleLines(rect.x, rect.y, rect.width, rect.height, BLACK);
        }
    }
}
