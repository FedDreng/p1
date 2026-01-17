#define RAYGUI_IMPLEMENTATION
#include "Licenseplate.h"
#include "busyness_prediction.h"
#include "car_assigner.h"
#include "navbar.h"
#include "parking_lots_matrixs_utils.h"
#include "preferences.h"
#include "raygui.h"
#include "raylib.h"
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

const char *path = "../assets/parkingLots";

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Car Assigner");

  SetTargetFPS(60);
  // createParkingLotGrid();
  readParkingLotFile(path);
  loadOccipied();
  getPreferences(0);

  printf("\n=== DEBUG ===\n");
  printf("currentUser.username: '%s'\n", currentUser.username);
  printf("currentUser.licensePlate: '%s'\n", currentUser.licensePlate);
  printf("currentUserNR: %d\n", currentUserNR);
  printf("numberOfProfiles: %d\n", numberOfProfiles);
  printf("=== END DEBUG ===\n\n");

  licenseplate();

  /* static car
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
    }

    if (selectedElement == ChooseBusyness) {
      DrawNav();
      busynessCalculator();
    }

    if (selectedElement == UserProfiles) {
      if (lastElement != selectedElement) {
        prefOption = -1;
      }
      // DrawNav();
      changePreferences();
    }

    if (selectedElement == ChooseSpot) {
      DrawNav();
      showParkingGridRayLib();
      mouseAssigner(&userCar);
    }

    EndDrawing();
    lastElement = selectedElement;
  }

  CloseWindow();
  return 0;
}
