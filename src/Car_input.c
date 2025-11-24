#include "Car_Input.h"
#include "raylib.h"
#include "raygui.h"

CarProperties GetCarInput(void) {
  CarProperties car = {0};
  int guiStep = 0;

  InitWindow(800, 600, "Car Size Input");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (guiStep == 0) {
      if (GuiButton((Rectangle){50,150,200,50}, "Small")) { car.size = CAR_SMALL; guiStep = 1; }
      if (GuiButton((Rectangle){50,220,200,50}, "Medium")) { car.size = CAR_MEDIUM; guiStep = 1; }
      if (GuiButton((Rectangle){50,290,200,50}, "Large")) { car.size = CAR_LARGE; guiStep = 1; }
    }
    else if (guiStep == 1) {
      if (GuiButton((Rectangle){50,150,200,50}, "Normal")) { car.isElectric=false; car.isHandicap=false; break; }
      if (GuiButton((Rectangle){50,220,200,50}, "Elektrisk")) { car.isElectric=true; car.isHandicap=false; break; }
      if (GuiButton((Rectangle){50,290,200,50}, "Handicap")) { car.isElectric=false; car.isHandicap=true; break; }
      if (GuiButton((Rectangle){50,360,200,50}, "Begge")) { car.isElectric=true; car.isHandicap=true; break; }
    }

    EndDrawing();
  }

  CloseWindow();
  return car;
}
