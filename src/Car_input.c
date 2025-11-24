#include "Car_Input.h"
#include "raylib.h"
#include "raygui.h"

CarInputState GetCarInput(CarInputState state) {
  // STEP 0: choose size
  if (state.guiStep == 0) {
    if (GuiButton((Rectangle){50,150,200,50}, "Small"))  {
      state.car.size = CAR_SMALL;
      state.guiStep = 1; // ligesomme break !
    }
    if (GuiButton((Rectangle){50,220,200,50}, "Medium")) {
      state.car.size = CAR_MEDIUM;
      state.guiStep = 1;
    }
    if (GuiButton((Rectangle){50,290,200,50}, "Large"))  {
      state.car.size = CAR_LARGE;
      state.guiStep = 1;
    }
  }

    // STEP 1: choose type
  else if (state.guiStep == 1) {
    if (GuiButton((Rectangle){50,150,200,50}, "Normal")) {
      state.car.isElectric = false;
      state.car.isHandicap = false;
      state.guiStep = 2;
    }
    if (GuiButton((Rectangle){50,220,200,50}, "Elektrisk")) {
      state.car.isElectric = true;
      state.car.isHandicap = false;
      state.guiStep = 2;
    }
    if (GuiButton((Rectangle){50,290,200,50}, "Handicap")) {
      state.car.isElectric = false;
      state.car.isHandicap = true;
      state.guiStep = 2;
    }
    if (GuiButton((Rectangle){50,360,200,50}, "Begge")) {
      state.car.isElectric = true;
      state.car.isHandicap = true;
      state.guiStep = 2;
    }
  }

  return state;
}
