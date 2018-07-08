/*
    Este Programa foi feito para o nosso trabalho final da cadeira de Algoritmos e Programação da UFRGS, em 2018/1.
    É um clone do jogo Megamania, originalmente lançado para Atari 2600, utilizando a linguagem C e uma biblioteca gráfica.

    A lógica de execução do programa, junto com o resto da descrição do trabalho, está descrita no documento do trabalho
    sob o título "A Lógica do Jogo".

    Este arquivo, junto com utility.c, descreve funções úteis variadas. São precedidas pelo prefixo “Utility_”

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
