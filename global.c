/*
    Este Programa foi feito para o nosso trabalho final da cadeira de Algoritmos e Programação da UFRGS, em 2018/1.
    É um clone do jogo Megamania, originalmente lançado para Atari 2600, utilizando a linguagem C e uma biblioteca gráfica.

    A lógica de execução do programa, junto com o resto da descrição do trabalho, está descrita no documento do trabalho
    sob o título "A Lógica do Jogo".

    Neste arquivo, junto com global.h, é onde são declaradas as estruturas e variáveis globais usadas no programa.
    Também é onde são definidas constantes e incluídas algumas bibliotecas padrão do C, utilizadas nos outros arquivos.

    Autores: Henry de Avila e Marcos Landi

    08/07/2018
*/
#include "global.h"

float energy = ENERGYMAX;
int numberlifes = 3;
int nEnemies = 0;
int liveEnemies = 0;
int score = 0;



