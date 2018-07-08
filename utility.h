/*
    Este Programa foi feito para o nosso trabalho final da cadeira de Algoritmos e Programa��o da UFRGS, em 2018/1.
    � um clone do jogo Megamania, originalmente lan�ado para Atari 2600, utilizando a linguagem C e uma biblioteca gr�fica.

    A l�gica de execu��o do programa, junto com o resto da descri��o do trabalho, est� descrita no documento do trabalho
    sob o t�tulo "A L�gica do Jogo".

    Este arquivo, junto com utility.c, descreve fun��es �teis variadas. S�o precedidas pelo prefixo �Utility_�

    Autores: Henry de Avila e Marcos Landi

    08/07/2018
*/
#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include "global.h"

// This function creates a block of text
sfText* Utility_TextCreate(char* string, char* fontName, int charSize, sfColor color, sfVector2f position);
// This creates a button with some text
TYPE_BUTTON Utility_CreateButton(char stringText[50], float textSize, sfVector2f position, sfVector2f baseSize, sfColor cBase);
// This function returns 1 if mouse is on the button. Else, returns 0.
int Utility_isOnButton(TYPE_BUTTON *button, sfRenderWindow* window);


#endif
