#define RAYGUI_IMPLEMENTATION
#include "parking_lots_matrixs_utils.h"
#include "raygui.h"
#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int main(void) {

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "title");
  SetTargetFPS(60);

  createParkingLotGrid();

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    showParkingGridRayLib();

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

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
