/*
    Este Programa foi feito para o nosso trabalho final da cadeira de Algoritmos e Programação da UFRGS, em 2018/1.
    É um clone do jogo Megamania, originalmente lançado para Atari 2600, utilizando a linguagem C e uma biblioteca gráfica.

    A lógica de execução do programa, junto com o resto da descrição do trabalho, está descrita no documento do trabalho
    sob o título "A Lógica do Jogo".

    Este arquivo é onde fica o main(), a função inicial que é executada quando o jogo é iniciado.

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
