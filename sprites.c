/*
    Este Programa foi feito para o nosso trabalho final da cadeira de Algoritmos e Programação da UFRGS, em 2018/1.
    É um clone do jogo Megamania, originalmente lançado para Atari 2600, utilizando a linguagem C e uma biblioteca gráfica.

    A lógica de execução do programa, junto com o resto da descrição do trabalho, está descrita no documento do trabalho
    sob o título "A Lógica do Jogo".

    Este arquivo, junto com sprites.h, descreve funções relacionadas aos Sprites, tanto para lidar com sprites como funções
    para desenhar elementos gráficos específicos.

    Autores: Henry de Avila e Marcos Landi

    08/07/2018
*/
#include "sprites.h"

sfSprite* sfSprite_createFromFile(const char* filename, sfVector2f scale, sfVector2f pos)
{
    sfTexture* texture;
    sfSprite* sprite;

    char img[40];
    strcpy(img, "imgs/");

    texture = sfTexture_createFromFile(strcat(img,filename), NULL);
    sprite = sfSprite_create();

    sfSprite_setTexture(sprite, texture, sfTrue);
    sfSprite_setOrigin(sprite, (sfVector2f){sfSprite_getLocalBounds(sprite).width/2,
                                         sfSprite_getLocalBounds(sprite).height/2});
    sfSprite_scale(sprite, scale);
    sfSprite_setPosition(sprite, pos);

    return sprite;
 }

 int sfSprite_CollisionPoint(sfSprite* sprite, sfVector2f point)
{
    int answer = 0;

    float spriteWidth = sfSprite_getLocalBounds(sprite).width;
    float spriteHeight = sfSprite_getLocalBounds(sprite).height;

    float posX = sfSprite_getPosition(sprite).x;
    float posY = sfSprite_getPosition(sprite).y;

    if(point.x >= (posX - spriteWidth/2.0) &&
       point.x <= (posX + spriteWidth/2.0) &&
       point.y >= (posY - spriteHeight/2.0)&&
       point.y <= (posY + spriteHeight/2.0))
       answer = 1;

    return answer;
}

int outOfScreen(sfSprite* sprite, float hScreen, float wScreen)
{
    int answer = 0;

    if(sfSprite_getPosition(sprite).x > wScreen || sfSprite_getPosition(sprite).x < 0 ||
       sfSprite_getPosition(sprite).y > hScreen || sfSprite_getPosition(sprite).y < -40)
        answer = 1;

    return answer;
}

void drawLifes(sfRenderWindow* window, sfSprite* life, int* numL)
{
    int i;

    for(i=0; i < *numL; i++)
    {
        sfSprite_setPosition(life,(sfVector2f){350 + 50*i, 575});       // The first life's position is ( 350, 575)
        sfRenderWindow_drawSprite(window, life, NULL);
    }
}

void loadGameSprites(TYPE_GAMEOBJECTS* gameObjects)
{
    /// Initializing background
    gameObjects->menuBackground = sfSprite_createFromFile("background.png",
                                                        (sfVector2f){3.125, 2.3475},
                                                        (sfVector2f){WIDTH/2, HEIGHT/2});

    gameObjects->ship.shipSprite = sfSprite_createFromFile("nave.png",
                                                         (sfVector2f){0.8,0.8},
                                                         (sfVector2f){WIDTH/2, 450});

    // Fire
    gameObjects->fire = sfSprite_createFromFile("fire.png",
                                            (sfVector2f){0.5,0.5},
                                            (sfVector2f){-40, -40});

    // Enemy fire
    gameObjects->enemyFire = sfSprite_createFromFile("fire.png",
                                            (sfVector2f){0.5,0.5},
                                            (sfVector2f){-40, -40});
    sfSprite_setColor(gameObjects->enemyFire, sfColor_fromRGB(255, 100, 255));

    // Background
    gameObjects->background = sfSprite_createFromFile("background.png",
                                                  (sfVector2f){3.125, 2.3475},
                                                  (sfVector2f){WIDTH/2, HEIGHT/2});

    // Lifes
    gameObjects->life = sfSprite_createFromFile("life.png",
                                            (sfVector2f){1,1},
                                            (sfVector2f){350,575});

    // Life bar
    gameObjects->lifebar = sfSprite_createFromFile("lifebar.png",
                                               (sfVector2f){2, 1},
                                               (sfVector2f){WIDTH/2, 525});

    gameObjects->gameover = sfSprite_createFromFile("gameover.png",
                                                  (sfVector2f){1,1},
                                                  (sfVector2f){WIDTH/2, HEIGHT/2});

    gameObjects->enemyBlack = sfSprite_createFromFile("enemyBlack.png", (sfVector2f){ 0.7, 0.7}, (sfVector2f){-100, -100});
    gameObjects->enemyRed = sfSprite_createFromFile("enemyBlue.png", (sfVector2f){ 0.7, 0.7}, (sfVector2f){-100, -100});
    gameObjects->enemyGreen = sfSprite_createFromFile("enemyRed.png", (sfVector2f){ 0.7, 0.7}, (sfVector2f){-100, -100});
    gameObjects->enemyBlue = sfSprite_createFromFile("enemyGreen.png", (sfVector2f){ 0.7, 0.7}, (sfVector2f){-100, -100});

    gameObjects->fillLifeBar = sfRectangleShape_create();
    sfRectangleShape_setSize(gameObjects->fillLifeBar, (sfVector2f){436, 35});
    sfRectangleShape_setPosition(gameObjects->fillLifeBar, (sfVector2f){182, 507.5});
    sfRectangleShape_setFillColor(gameObjects->fillLifeBar, sfColor_fromRGB(255,255,255));

    gameObjects->fillLifeBar2 = sfRectangleShape_create();
    sfRectangleShape_setSize(gameObjects->fillLifeBar2, (sfVector2f){ENERGYMAX, ENERGYY});
    sfRectangleShape_setPosition(gameObjects->fillLifeBar2, (sfVector2f){182, 507.5});
    sfRectangleShape_setFillColor(gameObjects->fillLifeBar2, sfColor_fromRGB(100,0,0));

    // GUI
    gameObjects->base = sfRectangleShape_create();
    sfRectangleShape_setSize(gameObjects->base, (sfVector2f){WIDTH, 100});
    sfRectangleShape_setPosition(gameObjects->base, (sfVector2f){0, 500});
    sfRectangleShape_setFillColor(gameObjects->base, sfColor_fromRGB(150,0,0));
}

void cleanGameStuff(TYPE_GAMEOBJECTS* gameObjects, sfRenderWindow* window)
{
    sfSprite_destroy(gameObjects->ship.shipSprite);
    sfSprite_destroy(gameObjects->fire);
    sfSprite_destroy(gameObjects->enemyFire);
    sfSprite_destroy(gameObjects->menuBackground);
    sfSprite_destroy(gameObjects->background);
    sfSprite_destroy(gameObjects->life);
    sfSprite_destroy(gameObjects->lifebar);
    sfSprite_destroy(gameObjects->gameover);
    sfSprite_destroy(gameObjects->enemyBlack);
    sfSprite_destroy(gameObjects->enemyRed);
    sfSprite_destroy(gameObjects->enemyBlue);
    sfSprite_destroy(gameObjects->enemyGreen);
    sfRectangleShape_destroy(gameObjects->fillLifeBar);
    sfRectangleShape_destroy(gameObjects->fillLifeBar2);
    sfRectangleShape_destroy(gameObjects->base);
    sfRenderWindow_destroy(window);
}
