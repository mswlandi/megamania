/*
    Este Programa foi feito para o nosso trabalho final da cadeira de Algoritmos e Programa��o da UFRGS, em 2018/1.
    � um clone do jogo Megamania, originalmente lan�ado para Atari 2600, utilizando a linguagem C e uma biblioteca gr�fica.

    A l�gica de execu��o do programa, junto com o resto da descri��o do trabalho, est� descrita no documento do trabalho
    sob o t�tulo "A L�gica do Jogo".

    Este arquivo � onde fica o main(), a fun��o inicial que � executada quando o jogo � iniciado.

    Autores: Henry de Avila e Marcos Landi

    08/07/2018
*/

#include "global.h"
#include "utility.h"
#include "enemies.h"
#include "score.h"
#include "layout.h"
#include "sprites.h"

int main()
{
    // Loads all of the game's sprites
    loadGameSprites(&gameObjects);

    // Video variables
    sfVideoMode mode = {WIDTH, HEIGHT, 32};
    sfRenderWindow* window;

    // Create the main window
    window = sfRenderWindow_create(mode, "Megamania", sfResize | sfClose, NULL);

    // Playing level
    Layout_GameMenu(window);

    // Cleaning everything
    cleanGameStuff(&gameObjects, window);

    return 0;
}
