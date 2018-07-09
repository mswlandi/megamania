/*
    Este Programa foi feito para o nosso trabalho final da cadeira de Algoritmos e Programação da UFRGS, em 2018/1.
    É um clone do jogo Megamania, originalmente lançado para Atari 2600, utilizando a linguagem C e uma biblioteca gráfica.

    A lógica de execução do programa, junto com o resto da descrição do trabalho, está descrita no documento do trabalho
    sob o título "A Lógica do Jogo".

    Este arquivo, junto com score.h, descreve funções relacionadas ao score de cada jogo. São precedidas pelo prefixo “Score_”.

    Autores: Henry de Avila e Marcos Landi

    08/07/2018
*/
#include "global.h"
#include "score.h"

int Score_AddHighScore(FILE *highscores, int score)
{
    int top[5] = {0};
    int i = 0;  // Count
    int j = 0;  // Count
    int answer = 0; // It's the position this score deserves
    int scoreBuffer = 0;
    char stringBuffer[60];
    char buffer;

    // Opening file
    highscores = fopen("highscores.txt", "r+");

    rewind(highscores);

    if(!highscores)
        printf("Open highscores.txt error\n");
    else
    {
        /// Reading highscores.txt
        while(!feof(highscores))
        {
            buffer = getc(highscores);
            if(buffer != '\n')
            {
                stringBuffer[i] = buffer;
                stringBuffer[i+1] = '\0';   // Putting an end after the last number of the score
                i++;
            }
            else
            {
                top[j] = atoi(stringBuffer);
                i = 0;
                j++;
            }
        }

        /// Comparing highscores.txt with given score
        for(i = 0; i < 5; i++)
        {
            if(score > top[i])
            {
                answer = (i+1);
                scoreBuffer = top[i];
                top[i] = score;
                for(j = (i+1); j < 5; j++)
                {
                    score = top[j]; // Score is being used as a buffer here
                    top[j] = scoreBuffer;
                    scoreBuffer = score;
                }
                i = 5;
            }
        }

        /// Writing in highscores.txt
        rewind(highscores);
        for(i = 0; i < 5; i++)
        {
            itoa(top[i], stringBuffer, 10);
            fprintf(highscores, "%s\n", stringBuffer);
        }

        fclose(highscores);
    }

    return answer;
}

sfText* Score_TextCreate(int score, sfVector2f posScore, int charSize)
{
    sfText* nbrScore;
    char stringScore[] = "0";

    itoa(score, stringScore, 10);

    nbrScore = Utility_TextCreate(stringScore, "Quantify Bold v2.6.ttf", charSize, sfColor_fromRGB(255,255,255), posScore);

    return nbrScore;
}

sfText* Score_CreateLayout(sfVector2f position, int charSize)
{
    sfText* scoreText;

    scoreText = Utility_TextCreate("S C O R E", "Quantify Bold v2.6.ttf", charSize, sfColor_fromRGB(255, 255, 255), position);

    return scoreText;
}

float Score_EnergyBar(float energy, float maxEnergy)
{
    float answer;

    answer = energy/(maxEnergy/40) * 50;    // We want the energy bar to have 40 parts, so we divide it by 40 and divide the current energy by this fraction,
                                            // that tells us how many "slices" of energy we have. each slice is worth 50 energy (which is defined in the PDF)
    return answer;
}

