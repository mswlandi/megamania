#ifndef LAYOUT_H_INCLUDED
#define LAYOUT_H_INCLUDED

// Default headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// CSFML header
#include <SFML/Graphics.h>
// Our headers
#include "structs.h"
#include "utility.h"

// This creates a button with some text
TYPE_BUTTON Layout_CreateButton(char stringText[50], float textSize, sfVector2f position, sfVector2f baseSize, sfColor cBase);
// This function returns 1 if mouse is on the button. Else, returns 0.
int Layout_isOnButton(TYPE_BUTTON *button, sfRenderWindow* window);
// This function shows the screen when you click on highscores button
void Layout_Highscores(sfRenderWindow* window, sfSprite* background);
// This function shows the credits screen
void Layout_Credits(sfRenderWindow* window, sfSprite* background);

#endif // LAYOUT_H_INCLUDED
