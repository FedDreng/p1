#define RAYGUI_IMPLEMENTATION
#include "parking_lots_matrixs_utils.h"
#include "raygui.h"
#include "raylib.h"

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGHT (450)

#define WINDOW_TITLE "Title"

int main(void) {

  readParkingLotFile();

  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
  SetTargetFPS(60);

  createParkingLotGrid();

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);
    showParkingGridRayLib();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
