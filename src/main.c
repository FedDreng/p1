#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"



//iPhone 17 res divided by 4 (way too big otherwise, promise)
#define SCREEN_WIDTH (1206 / 4)
#define SCREEN_HEIGHT (2622 / 4)

#define WINDOW_TITLE "AAU-Park"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);


    InitAudioDevice();

    // Define the camera to look into our 3d world
    Camera camera = { 0 };
    camera.position = (Vector3){ 6.0f, 6.0f, 6.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type

    Color semiTransparent = (Color){255, 255, 255, 128};  // 50% transparent

    Texture2D bilal = LoadTexture(ASSETS_PATH "IMG_1983.jpg"); // bilals smukke ansigt fr
    Sound bilalOST = LoadSound(ASSETS_PATH "SCIFIBEAT.mp3");

    Model lot = LoadModel(ASSETS_PATH "untitledParkingLot.glb");
    Vector3 position = {0.0f,0.0f,0.0f};

    PlaySound(bilalOST);

    while (!WindowShouldClose())
    {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
            DrawModel(lot, position, 1.0f, WHITE);    // Draw animated model
            DrawGrid(10, 1.0f);
        EndMode3D();

        //const int texture_x = SCREEN_WIDTH / 2; // - texture.width / 2;
        //const int texture_y = SCREEN_HEIGHT / 2; // - texture.height / 2;
        const int texture_x = SCREEN_WIDTH / 2 - (bilal.width * 0.2f) / 2;
        const int texture_y = SCREEN_HEIGHT / 2 - (bilal.width * 0.2f) / 2;

        DrawTextureEx(bilal, (Vector2){texture_x, texture_y}, 0 , 0.2f, semiTransparent);

        //const char* text = "OMG! IT WORKS!";
        const char* text = "Bilal er så sexy!";
        const Vector2 text_size = MeasureTextEx(GetFontDefault(), text, 20, 1);
        DrawText(text, SCREEN_WIDTH / 2 - text_size.x / 2, texture_y + bilal.height + text_size.y + 10, 20, BLACK);


        const Vector2 bilal_text_size = MeasureTextEx(GetFontDefault(), text, 20, 1);
        DrawText(text,SCREEN_WIDTH / 2 - (bilal_text_size.x / 2), 50, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
