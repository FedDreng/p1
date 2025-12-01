#define RAYGUI_IMPLEMENTATION
#include "Car_Input.h"
#include "Licenseplate.h"
#include "busyness_prediction.h"
#include "car_assigner.h"
#include "filecheck.h"
#include "navbar.h"
#include "parking_lots_matrixs_utils.h"
#include "preferences.h"
#include "raygui.h"
#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "title");

  CarInputState input = {0};
  // readParkingLotFile();

  SetTargetFPS(60);
  // busynessCalculator();
  createParkingLotGrid();

  readParkingLotFile();

  // getPreferences(0);
  licenseplate();

  // static car
  /*
  Car userCar = {.size = {TRUE, FALSE, FALSE},
              .is_handicapped = FALSE,
              .wants_uni_close = TRUE,
              .wants_exit_close = FALSE};
  */
  Car userCar = {0};

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    userCar = createCarFromInput(userCar);

    if (selectedElement == Assign) {
      assignCar(&userCar);
      selectedElement = ChooseLot;
    }

    if (selectedElement == ChooseLot) {
      DrawNav();

      showParkingGridRayLib();

      //  mouseAssigner();
    }

    if (selectedElement == ChooseBusyness) {
      DrawNav();
      busynessCalculator();
    }

    if (selectedElement == ChooseSpot) {
      // GUI
    }

    if (selectedElement == UserProfiles) {
      DrawNav();
      changePreferences();
      changePrefGui();
    }

    EndDrawing();
  }

  CloseWindow();
  return 0;
}

// printf("%d\n%d\n%d\n\n", car.size, car.isElectric, car.isHandicap);
