#define RAYGUI_IMPLEMENTATION
#include "Car_Input.h"
#include "busyness_prediction.h"
#include "car_assigner.h"
#include "navbar.h"
#include "parking_lots_matrixs_utils.h"
#include "raygui.h"
#include "raylib.h"
#include "preferences.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "title");

  CarInputState input = {0};
  // readParkingLotFile();

  SetTargetFPS(60);
  // busynessCalculator();
  createParkingLotGrid();

  Car Car = {.size = {TRUE, FALSE, FALSE},
             .is_handicapped = FALSE,
             .wants_uni_close = TRUE,
             .wants_exit_close = FALSE};

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    input = GetCarInput(input);


    if (selectedElement == ChooseLot) {
        DrawNav();

      // Print the selected car size to the console
      // 1 = small, 2 = medium, 3 = large
      printf("Size: %d\n", input.car.size);

      // Print whether the car is electric or not
      // 0 = false, 1 = true
      printf("Electric: %d\n", input.car.isElectric);

      // Print whether the car is handicapped-accessible
      // 0 = false, 1 = true
      printf("Handicap: %d\n", input.car.isHandicap);

      showParkingGridRayLib();

      assignCar(&Car);
      // mouseAssigner();
    }

    if (selectedElement == ChooseBusyness) {
      // THE BUSYNESS CALCULATION GUI
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}

// printf("%d\n%d\n%d\n\n", car.size, car.isElectric, car.isHandicap);
