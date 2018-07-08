/*
    Este Programa foi feito para o nosso trabalho final da cadeira de Algoritmos e Programação da UFRGS, em 2018/1.
    É um clone do jogo Megamania, originalmente lançado para Atari 2600, utilizando a linguagem C e uma biblioteca gráfica.

    A lógica de execução do programa, junto com o resto da descrição do trabalho, está descrita no documento do trabalho
    sob o título "A Lógica do Jogo".

    Este arquivo, junto com score.c, descreve funções relacionadas ao score de cada jogo. São precedidas pelo prefixo “Score_”.

    Autores: Henry de Avila e Marcos Landi

    08/07/2018
*/
#ifndef SCORE_H_INCLUDED
#define SCORE_H_INCLUDED

#include "global.h"
#include "sprites.h"
#include "enemies.h"
#include "layout.h"
#include "utility.h"

// This function adds a new highscore to TOP 5
int Score_AddHighScore(FILE *highscores, int score);
// This function creates a text by given score
sfText* Score_TextCreate(int score, sfVector2f posScore, int charSize);
// This function creates the text "S C O R E";
sfText* Score_CreateLayout(sfVector2f position, int charSize);
// This sets the score according the current energy bar, after the level's end
float Score_EnergyBar (float energy, float maxEnergy);

#endif // SCORE_H_INCLUDED
