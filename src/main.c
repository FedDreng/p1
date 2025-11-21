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

    // Mouse click logic
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {

      int mx = GetMouseX() / CELL_SIZE;
      int my = GetMouseY() / CELL_SIZE;

      if (mx >= 0 && mx < GRID_WIDTH && my >= 0 && my < GRID_HEIGHT) {

        lot *currentLot = &parkingGrid[my][mx];

        carSize myCar = {true, false, false}; // small car

        if (canFit(myCar, currentLot)) {
          currentLot->occupied = TRUE;
        } else {
          printf("Car does NOT fit at (%d, %d)\n", mx, my);
        }
      }
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
