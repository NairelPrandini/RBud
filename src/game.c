#include "game.h"
#include "winapi.h"
#include "raylib.h"
#include "raymath.h"
#include "assets.h"
#include <stdio.h>

static Vector2 petPosition;
static Vector2 petVelocity;

static bool dragging = false;
static Vector2 dragOffset;
static float petGravity = 2048.0f;

static Vector2 lastMousePosition;

// Sprites from Assets
extern Texture2D petSprite;

void InitGame()
{
    petPosition = (Vector2){200, 200};
    petVelocity = (Vector2){0, 0};
    dragging = false;

    // Load assets
    LoadAssets();
}

static DesktopWindow GroundWindow;
static DesktopWindow TopWindow;
static RECT WorkArea;
static RECT ScreenArea;

static DesktopWindow StandingWindow;

void UpdateGame()
{
    // Check for ground collision
    WorkArea = GetWorkArea();
    ScreenArea = (RECT){0, 0, GetScreenWidth(), GetScreenHeight()};

    DesktopWindow windows[1024];
    int count;
    GetWindowsByLayer(windows, &count);

    GroundWindow = (DesktopWindow){.rect = {0, ScreenArea.bottom - abs(ScreenArea.bottom - WorkArea.bottom), 0, ScreenArea.bottom}, .layer = -1, .title = "NoneAtGround"};
    TopWindow = (DesktopWindow){.rect = {0, ScreenArea.bottom, 0, ScreenArea.bottom}, .layer = -1, .title = "NoneAtTop"};

    StandingWindow = (DesktopWindow){.rect = {0, ScreenArea.bottom, 0, ScreenArea.bottom}, .layer = -1, .title = "NoneAtStanding"};

    for (int i = 0; i < count; i++)
    {
        RECT windowRect = windows[i].rect;
        if (petPosition.x > windowRect.left && petPosition.x < windowRect.right)
        {
            if (windowRect.top < GroundWindow.rect.top && petPosition.y <= windowRect.top && windowRect.top > 0)
                GroundWindow = windows[i];

            if (windowRect.top < TopWindow.rect.top && petPosition.y > windowRect.top)
                TopWindow = windows[i];
        }
    }

    if (TopWindow.layer == -1)
    {
        StandingWindow = GroundWindow;
    }
    else if (TopWindow.layer != -1 && GroundWindow.rect.top >= TopWindow.rect.bottom)
    {
        StandingWindow = GroundWindow;
    }
    else if (TopWindow.layer != -1 && GroundWindow.rect.top <= TopWindow.rect.bottom && GroundWindow.layer < TopWindow.layer)
    {
        StandingWindow = GroundWindow;
    }
    else
    {
        StandingWindow = (DesktopWindow){.rect = {0, ScreenArea.bottom, 0, ScreenArea.bottom}, .layer = -1, .title = "NoneAtGround"};
    }

    if ((abs(StandingWindow.rect.top - petPosition.y) < 10 && petVelocity.y > 0) || petPosition.y > ScreenArea.bottom)
    {
        petPosition.y = StandingWindow.rect.top - 5;
        petVelocity.y = 0.0f;
        petVelocity.x *= 0.95f;
    }

    DragPet();
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
        if (petPosition.x < WorkArea.left)
            petPosition.x = WorkArea.left;
        if (petPosition.x > WorkArea.right)
            petPosition.x = WorkArea.right;
        if (petPosition.y > GroundWindow.rect.bottom)
            petPosition.y = GroundWindow.rect.bottom;
    }
}

void DrawGame()
{
    // DrawFPS(10, 10); // Draw FPS in the top-left corner

    Rectangle sourceRec = {0.0f, 0.0f, (float)petSprite.width, (float)petSprite.height};
    Rectangle destRec = {petPosition.x, petPosition.y + 5, (float)petSprite.width, (float)petSprite.height};
    Vector2 origin = {(float)petSprite.width / 2, (float)petSprite.height};

    // Draw the texture with the specified parameters
    DrawTexturePro(petSprite, sourceRec, destRec, origin, 0.0f, WHITE);
}