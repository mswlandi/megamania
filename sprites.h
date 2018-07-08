/*
    Este Programa foi feito para o nosso trabalho final da cadeira de Algoritmos e Programação da UFRGS, em 2018/1.
    É um clone do jogo Megamania, originalmente lançado para Atari 2600, utilizando a linguagem C e uma biblioteca gráfica.

    A lógica de execução do programa, junto com o resto da descrição do trabalho, está descrita no documento do trabalho
    sob o título "A Lógica do Jogo".

    Este arquivo, junto com sprites.c, descreve funções relacionadas aos Sprites, tanto para lidar com sprites como funções
    para desenhar elementos gráficos específicos.

    Autores: Henry de Avila e Marcos Landi

    08/07/2018
*/
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
void loadGameSprites(TYPE_GAMEOBJECTS* gameObjects);
// This function cleans (probably) every objec that has a destroy function in the game
void cleanGameStuff(TYPE_GAMEOBJECTS* gameObjects, sfRenderWindow* window);

#endif
