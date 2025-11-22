#include "car_assigner.h"
#include "parking_lots_matrixs_utils.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

#define IMPOSSIBLE -1000000

int calculateLotScore(const Car *car, const lot *L) {
  if (L->type == road || L->type == obstacle)
    return IMPOSSIBLE;

  if (L->occupied)
    return IMPOSSIBLE;

  if (!canFit(car->size, (lot *)L))
    return IMPOSSIBLE;

  // EV car must go to EV spot
  if (car->is_ev && L->type != EV)
    return IMPOSSIBLE;

  // Handicapped car must go to handicapped spot
  if (car->is_handicapped && L->type != handicaped)
    return IMPOSSIBLE;

  // Non-handicapped car must NOT go to handicapped spot
  if (!car->is_handicapped && L->type == handicaped)
    return IMPOSSIBLE;

  int score = 0;

  if (car->wants_uni_close)
    score -= L->dist_to_university;

  if (car->wants_exit_close)
    score -= L->dist_to_exit;

  return score;
}

lot *chooseBestLot(const Car *car) {
  lot *best = NULL;
  int bestScore = IMPOSSIBLE;

  for (int y = 0; y < GRID_HEIGHT; y++) {
    for (int x = 0; x < GRID_WIDTH; x++) {

      lot *L = &parkingGrid[y][x];
      int score = calculateLotScore(car, L);

      if (score > bestScore) {
        bestScore = score;
        best = L;
      }
    }
  }

  return best;
}

void assignCar(Car *car) {
  lot *chosen = chooseBestLot(car);

  if (chosen == NULL) {
    printf("⚠️No suitable parking spot available!\n");
    return;
  }

  chosen->occupied = TRUE;

  printf("Car parked at (%d, %d)\n", chosen->x, chosen->y);
  memset(car, 0, sizeof(Car));
}

void assigner(int x, int y) {
  if (x != -1 && y != -1) {
    // Check if the mouse click is within the grid bounds
    if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {

      // Get a pointer to the parking lot cell at the clicked location
      lot *currentLot = &parkingGrid[y][x];

      // Define a car of size small
      carSize myCar = {true, false, false}; // small car

      // Check if the car can fit in the selected lot
      if (canFit(myCar, currentLot)) {
        // Mark the lot as occupied if the car fits
        currentLot->occupied = TRUE;
        x = -1;
        y = -1;
      } else {
        // Print a message if the car cannot fit in this lot
        printf("Car does NOT fit at (%d, %d)\n", x, y);
      }
    }
  }
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
