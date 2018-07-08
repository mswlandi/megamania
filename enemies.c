/*
    Este Programa foi feito para o nosso trabalho final da cadeira de Algoritmos e Programação da UFRGS, em 2018/1.
    É um clone do jogo Megamania, originalmente lançado para Atari 2600, utilizando a linguagem C e uma biblioteca gráfica.

    A lógica de execução do programa, junto com o resto da descrição do trabalho, está descrita no documento do trabalho
    sob o título "A Lógica do Jogo".

    Este arquivo, junto com enemies.h, descreve as funções relacionadas aos inimigos, com o prefixo “Enemies_”.

    Autores: Henry de Avila e Marcos Landi

    08/07/2018
*/
#include "global.h"
#include "enemies.h"
#include "sprites.h"

TYPE_ENEMIES Enemy_Create(int color, int posX, int posY)
{
    TYPE_ENEMIES enemy;

    enemy.color = color;
    enemy.posX = posX;
    enemy.posY = posY;
    enemy.isAlive = 1;
    enemy.fire.isOnScreen = 0;

    return enemy;
}

void Enemies_Set(TYPE_LEVEL* level, int numberLevel, TYPE_ENEMIES enemies[], int *nEnemies, int *liveEnemies)
{
    FILE *map;

    char levelname[50] = "map_";
    char numLevelChar[2];
    char pontotxt[6] = ".txt";

    strcat(levelname, itoa(numberLevel, numLevelChar, 10));
    strcat(levelname, pontotxt);

    printf("Entrei no enemies set: %s\n", levelname);
    puts(levelname);
    map = fopen(levelname, "r"); // It will open the file whose name is a parameter (dir: bin/debug)

    int posXAux = 40;   // starting X position
    int posYAux = 0;
    int speed;          // It's the first char in the file map
    char direction;     // It's the second char in the file map
    char buffer = '\n';

    rewind(map);

    fseek(map, 0, SEEK_SET);
    fscanf(map, "%d", &speed);  // It will put the first char, as a number, in speed
    level->levelSpeed = speed;
    fseek(map, 2, SEEK_SET);    // Jumping to second char of the file
    direction = getc(map);      // Getting the second char of the file, which is the direction of movement of the enemies
    level->direction = direction;

    *nEnemies = 0;
    *liveEnemies = 0;

    while(!feof(map))
    {
       buffer = getc(map);
       switch(buffer)
       {
           case '\n':   posYAux += DIST_ENEMY_Y;    // A new line means we have to increase the Y position, and also
                        posXAux = 40;               // go back to the starting X position
                        break;

           case ' ':    posXAux += DIST_ENEMY_X;    // Blanks means more space between the enemies
                        break;

           case 'x':    enemies[*nEnemies] = Enemy_Create(rand()%4, posXAux, posYAux);    // Setting the color to a random one (4 possibilities)
                        enemies[*nEnemies].initialPos = (sfVector2f){posXAux, posYAux};
                        *nEnemies += 1;
                        *liveEnemies += 1;
                        break;

           default:     break;
       }
    }
    fclose(map);
}

void Enemies_Move(TYPE_LEVEL level, TYPE_ENEMIES enemies[MAXENEMIES], int sizeArray, float dtime, float *number)
{
    int i;
    for(i = 0; i < sizeArray; i++)
    {
        if(enemies[i].posX >= 0 && enemies[i].posX <= 800 && enemies[i].isAlive == 1)
        {
            //Moves the enemy like this: StandardSpeed*levelSpeed/second
            if(level.direction == 'R')
                enemies[i].posX += SPEED_ENEMY*dtime*level.levelSpeed;
            if(level.direction == 'L')
                enemies[i].posX -= SPEED_ENEMY*dtime*level.levelSpeed;
            if(level.direction == 'S')
            {
                enemies[i].posX += SPEED_ENEMY*dtime*level.levelSpeed;
                enemies[i].posY += sin(Utility_Counting(number, dtime))*120*dtime;
            }

        }
        else
        {
            if(level.direction == 'R' || level.direction == 'S')
                enemies[i].posX = 0;
            else if(level.direction == 'L')
                enemies[i].posX = WIDTH;
        }
    }
}

void Enemies_Draw(sfRenderWindow* window, TYPE_ENEMIES enemies[MAXENEMIES], int sizeArray, TYPE_GAMEOBJECTS gameObjects)
{
    int i;
    sfVector2f bufferPos;

    for(i = 0; i < sizeArray; i++)
    {
        bufferPos.x = enemies[i].posX;
        bufferPos.y = enemies[i].posY;

        // This function only draws alive enemys
        if(enemies[i].isAlive == 1)
        {
            switch(enemies[i].color)
            {
            case 1:
                sfSprite_setPosition(gameObjects.enemyRed, bufferPos);
                sfRenderWindow_drawSprite(window, gameObjects.enemyRed, NULL);
                break;
            case 2:
                sfSprite_setPosition(gameObjects.enemyGreen, bufferPos);
                sfRenderWindow_drawSprite(window, gameObjects.enemyGreen, NULL);
                break;
            case 3:
                sfSprite_setPosition(gameObjects.enemyBlue, bufferPos);
                sfRenderWindow_drawSprite(window, gameObjects.enemyBlue, NULL);
                break;
            // If its anything else (including 0), it's gonna be black
            default:
                sfSprite_setPosition(gameObjects.enemyBlack, bufferPos);
                sfRenderWindow_drawSprite(window, gameObjects.enemyBlack, NULL);

            }
        }
    }
}

void Enemies_Shooting(TYPE_ENEMIES enemies[], int numberEnemies, int livingEnemies, int levelSpeed)
{
    int aux;

    srand(time(NULL));

    // If no one could shoot, but some enemy still be alive, this would be an infinite loop
    if(Enemies_canShoot(enemies, numberEnemies))
    {
        // Yes, with a lower ammount of enemies, the longer this loop could take, but
        // it's just too little to make a difference
        do{
            aux = rand() % numberEnemies;
        }while(!enemies[aux].isAlive || enemies[aux].fire.isOnScreen);

        enemies[aux].fire.isOnScreen = 1;
        enemies[aux].fire.posX = enemies[aux].posX;
        enemies[aux].fire.posY = enemies[aux].posY + 40;
    }
}

void Enemies_DrawFires(sfRenderWindow *window, sfSprite* sprite, TYPE_ENEMIES enemies[], int numberEnemies)
{
    int i;

    for(i = 0; i < numberEnemies; i++)
    {
        if(enemies[i].fire.isOnScreen == 1)
        {
            sfSprite_setPosition(sprite, (sfVector2f){enemies[i].fire.posX, enemies[i].fire.posY});
            sfRenderWindow_drawSprite(window, sprite, NULL);
        }
    }
}

int Enemies_MovingFires(float speedY, TYPE_ENEMIES enemies[], int numberEnemies, float dtime, TYPE_PLAYERSHIP player)
{
    int i;
    int didHit = 0; // If it's 1, one of these fires hit the player.

    for(i = 0; i < numberEnemies; i++)
    {
        if(enemies[i].fire.isOnScreen == 1)
        {
            // If the fire is in the screen and not in the player's ship
            if(enemies[i].fire.posY < HEIGHT &&
               !sfSprite_CollisionPoint(player.shipSprite, (sfVector2f){enemies[i].fire.posX, enemies[i].fire.posY}))
            {
                enemies[i].fire.posY += speedY*dtime;
            }
            else
            {
                enemies[i].fire.isOnScreen = 0;
                // If it's in the player's ship
                if(sfSprite_CollisionPoint(player.shipSprite, (sfVector2f){enemies[i].fire.posX, enemies[i].fire.posY}))
                {
                    didHit = 1;
                }
            }
        }
    }
    return didHit;
}

int Enemies_HowManyFires(TYPE_ENEMIES enemies[], int numberEnemies)
{
    int i;
    int nbFires = 0;

    for(i = 0; i < numberEnemies; i++)
    {
        if(enemies[i].fire.isOnScreen == 1)
            nbFires++;
    }

    return nbFires;
}

int Enemies_canShoot(TYPE_ENEMIES enemies[], int numberEnemies)
{
    int i;
    int canThey = 0;

    // This loop is ment to toggle if one of the enemies can shoot
    for(i=0; i<numberEnemies; i++)
    {
        if(enemies[i].isAlive && !enemies[i].fire.isOnScreen)
            canThey = 1;
    }

    return canThey;
}

void Enemies_destroyFires(TYPE_ENEMIES enemies[], int numberEnemies)
{
    int i;

    // By "destroying", we mean just toggling the flag off
    for(i=0; i<numberEnemies; i++)
    {
        enemies[i].fire.isOnScreen = 0;
    }
}

int Enemies_isAtSamePoint(TYPE_ENEMIES* enemies, int sizeArray, sfSprite* sprite)
{
    int i;

    sfFloatRect enemyRect;
    sfFloatRect spriteRect = sfSprite_getGlobalBounds(sprite);

    int numberOfEnemyDead = -1; // If all the enemies in the array are alive, it will be -1.
                                // Else, it will be the number of the enemy dead in the array.

    float sizeEnemyX = sfSprite_getLocalBounds(gameObjects.enemyBlack).width;
    float sizeEnemyY = sfSprite_getLocalBounds(gameObjects.enemyBlack).height;

    for(i = 0; i < sizeArray; i++)
    {
        // The top-left corner is calculated according to the position
        // of the sprite, which is at its center.
        enemyRect.top = enemies[i].posY - sizeEnemyY/2;
        enemyRect.left = enemies[i].posX - sizeEnemyX/2;
        enemyRect.width = sizeEnemyX;
        enemyRect.height = sizeEnemyY;

        // If the enemy intersects with the fire, return the index number of the enemy
        if(sfFloatRect_intersects(&enemyRect, &spriteRect, NULL) && enemies[i].isAlive == 1)
        {
            numberOfEnemyDead = i;
        }
    }

    return numberOfEnemyDead;
}
