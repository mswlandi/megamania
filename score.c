#include "score.h"

int Score_AddHighScore(FILE *highscores, int score)
{
    int top[5] = {0};
    int i = 0;
    int j = 0;
    int answer = 0;
    int scoreBuffer = 0;
    char stringBuffer[60];
    char buffer;

    highscores = fopen("highscores.txt", "r+");

    rewind(highscores);

    printf("Score antes de entrar no while: %d\n", score);

    if(!highscores)
        printf("Open highscores.txt error\n");
    else
    {
        while(!feof(highscores))
        {
            buffer = getc(highscores);
            if(buffer != '\n')
            {
                stringBuffer[i] = buffer;
                stringBuffer[i+1] = '\0';
                i++;
            }
            else
            {
                top[j] = atoi(stringBuffer);
                i = 0;
                j++;
            }
        }

        for(i = 0; i < 5; i++) // Teste
            printf("Numero %d: %d\n", i+1, top[i]);

        for(i = 0; i < 5; i++)
        {
            if(score > top[i])
            {
                answer = (i+1);
                scoreBuffer = top[i];
                top[i] = score;
                for(j = (i+1); j < 5; j++)
                {
                    score = top[j]; // Score is being used as a buffer
                    top[j] = scoreBuffer;
                    scoreBuffer = score;
                }
                i = 5;
            }
        }

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

    nbrScore = Text_Create(stringScore, "Quantify Bold v2.6.ttf", charSize, sfColor_fromRGB(255,255,255), posScore);

    return nbrScore;
}

sfText* Score_CreateLayout(sfVector2f position, int charSize)
{
    sfText* scoreText;

    scoreText = Text_Create("S C O R E", "Quantify Bold v2.6.ttf", charSize, sfColor_fromRGB(255, 255, 255), position);

    return scoreText;
}

float Score_EnergyBar(float energy, float maxEnergy)
{
    float answer;

    answer = energy/(maxEnergy/40) * 50;    // We want the energy bar to have 40 parts, so we divide it by 40 and divide the current energy by this
                                            // fraction, that tells us how many "slices" of energy we have. each slice is worth 50 energy (which is defined at the PDF)
    return answer;
}
