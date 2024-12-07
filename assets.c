#include "raylib.h"
#include "./headers/assets.h"

Texture2D petSprite;

void LoadAssets()
{
    // Load textures, sounds, etc.
    petSprite = LoadTexture("assets/pet.png");
}

void UnloadAssets()
{
    // Unload textures, sounds, etc.
    UnloadTexture(petSprite);
}