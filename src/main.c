#include "raylib.h"
#include "game.h"
#include "assets.h"
#include "winapi.h"
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
    
    SetTargetFPS(200);

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
