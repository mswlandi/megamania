#ifndef LAYOUT_H_INCLUDED
#define LAYOUT_H_INCLUDED

// Our headers
#include "global.h"
#include "utility.h"
#include "sprites.h"
#include "enemies.h"
#include "score.h"

// This creates a button with some text
TYPE_BUTTON Layout_CreateButton(char stringText[50], float textSize, sfVector2f position, sfVector2f baseSize, sfColor cBase);
// This function returns 1 if mouse is on the button. Else, returns 0.
int Layout_isOnButton(TYPE_BUTTON *button, sfRenderWindow* window);
// This function shows the screen when you click on highscores button
void Layout_Highscores(sfRenderWindow* window, sfSprite* background);
// This function shows the credits screen
void Layout_Credits(sfRenderWindow* window, sfSprite* background);
// This loads all the stage (player ship, enemies, background, logic, ..., using a lot of others functions)
void Layout_Stage(sfRenderWindow* window, TYPE_LEVEL level);
// This is the menu of the game
void Layout_GameMenu(sfRenderWindow* window);
// This loads Game Over screen
void Layout_GameOver(sfRenderWindow* window, sfEvent event);

#endif // LAYOUT_H_INCLUDED
