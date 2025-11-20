#ifndef PARKING_H
#define PARKING_H

typedef enum { FALSE, TRUE } bool;

typedef enum { handicaped, road, EV, parking_bay, obstacle } ParkingType;

// structure that defines the sizes of the parking spaces depending on vehicles
typedef struct {
  bool is_small;  // HATCHBACK
  bool is_medium; // SEDAN
  bool is_large;  // SUV, TRUCK
} carSize;

typedef struct {
  int x, y;
  char y_axis; // Name for y axis (H3, I3 ...)
  carSize lot_size;
  // bool is_handicapped;
  ParkingType type;
} lot;

void createParkingLotGrid();

void showParkingGridRayLib();

void readParkingLotFile();

extern int pee;

#define GRID_WIDTH 7
#define GRID_HEIGHT 7

// the idea of the grid:
// r r r r r r r
// r o o o o o o
// r h b b b b b
// r r r r r r r
// r b b b b b b
// r o o o o o o
// r r r r r r r

#define GRID_WIDTH 7
#define GRID_HEIGHT 7

lot parkingGrid[GRID_HEIGHT][GRID_WIDTH];

#endif
