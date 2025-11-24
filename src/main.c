#define RAYGUI_IMPLEMENTATION
#include "car_assigner.h"
#include "parking_lots_matrixs_utils.h"
#include "raygui.h"
#include "raylib.h"
#include "Car_Input.h"
#include "busyness_prediction.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "title");
    CarProperties car = GetCarInput();

    readParkingLotFile();
    busynessCalculator();

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
    //busynessCalculator();
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




//printf("%d\n%d\n%d\n\n", car.size, car.isElectric, car.isHandicap);