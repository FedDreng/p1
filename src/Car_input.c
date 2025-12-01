#include "Car_Input.h"
#include "Licenseplate.h"
#include "navbar.h"
#include "raygui.h"
#include "raylib.h"
#include <string.h>

typedef enum { TRUE, FALSE } boolean;

boolean is_valid_size(const char *s) {
  if (!s)
    return false;
  return strcasecmp(s, "Hatchback") == 0 || strcasecmp(s, "Sedan") == 0 ||
         strcasecmp(s, "SUV") == 0;
}

// Processes user input through a simple GUI flow.
// guiStep controls which screen/state is active:
//
// Step 0 → Select car size
// Step 1 → Select car type (normal, electric, handicap, both)
//
// The function returns the updated CarInputState after handling button input.

CarInputState GetCarInput(CarInputState state) {
  // --- Validate car size (Step 0) ---

  // STEP 0: Select car size

  if (selectedElement == ChooseSize) {
    if (is_valid_size(car_size)) {
      selectedElement = ChooseType;
    }
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
      selectedElement = Assign;
    }

    if (fuel_type && strcasecmp(fuel_type, "El") != 0) {
      //   Electric only
      if (GuiButton((Rectangle){50, 220, 200, 50}, "Elektrisk")) {
        state.car.isElectric = true;
        state.car.isHandicap = false;
        selectedElement = Assign;
      }
    }

    // Handicap only
    if (GuiButton((Rectangle){50, 290, 200, 50}, "Handicap")) {
      state.car.isElectric = false;
      state.car.isHandicap = true;
      selectedElement = Assign;
    }

    // Handycap and Electric
    if (GuiButton((Rectangle){50, 360, 200, 50}, "Begge")) {
      state.car.isElectric = true;
      state.car.isHandicap = true;
      selectedElement = Assign;
    }
  }
  // Return the updated state so the main program knows what the user selected
  return state;
}
