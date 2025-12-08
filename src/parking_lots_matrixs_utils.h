#ifndef PARKING_H
#define PARKING_H
typedef enum { FALSE, TRUE } typeBoolean;

typedef enum { handicaped, road, EV, parking_bay, obstacle } ParkingType;

// structure that defines the sizes of the parking spaces depending on vehicles
typedef struct {
  typeBoolean is_small;  // HATCHBACK
  typeBoolean is_medium; // SEDAN
  typeBoolean is_large;  // SUV, TRUCK
} carSize;

typedef struct {
  int x, y;
  char y_axis; // Name for y axis (H3, I3 ...)
  carSize lot_size;
  ParkingType type;
  typeBoolean isBlinking;
  typeBoolean occupied;
  carSize allowed;

  int dist_to_university;
  int dist_to_exit;

  char username[20];
  char licensePlate[8];

} lot;

typedef struct {
  char username[50];
  char license[20];
  int posX;
  int posY;
} Occipied;

typedef struct Color Color;

void createParkingLotGrid();

void showParkingGridRayLib();

void readParkingLotFile();

Color getParkingColor(ParkingType t);

typeBoolean canFit(carSize car, lot *currentLot);

extern int pee;
extern typeBoolean hasAssignedSpot;

#define CELL_SIZE 50

#define GRID_WIDTH 10
#define GRID_HEIGHT 10

// the idea of the grid:
// r r r r r r r
// r o o o o o o
// r h b b b b b
// r r r r r r r
// r b b b b b b
// r o o o o o o
// r r r r r r r

extern lot parkingGrid[GRID_HEIGHT][GRID_WIDTH];
extern int isAssigned;

#endif
