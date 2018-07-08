#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

// Our headers
#include "global.h"

// This function creates a block of text
sfText* Text_Create(char* string, char* fontName, int charSize, sfColor color, sfVector2f position);
// This creates a button with some text
TYPE_BUTTON Utility_CreateButton(char stringText[50], float textSize, sfVector2f position, sfVector2f baseSize, sfColor cBase);
// This function returns 1 if mouse is on the button. Else, returns 0.
int Utility_isOnButton(TYPE_BUTTON *button, sfRenderWindow* window);


#endif // UTILITY_H_INCLUDED
