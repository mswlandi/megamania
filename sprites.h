#ifndef SPRITES_H_INCLUDED
#define SPRITES_H_INCLUDED

#include "global.h"
#include "utility.h"
#include "enemies.h"
#include "score.h"
#include "layout.h"


// This bundles together a sprite with a texture, and the origin in the middle.
sfSprite* sfSprite_createFromFile(const char* filename, sfVector2f scale, sfVector2f pos);
// This function check if the point is at the same point of the sprite
int sfSprite_CollisionPoint(sfSprite* sprite, sfVector2f point);
// This will check if the sprite is out of the screen
int outOfScreen(sfSprite* sprite, float hScreen, float wScreen);
// This draws the lifes on the screen (uses sfSprite_createFromFile).
void drawLifes(sfRenderWindow* window, sfSprite* life, int* numL);
// This function loads all the sprites that we use in the game
void loadgameObjects(TYPE_GAMEOBJECTS* gameObjects);
// This function cleans (probably) every objec that has a destroy function in the game
void cleanGameStuff(TYPE_GAMEOBJECTS* gameObjects, sfRenderWindow* window);

#endif
