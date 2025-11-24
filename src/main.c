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

    CarInputState input = {0};
    //readParkingLotFile();

    SetTargetFPS(60);
    //busynessCalculator();
    createParkingLotGrid();

    Car Car = {.size = {TRUE, FALSE, FALSE},
    .is_handicapped = FALSE,
    .wants_uni_close = TRUE,
    .wants_exit_close = FALSE
    };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        input = GetCarInput(input);

        if (input.guiStep == 2) {
            printf("Size: %d\n", input.car.size); // 1: small 2: medium 3: large
            printf("Electric: %d\n", input.car.isElectric); // 0: false 1: true
            printf("Handicap: %d\n", input.car.isHandicap); // 0: false 1: true
           
            showParkingGridRayLib();

            assignCar(&Car);
            // mouseAssigner();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}




//printf("%d\n%d\n%d\n\n", car.size, car.isElectric, car.isHandicap);