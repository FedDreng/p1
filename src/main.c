#define RAYGUI_IMPLEMENTATION
#include "car_assigner.h"
#include "parking_lots_matrixs_utils.h"
#include "raygui.h"
#include "raylib.h"
#include "preferences.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "title");
  SetTargetFPS(60);

  createParkingLotGrid();

  Car Car = {.size = {TRUE, FALSE, FALSE},
             .is_handicapped = FALSE,
             .wants_uni_close = TRUE,
             .wants_exit_close = FALSE};

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    showParkingGridRayLib();

    assignCar(&Car);
    // mouseAssigner();
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
