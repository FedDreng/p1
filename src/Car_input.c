#include "Car_Input.h"
#include "navbar.h"
#include "raygui.h"
#include "raylib.h"
// Processes user input through a simple GUI flow.
// guiStep controls which screen/state is active:
//
// Step 0 → Select car size
// Step 1 → Select car type (normal, electric, handicap, both)
//
// The function returns the updated CarInputState after handling button input.

CarInputState GetCarInput(CarInputState state) {

  // STEP 0: Select car size

  if (selectedElement == ChooseSize) {

    // Small car option
    if (GuiButton((Rectangle){50, 150, 200, 50}, "Small")) {
      state.car.size = CAR_SMALL;
      selectedElement = ChooseType; // Proceed to next step
    }
    // Medium car option
    if (GuiButton((Rectangle){50, 220, 200, 50}, "Medium")) {
      state.car.size = CAR_MEDIUM;
      selectedElement = ChooseType; // Proceed to next step
    }
    // Large car option
    if (GuiButton((Rectangle){50, 290, 200, 50}, "Large")) {
      state.car.size = CAR_LARGE;
      selectedElement = ChooseType; // Proceed to next step
    }
  }

  // STEP 1: Select car type
  else if (selectedElement == ChooseType) {

    // Normal (neither electric nor handicap)
    if (GuiButton((Rectangle){50, 150, 200, 50}, "Normal")) {
      state.car.isElectric = false;
      state.car.isHandicap = false;
      selectedElement = ChooseLot;
    }

    // Electric only
    if (GuiButton((Rectangle){50, 220, 200, 50}, "Elektrisk")) {
      state.car.isElectric = true;
      state.car.isHandicap = false;
      selectedElement = ChooseLot;
    }

    // Handicap only
    if (GuiButton((Rectangle){50, 290, 200, 50}, "Handicap")) {
      state.car.isElectric = false;
      state.car.isHandicap = true;
      selectedElement = ChooseLot;
    }

    // Handycap and Electric
    if (GuiButton((Rectangle){50, 360, 200, 50}, "Begge")) {
      state.car.isElectric = true;
      state.car.isHandicap = true;
      selectedElement = ChooseLot;
    }
  }
  // Return the updated state so the main program knows what the user selected
  return state;
}
