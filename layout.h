/*
    Este Programa foi feito para o nosso trabalho final da cadeira de Algoritmos e Programa��o da UFRGS, em 2018/1.
    � um clone do jogo Megamania, originalmente lan�ado para Atari 2600, utilizando a linguagem C e uma biblioteca gr�fica.

    A l�gica de execu��o do programa, junto com o resto da descri��o do trabalho, est� descrita no documento do trabalho
    sob o t�tulo "A L�gica do Jogo".

    Este arquivo, junto com layout.c, descreve as �fun��es layout� e outras fun��es relacionadas aos layouts,
    todas com o prefixo �Layout_�.

    Autores: Henry de Avila e Marcos Landi

    08/07/2018
*/
#ifndef LAYOUT_H_INCLUDED
#define LAYOUT_H_INCLUDED

// Our headers
#include "global.h"
#include "utility.h"
#include "sprites.h"
#include "enemies.h"
#include "score.h"

// This creates a button with some text. Its origin is in the middle of button.
TYPE_BUTTON Layout_CreateButton(char stringText[50], float textSize, sfVector2f position, sfVector2f baseSize, sfColor cBase);
// This function returns 1 if mouse is on the button. Else, returns 0.
int Layout_isOnButton(TYPE_BUTTON *button, sfRenderWindow* window);
// This function shows the ranking screen when you click on highscores button.
void Layout_Highscores(sfRenderWindow* window, sfSprite* background);
// This function shows the credits screen.
void Layout_Credits(sfRenderWindow* window, sfSprite* background);
// This loads all the stage (player ship, enemies, background, logic, ..., using a lot of others functions).
void Layout_Stage(sfRenderWindow* window, TYPE_LEVEL level);
// This is the menu of the game, loaded when you launch .exe
void Layout_GameMenu(sfRenderWindow* window);
// This loads Game Over screen.
void Layout_GameOver(sfRenderWindow* window, sfEvent event);
// This function shows a specific screen when you win the game (all the levels finished).
void Layout_JustWon(sfRenderWindow* window, sfSprite* background, int flagRank);

#endif // LAYOUT_H_INCLUDED
