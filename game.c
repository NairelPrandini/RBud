#include "./headers/game.h"
#include "./headers/winapi.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

// Sprites from Assets
extern Texture2D petSprite;

static Vector2 petPosition;
static Vector2 petVelocity;
static bool dragging = false;
static Vector2 dragOffset;
static float petGravity = 1024.0f;

static Vector2 lastMousePosition;

static double lastUpdateTime = 0.0;
static const double updateInterval = 0.01; // 10 ms

void InitGame()
{
    petPosition = (Vector2){200, 200};
    petVelocity = (Vector2){0, 0};
    dragging = false;
}

static DesktopWindow GroundWindow;
static RECT WorkArea;
static RECT ScreenArea;

void UpdateGame()
{

    DragPet();

    // Check for ground collision
    WorkArea = GetWorkArea();
    ScreenArea = (RECT){0, 0, GetScreenWidth(), GetScreenHeight()};


    GroundWindow = (DesktopWindow){.rect = WorkArea, .layer = -1, .title = "WorkArea"};

    DesktopWindow windows[1024];
    int count;
    GetWindowsByLayer(windows, &count);

    for (int i = 0; i < count; i++)
    {
        RECT windowRect = windows[i].rect;
        if (petPosition.x >= windowRect.left && petPosition.x <= windowRect.right && petPosition.y <= windowRect.top && petPosition.y <= windowRect.bottom)
        {
            GroundWindow = windows[i];
        }
    }

    printf("Ground Window: %s\n", GroundWindow.title);

    
}

void DragPet()
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mousePosition = GetMousePosition();
        Rectangle petRect = {petPosition.x - petSprite.width / 2, petPosition.y - petSprite.height, petSprite.width, petSprite.height};
        if (CheckCollisionPointRec(mousePosition, petRect))
        {
            dragging = true;
            dragOffset = Vector2Subtract(mousePosition, petPosition);
            lastMousePosition = mousePosition;
        }
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        dragging = false;
        Vector2 currentMousePosition = GetMousePosition();
        petVelocity = Vector2Subtract(currentMousePosition, lastMousePosition);
        petVelocity = Vector2Scale(petVelocity, 8.0f);
    }

    if (dragging)
    {
        Vector2 mousePosition = GetMousePosition();
        petPosition = Vector2Subtract(mousePosition, dragOffset);
        petVelocity = (Vector2){0, 0};
        lastMousePosition = mousePosition;
    }
    else
    {
        petVelocity.y += petGravity * GetFrameTime();
        petPosition = Vector2Add(petPosition, Vector2Scale(petVelocity, GetFrameTime()));
    }


    if (!dragging)
    {
        if (petPosition.x < WorkArea.left) petPosition.x = WorkArea.left;
        if (petPosition.x > WorkArea.right) petPosition.x = WorkArea.right;
        if (petPosition.y > WorkArea.bottom) petPosition.y = WorkArea.bottom;
    }
}

void DrawGame()
{
    //DrawFPS(10, 10); // Draw FPS in the top-left corner

    Rectangle sourceRec = {0.0f, 0.0f, (float)petSprite.width, (float)petSprite.height};
    Rectangle destRec = {petPosition.x, petPosition.y, (float)petSprite.width, (float)petSprite.height};
    Vector2 origin = {(float)petSprite.width / 2, (float)petSprite.height};

    // Draw the texture with the specified parameters
    DrawTexturePro(petSprite, sourceRec, destRec, origin, 0.0f, WHITE);
}