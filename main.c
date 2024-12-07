#include "raylib.h"
#include "./headers/game.h"
#include "./headers/assets.h"
#include "./headers/winapi.h"
#include <pthread.h>

int main(void)
{
    // Initialize Raylib
    InitWindow(255, 255, "Virtual Pet");

    // Call WinAPI functions safely
    InitWindowSettings();

    // Load assets
    LoadAssets();

    // Initialize game
    InitGame();
    
    // SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Update game logic
        UpdateGame();

        // Draw game
        BeginDrawing();
        ClearBackground(BLANK);
        DrawGame();
        EndDrawing();
    }

    // Unload assets
    UnloadAssets();

    // De-Initialization
    CloseWindow();
    return 0;
}
