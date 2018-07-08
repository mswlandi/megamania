#ifndef ENEMIES_H_INCLUDED
#define ENEMIES_H_INCLUDED

#include "global.h"

// This creates one enemy and returns it.
TYPE_ENEMIES Enemy_Create(int color, int posX, int posY);
// This set the enemies by a level, which has the name of the map in its content
void Enemies_Set(TYPE_LEVEL* level, TYPE_ENEMIES enemies[], int *nEnemies, int *liveEnemies);
// This draws the enemies, one by one, if it's alive.
void Enemies_Draw(sfRenderWindow* window, TYPE_ENEMIES enemies[MAXENEMIES], int sizeArray, TYPE_GAMEOBJECTS gameObjects);
// This moves the enemies according the level
void Enemies_Move(TYPE_LEVEL level, TYPE_ENEMIES enemies[MAXENEMIES], int sizeArray, float dtime);
// This function sets which enemies will shoot in one frame
void Enemies_Shooting(TYPE_ENEMIES enemies[], int numberEnemies, int livingEnemies, int levelSpeed);
// This function draws all the enemies's fires which are enabled.
void Enemies_DrawFires(sfRenderWindow *window, sfSprite* sprite, TYPE_ENEMIES enemies[], int numberEnemies);
// This function moves all the fires which are enabled. Also, it checks if the player was hit
int Enemies_MovingFires(float speedY, TYPE_ENEMIES enemies[], int numberEnemies, float dtime, TYPE_PLAYERSHIP player);
// This function returns the quantity of current enemies's fires on the screen.
int Enemies_HowManyFires(TYPE_ENEMIES enemies[], int numberEnemies);
// This function returns true (1) if at least 1 enemy is able to shoot.
int Enemies_canShoot(TYPE_ENEMIES enemies[], int numberEnemies);
// This destroys all of the enemies' fires
void Enemies_destroyFires(TYPE_ENEMIES enemies[], int numberEnemies);
// This returns the number of enemy in the array, if the sprite given has the same position than that enemy. Else, it returns -1.
int Enemies_isAtSamePoint(TYPE_ENEMIES* enemies, int sizeArray, sfSprite* sprite);

#endif
