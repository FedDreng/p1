#include "navbar.h"
#include "raygui.h"
#include "raylib.h"

Rectangle navbar = {0, 0, 800, 60};
Color navColor = (Color){240, 240, 240, 255};

menuElement selectedElement = ChooseType;

const char *elementNames[ELEMENT_COUNT] = {"Choose Lot",     "Choose Size",
                                           "Choose Type",    "BusyNess",
                                           "Reserve a spot", "User Profiles"};

menuElement navElementAssigner(int index) {
  if (index < 0 || index >= ELEMENT_COUNT)
    return ChooseLot; // default fallback

  return (menuElement)index;
}

void DrawNav() {
  // Draw navbar background
  DrawRectangleRec(navbar, navColor);
  DrawRectangleLinesEx(navbar, 1, LIGHTGRAY);

  int elementWidth = 100;
  int elementGap = 20;

  int screenWidth = GetScreenWidth();
  int posX =
      (screenWidth / 2) - ((elementGap + elementWidth) * ELEMENT_COUNT) / 2;
  for (int i = 0; i < ELEMENT_COUNT; i++) {
    if (GuiButton((Rectangle){posX, 15, elementWidth, 30}, elementNames[i])) {
      selectedElement = navElementAssigner(i); // <--- GLOBAL WRITE
    }
    posX += elementWidth + elementGap;
  }
}
