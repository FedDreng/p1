#ifndef PARKING_H
#define PARKING_H

typedef enum { FALSE, TRUE } boolean;

typedef enum { handicaped, road, EV, parking_bay, obstacle } ParkingType;

// structure that defines the sizes of the parking spaces depending on vehicles
typedef struct {
  boolean is_small;  // HATCHBACK
  boolean is_medium; // SEDAN
  boolean is_large;  // SUV, TRUCK
} carSize;

typedef struct {
  int x, y;
  char y_axis; // Name for y axis (H3, I3 ...)
  carSize lot_size;
  // bool is_handicapped;
  ParkingType type;

  boolean occupied;
  carSize allowed;

  // NEW FIELDS FOR PREFERENCES
  int dist_to_university;
  int dist_to_exit;
} lot;

typedef struct Color Color;

void createParkingLotGrid();

void showParkingGridRayLib();

void readParkingLotFile();

Color getParkingColor(ParkingType t);

boolean canFit(carSize car, lot *currentLot);

extern int pee;

#define CELL_SIZE 50

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

extern lot parkingGrid[GRID_HEIGHT][GRID_WIDTH];

#endif
