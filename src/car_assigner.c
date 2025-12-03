#include "car_assigner.h"
#include "Car_Input.h"
#include "Licenseplate.h"
#include "parking_lots_matrixs_utils.h"
#include "preferences.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

CarInputState input = {0};

// -- Dont place on spot --
#define IMPOSSIBLE -1000000
// -- Around importance --
#define VERY_IMPORTANT 500
#define IMPORTANT 400
// -- sizes --
#define FIRST_SPOT 300
#define SECOND_SPOT 250
#define THIRD_SPOT 200
// -- Nothing --
#define NO_SCORE 0

// make preferences be bigger and more comparable sizes
#define AMPLIFIER 10

int isIsolated(int x, int y) {
  // Check if left side is taken
  if (x > 0) {
    lot *L = &parkingGrid[y][x - 1];
    if (L->type != road && L->type != obstacle && L->occupied)
      return 0; // not isolated
  }

  // Check if right side is taken
  if (x < GRID_WIDTH - 1) {
    lot *R = &parkingGrid[y][x + 1];
    if (R->type != road && R->type != obstacle && R->occupied)
      return 0; // not isolated
  }

  return 1; // both sides safe
}

int calculateLotScore(const Car *car, const lot *L) {
  // -- Sets spots that is not able to be occipied by this car --
  if (L->type == road || L->type == obstacle)
    return IMPOSSIBLE;

  if (L->occupied)
    return IMPOSSIBLE;

  if (!canFit(car->size, (lot *)L))
    return IMPOSSIBLE;

  if (!car->is_handicapped && L->type == handicaped)
    return IMPOSSIBLE;

  // Calculate score
  int score = 0;

  // Give Electric spot to car
  if (car->is_ev) {
    if (L->type == EV)
      score += VERY_IMPORTANT;
    else
      score += NO_SCORE;
  }
  // Give handicapped spot to car
  if (car->is_handicapped) {
    if (L->type == handicaped)
      score += IMPORTANT;
    else
      score += NO_SCORE;
  }

  // -- size finder --
  // if it is large
  if (car->size.is_large) {
    if (L->lot_size.is_large)
      score += FIRST_SPOT;
    else if (L->lot_size.is_medium)
      score += SECOND_SPOT;
    else if (L->lot_size.is_small)
      score += THIRD_SPOT;
  }
  // if it is medium
  if (car->size.is_medium) {
    if (L->lot_size.is_medium)
      score += FIRST_SPOT;
    else if (L->lot_size.is_large)
      score += SECOND_SPOT;
    else if (L->lot_size.is_small)
      score += THIRD_SPOT;
  }
  // if it is small
  if (car->size.is_small) {
    if (L->lot_size.is_small)
      score += FIRST_SPOT;
    else if (L->lot_size.is_medium)
      score += SECOND_SPOT;
    else if (L->lot_size.is_large)
      score += THIRD_SPOT;
  }

  // Give spot to car if its isolated for other cars
  if (car->want_Isolated) {
    if (isIsolated(L->x, L->y))
      score += (currentUser.prefIsolated * AMPLIFIER);
  }

  // Need Place of universety first
  // Maybe just place uni to always be at the left
  if (!car->placementPref) { // false == dist_to_university
    score -= L->dist_to_university;
  }

  if (car->placementPref) { // true == dist_to_exit
    score -= L->dist_to_exit;
  }

  return score;
}

// Using the calculated score to find the best parking spot
lot *chooseBestLot(const Car *car) {
  // Pointer to the currently best parking lot found; NULL means none yet
  lot *best = NULL;

  // Initialize best score to the lowest possible value so any valid score is
  // higher
  int bestScore = IMPOSSIBLE;

  // Loop through every row and column in the parking grid
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {

      // Get a pointer to the current lot at the axises
      lot *L = &parkingGrid[y][x];

      // Calculate how good this spot is for the given car
      int score = calculateLotScore(car, L);

      // If this spot is better than the best one so far, update our choice
      if (score > bestScore) {
        bestScore = score;
        best = L;
      }
    }
  }

  // Return the best matching lot, or NULL if none were suitable
  return best;
}
void OccipiedSpot(char *username, char *licensePlate, int *posX, int *posY);

void assignCar(Car *car) {

  // Reset blinking for all spots first
  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {
      parkingGrid[y][x].isBlinking = FALSE;
    }
  }

  lot *chosen = chooseBestLot(car);

  if (chosen == NULL) {
    // SHOULD DRAW ON THE WINDOW INSTEAD
    hasAssignedSpot = FALSE;
    printf("⚠️No suitable parking spot available!\n");
    return;
  }
  hasAssignedSpot = TRUE;
  // if spot is found the set it as occupied, and current spot to blink
  chosen->occupied = TRUE;
  chosen->isBlinking = TRUE;

  //  Sends to file
  OccipiedSpot(car->Pref.username, car->Pref.licensePlate, &chosen->x,
               &chosen->y);
  memset(car, 0, sizeof(Car));
}

void OccipiedSpot(char *username, char *licensePlate, int *posX, int *posY) {
  // find occipied.txt
  char *path = "../assets/occipied.txt";
  FILE *fptr = fopen(path, "a"); // append mode

  // validate
  if (fptr == NULL) {
    perror("Error opening file");
    return;
  }
  // Insert input into txt file
  fprintf(fptr, "%s,%s,%d,%d\n", username, licensePlate, *posX, *posY);

  fclose(fptr);
}

Car createCarFromInput(Car current) {
  // get input from Car_input
  input = GetCarInput(input);

  // check input from user or from licensePlate
  if (input.car.size == 1 || strcasecmp(car_size, "Hatchback") == 0) { // small
    // Set as {TRUE,FALSE,FALSE}
    current.size.is_small = true;
    current.size.is_medium = false;
    current.size.is_large = false;
  } else if (input.car.size == 2 || strcasecmp(car_size, "Sudan") == 0) { // med
    // Set as {FALSE,TRUE,FALSE}
    current.size.is_small = false;
    current.size.is_medium = true;
    current.size.is_large = false;
  } else if (input.car.size == 3 || strcasecmp(car_size, "SUV") == 0) { // large
    // Set as {FALSE,FALSE,TRUE}
    current.size.is_small = false;
    current.size.is_medium = false;
    current.size.is_large = true;
  }
  // If cartype is not defined by licensePlate take user input
  if (strcasecmp(fuel_type, "El") != 0) {
    current.is_ev = input.car.isElectric;
  }
  // Get handicapped from user
  current.is_handicapped = input.car.isHandicap;

  // Set preferences
  strncpy(current.Pref.licensePlate, currentUser.licensePlate,
          sizeof(current.Pref.licensePlate));

  strncpy(current.Pref.username, currentUser.username,
          sizeof(current.Pref.username));

  // set want isolated
  if (0 < currentUser.prefIsolated) {
    current.want_Isolated = TRUE;
  }

  return current;
}

void mouseAssigner() {
  // Check if the left mouse button was just pressed
  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

    // Get the mouse coordinates in terms of grid cells
    int mx = GetMouseX() / CELL_SIZE; // Convert pixel X to cell X
    int my = GetMouseY() / CELL_SIZE; // Convert pixel Y to cell Y

    // Check if the mouse click is within the grid bounds
    if (mx >= 0 && mx < GRID_WIDTH && my >= 0 && my < GRID_HEIGHT) {

      // Get a pointer to the parking lot cell at the clicked location
      lot *currentLot = &parkingGrid[my][mx];

      // Define a car of size small
      carSize myCar = {true, false, false}; // small car

      // Check if the car can fit in the selected lot
      if (canFit(myCar, currentLot)) {
        // Mark the lot as occupied if the car fits
        currentLot->occupied = TRUE;
      } else {
        // Print a message if the car cannot fit in this lot
        printf("Car does NOT fit at (%d, %d)\n", mx, my);
      }
    }
  }
}
