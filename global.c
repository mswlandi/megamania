/*
    Este Programa foi feito para o nosso trabalho final da cadeira de Algoritmos e Programa��o da UFRGS, em 2018/1.
    � um clone do jogo Megamania, originalmente lan�ado para Atari 2600, utilizando a linguagem C e uma biblioteca gr�fica.

    A l�gica de execu��o do programa, junto com o resto da descri��o do trabalho, est� descrita no documento do trabalho
    sob o t�tulo "A L�gica do Jogo".

    Neste arquivo, junto com global.h, � onde s�o declaradas as estruturas e vari�veis globais usadas no programa.
    Tamb�m � onde s�o definidas constantes e inclu�das algumas bibliotecas padr�o do C, utilizadas nos outros arquivos.

    Autores: Henry de Avila e Marcos Landi

    08/07/2018
*/
#include "global.h"

float energy = ENERGYMAX;
int numberlifes = 3;
int nEnemies = 0;
int liveEnemies = 0;
int score = 0;



