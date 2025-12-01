#include "car_assigner.h"
#include "Car_Input.h"
#include "parking_lots_matrixs_utils.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

CarInputState input = {0};

#define IMPOSSIBLE -1000000

int isIsolated(int x, int y) {
  // Out of bounds neighbors count as isolated (safe edges)
  // Left
  if (x > 0) {
    lot *L = &parkingGrid[y][x - 1];
    if (L->type != road && L->type != obstacle && L->occupied)
      return 0; // not isolated
  }

  // Right
  if (x < GRID_WIDTH - 1) {
    lot *R = &parkingGrid[y][x + 1];
    if (R->type != road && R->type != obstacle && R->occupied)
      return 0; // not isolated
  }

  return 1; // both sides safe
}

int calculateLotScore(const Car *car, const lot *L) {
  if (L->type == road || L->type == obstacle)
    return IMPOSSIBLE;

  if (L->occupied)
    return IMPOSSIBLE;

  if (!canFit(car->size, (lot *)L))
    return IMPOSSIBLE;

  if (!car->is_handicapped && L->type == handicaped)
    return IMPOSSIBLE;

  int score = 0;

  // EV preference
  if (car->is_ev) {
    if (L->type == EV)
      score += 400;
    else
      score += 0;
  }
  if (car->is_handicapped) {
    if (L->type == handicaped)
      score += 500;
    else
      score += 0;
  }

  // size finder
  if (car->size.is_large) { // if it is large
    if (L->lot_size.is_large)
      score += 300;
    else if (L->lot_size.is_medium)
      score += 250;
    else if (L->lot_size.is_small)
      score += 200;
  }
  if (car->size.is_medium) { // if it is medium
    if (L->lot_size.is_medium)
      score += 300;
    else if (L->lot_size.is_large)
      score += 250;
    else if (L->lot_size.is_small)
      score += 200;
  }
  if (car->size.is_small) { // if it is small
    if (L->lot_size.is_small)
      score += 300;
    else if (L->lot_size.is_medium)
      score += 250;
    else if (L->lot_size.is_large)
      score += 200;
  }

  if (car->want_Isolated) {
    if (isIsolated(L->x, L->y))
      score += 100;
  }

  // NOT WORKING YET
  if (!car->placementPref) { // false == dist_to_university
    score -= L->dist_to_university;
  }

  if (car->placementPref) { // true == dist_to_exit
    score -= L->dist_to_exit;
  }

  /*
  if (car->wants_uni_close)
    score -= L->dist_to_university;

  if (car->wants_exit_close)
    score -= L->dist_to_exit;
*/
  return score;
}

lot *chooseBestLot(const Car *car) {
  lot *best = NULL;
  int bestScore = IMPOSSIBLE;

  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {

      lot *L = &parkingGrid[y][x];
      int score = calculateLotScore(car, L);
      // EV preference
      if (car->is_ev) {
        if (L->type == EV)
          score += 1000; // strong preference for EV spot
        else
          score += 0; // fallback if no EV spots available
      }
      if (score > bestScore) {
        bestScore = score;
        best = L;
      }
    }
  }

  return best;
}
void OccipiedSpot(char *username, char *licensePlate, int *posX, int *posY);

void assignCar(Car *car) {
  strcpy(car->owner.username, "Mikkel");
  strcpy(car->owner.licensePlate, "AB26654");
  lot *chosen = chooseBestLot(car);

  if (chosen == NULL) {
    printf("⚠️No suitable parking spot available!\n");
    return;
  }

  chosen->occupied = TRUE;
  printf("license Plate is %s for user: %s.\nThe car is parked af: %d,%d\n\n",
         car->owner.licensePlate, car->owner.username, chosen->x, chosen->y);
  OccipiedSpot(car->owner.username, car->owner.licensePlate, &chosen->x,
               &chosen->y);
  memset(car, 0, sizeof(Car));
}

void OccipiedSpot(char *username, char *licensePlate, int *posX, int *posY) {
  char *path = "../assets/occipied.txt";
  FILE *fptr = fopen(path, "a"); // append mode

  if (fptr == NULL) {
    perror("Error opening file");
    return;
  }
  fprintf(fptr, "%s,%s,%d,%d\n", username, licensePlate, *posX, *posY);

  fclose(fptr);
}

Car createCarFromInput(Car current) {
  input = GetCarInput(input);

  if (input.car.size == 1) { // small
    current.size.is_small = true;
    current.size.is_medium = false;
    current.size.is_large = false;
  } else if (input.car.size == 2) { // medium
    current.size.is_small = false;
    current.size.is_medium = true;
    current.size.is_large = false;
  } else if (input.car.size == 3) { // large
    current.size.is_small = false;
    current.size.is_medium = false;
    current.size.is_large = true;
  }

  current.is_ev = input.car.isElectric;
  current.is_handicapped = input.car.isHandicap;

  // set want isolated
  current.want_Isolated = TRUE;
  /*
  if (IsKeyPressed(KEY_U)) {
    current.wants_uni_close = !current.wants_uni_close;
  }
  if (IsKeyPressed(KEY_X)) { // X for exit
    current.wants_exit_close = !current.wants_exit_close;
  }
  */

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
