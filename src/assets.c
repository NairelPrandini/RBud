#include "raylib.h"
#include "assets.h"

// Sprites from Assets
Texture2D petSprite;

void LoadAssets()
{
    // Load a PNG image
    petSprite = LoadTexture("res/kai.png");
}

void UnloadAssets()
{
    UnloadTexture(petSprite);
}