#ifndef SPRITES_H_INCLUDED
#define SPRITES_H_INCLUDED

// Default headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// CSFML header
#include <SFML/Graphics.h>
#include "structs.h"


// This bundles together a sprite with a texture, and the origin in the middle.
sfSprite* sfSprite_createFromFile(char* spriteMode, const char* filename, sfVector2f scale, sfVector2f pos);
// This function check if the point is at the same point of the sprite
int sfSprite_CollisionPoint(sfSprite* sprite, sfVector2f point);
// This will check if the sprite is out of the screen
int outOfScreen(sfSprite* sprite, float hScreen, float wScreen);
// This draws the lifes on the screen (uses sfSprite_createFromFile).
void drawLifes(sfRenderWindow* window, sfSprite* life, int* numL);

#endif // SPRITES_H_INCLUDED
