// Enemies' constants
#define MAXENEMIES 20
#define DIST_ENEMY_X 10
#define DIST_ENEMY_Y 50
#define SPEED_ENEMY 200
#define ENEMYFIRE_SPEED 300
// Video's constants
#define WIDTH 800
#define HEIGHT 600

#include "enemies.h"

TYPE_ENEMIES Enemy_Create(int color, int posX, int posY)
{
    TYPE_ENEMIES enemy;

    enemy.color = color;
    enemy.posX = posX;  // Making easier to lead with coordinates of enemy
    enemy.posY = posY;
    enemy.flag = 1;     // Seting him to alive (1)
    enemy.fire.flag = 0;

    return enemy;
}

void Enemies_Set(TYPE_LEVEL* level, TYPE_ENEMIES enemies[], int *nEnemies, int *liveEnemies)
{
    FILE *map;

    map = fopen(level->mapName, "r"); // It will open the file whose name is a parameter (dir: bin/debug)
    puts(level->mapName);

    int posXAux = 40;    // Initializing it
    int posYAux = 0;    // Initializing it
    int speed;          // It's the first char in the file map
    char direction;     // It's the second char in the file map
    char buffer = '\n'; // Initializing it

    rewind(map);

    fseek(map, 0, SEEK_SET);
    fscanf(map, "%d", &speed); // It will put the first char, as a number, in speed
    level->levelSpeed = speed;
    //printf("Velocidade do %s: %d", speed);
    fseek(map, 2, SEEK_SET);    // Jumping to second char of the file
    direction = getc(map);  // Putting the second char of the file, which is the direction of movement of the enemies, in direction
    level->direction = direction;

    *nEnemies = 0;
    *liveEnemies = 0;

    while(!feof(map))
    {
       buffer = getc(map);
       switch(buffer)
       {
           case '\n':   posYAux += DIST_ENEMY_Y;
                        posXAux = 40;                // It means that, in the file, we passed the an enemy's line, then we have to put the next enemies in other Y
                        break;

           case ' ':    posXAux += DIST_ENEMY_X;    // The blanks in the file means the distance between the enemies. This is the reason why we are adding posXAux
                        break;

           case 'x':    enemies[*nEnemies] = Enemy_Create(rand()%4, posXAux, posYAux);    // We are using rand, but the intention is to use a defined color
                        enemies[*nEnemies].initialPos = (sfVector2f){posXAux, posYAux};
                        printf("nEnemies = %d\n", *nEnemies);
                        printf("liveEnemies = %d\n", *liveEnemies);
                        *nEnemies += 1;
                        *liveEnemies += 1;
                        break;

           default:     break;
       }
    }
    fclose(map);
}

void Enemies_Move(TYPE_LEVEL level, TYPE_ENEMIES enemies[MAXENEMIES], int sizeArray, float dtime)
{
    int i; // Count
    switch(level.direction)
    {
        case 'R':   for(i = 0; i < sizeArray; i++)
                    {
                        if(enemies[i].posX >= 0 && enemies[i].posX <= 800 && enemies[i].flag == 1)
                        {
                            enemies[i].posX += SPEED_ENEMY*dtime*level.levelSpeed;
                        }
                        else
                        {
                            enemies[i].posX = 0;
                        }
                    }
                    break;
        case 'L':   for(i = 0; i < sizeArray; i++)
                    {
                        if(enemies[i].posX >= 0 && enemies[i].posX <= 800 && enemies[i].flag == 1)
                        {
                            enemies[i].posX -= SPEED_ENEMY*dtime*level.levelSpeed;
                        }
                        else
                        {
                            enemies[i].posX = 800;
                        }
                    }
                    break;
        default: break;
    }
}

void Enemies_Draw(sfRenderWindow* window, TYPE_ENEMIES enemies[MAXENEMIES], int sizeArray, TYPE_ALLSPRITES gameSprites)
{
    int i;
    sfVector2f bufferPos;

    for(i = 0; i < sizeArray; i++)
    {
        bufferPos.x = enemies[i].posX;
        bufferPos.y = enemies[i].posY;

        // This function only draws alive enemys
        if(enemies[i].flag == 1)
        {
            switch(enemies[i].color)
            {
            case 1:
                sfSprite_setPosition(gameSprites.enemyRed, bufferPos);
                sfRenderWindow_drawSprite(window, gameSprites.enemyRed, NULL);
                break;
            case 2:
                sfSprite_setPosition(gameSprites.enemyGreen, bufferPos);
                sfRenderWindow_drawSprite(window, gameSprites.enemyGreen, NULL);
                break;
            case 3:
                sfSprite_setPosition(gameSprites.enemyGreen, bufferPos);
                sfRenderWindow_drawSprite(window, gameSprites.enemyGreen, NULL);
                break;
            // If its anything else (including 0), it's gonna be black
            default:
                sfSprite_setPosition(gameSprites.enemyBlack, bufferPos);
                sfRenderWindow_drawSprite(window, gameSprites.enemyBlack, NULL);

            }
        }
    }
}

int Enemies_Shooting(TYPE_ENEMIES enemies[], int numberEnemies, int livingEnemies, int levelSpeed)
{
    int i;
    int aux;

    int fireNmb = 0;

    srand(time(NULL));

    if(livingEnemies >= levelSpeed)     // If livingEnemies >= levelSpeed, this for won't be in an eternal loop. But, if livingEnemies < levelSpeed, then it will.
        for(i = 0; i < levelSpeed; i++)
        {
            aux = rand() % numberEnemies;

            if(enemies[aux].flag == 1 && enemies[aux].fire.flag == 0)      // If the enemy is alive, he can shoot
            {
                enemies[aux].fire.flag = 1;
                enemies[aux].fire.posX = enemies[aux].posX;
                enemies[aux].fire.posY = enemies[aux].posY + 40;
                fireNmb++;
            }
            else                            // Else, he can't shoot. Then, we have to pick other enemy
                i--;
        }
    else
        for(i = 0; i < livingEnemies; i++)  // As the first for will be in an eternal loop if livingEnemies < levelSpeed, this won't.
        {
            aux = rand() % numberEnemies;

            if(enemies[aux].flag == 1 && enemies[aux].fire.flag == 0)      // If the enemy is alive, he can shoot
            {
                enemies[aux].fire.flag = 1;
                enemies[aux].fire.posX = enemies[aux].posX;
                enemies[aux].fire.posY = enemies[aux].posY + 40;
                fireNmb++;
            }
            else                            // Else, he can't shoot. Then, we have to pick other enemy
                i--;
        }

    return fireNmb;
}

void Enemies_DrawFires(sfRenderWindow *window, sfSprite* sprite, TYPE_ENEMIES enemies[], int numberEnemies)
{
    int i;

    for(i = 0; i < numberEnemies; i++)
    {
        if(enemies[i].fire.flag == 1)
        {
            sfSprite_setPosition(sprite, (sfVector2f){enemies[i].fire.posX, enemies[i].fire.posY});
            sfRenderWindow_drawSprite(window, sprite, NULL);
        }
    }
}

int Enemies_MovingFires(float speedY, TYPE_ENEMIES enemies[], int numberEnemies, float dtime, TYPE_PLAYERSHIP player)
{
    int i;
    int flag = 0; // If it's 1, one of these fires hit the player.

    for(i = 0; i < numberEnemies; i++)
    {
        if(enemies[i].fire.flag == 1)
        {
            if(enemies[i].fire.posY < HEIGHT &&
               !sfSprite_CollisionPoint(player.shipSprite, (sfVector2f){enemies[i].fire.posX, enemies[i].fire.posY}))
            {
                enemies[i].fire.posY += speedY*dtime;
            }
            else
                if(sfSprite_CollisionPoint(player.shipSprite, (sfVector2f){enemies[i].fire.posX, enemies[i].fire.posY}))
                {
                    enemies[i].fire.flag = 0;
                    flag = 1;
                }
                else
                    enemies[i].fire.flag = 0;
        }
    }
    return flag;
}

int Enemies_HowManyFires(TYPE_ENEMIES enemies[], int numberEnemies)
{
    int i;
    int nbFires = 0;

    for(i = 0; i < numberEnemies; i++)
    {
        if(enemies[i].fire.flag == 1)
            nbFires++;
    }

    return nbFires;
}
