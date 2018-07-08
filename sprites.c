// Video's constants
#define WIDTH 800
#define HEIGHT 600
// Energy's constants
#define ENERGYMAX 436
#define ENERGYY 35

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

void loadGameSprites(TYPE_ALLSPRITES* gameSprites)
{
    /// Initializing background
    gameSprites->menuBackground = sfSprite_createFromFile("background.png",
                                                        (sfVector2f){3.125, 2.3475},
                                                        (sfVector2f){WIDTH/2, HEIGHT/2});

    gameSprites->ship.shipSprite = sfSprite_createFromFile("nave.png",
                                                         (sfVector2f){0.8,0.8},
                                                         (sfVector2f){WIDTH/2, 450});

    // Fire
    gameSprites->fire = sfSprite_createFromFile("fire.png",
                                            (sfVector2f){0.5,0.5},
                                            (sfVector2f){-40, -40});

    // Enemy fire
    gameSprites->enemyFire = sfSprite_createFromFile("fire.png",
                                            (sfVector2f){0.5,0.5},
                                            (sfVector2f){-40, -40});
    sfSprite_setColor(gameSprites->enemyFire, sfColor_fromRGB(255, 100, 255));

    // Background
    gameSprites->background = sfSprite_createFromFile("background.png",
                                                  (sfVector2f){3.125, 2.3475},
                                                  (sfVector2f){WIDTH/2, HEIGHT/2});

    // Lifes
    gameSprites->life = sfSprite_createFromFile("life.png",
                                            (sfVector2f){1,1},
                                            (sfVector2f){350,575});

    // Life bar
    gameSprites->lifebar = sfSprite_createFromFile("lifebar.png",
                                               (sfVector2f){2, 1},
                                               (sfVector2f){WIDTH/2, 525});

    gameSprites->gameover = sfSprite_createFromFile("gameover.png",
                                                  (sfVector2f){1,1},
                                                  (sfVector2f){WIDTH/2, HEIGHT/2});

    gameSprites->enemyBlack = sfSprite_createFromFile("enemyBlack.png", (sfVector2f){ 0.7, 0.7}, (sfVector2f){-100, -100});
    gameSprites->enemyRed = sfSprite_createFromFile("enemyBlue.png", (sfVector2f){ 0.7, 0.7}, (sfVector2f){-100, -100});
    gameSprites->enemyGreen = sfSprite_createFromFile("enemyRed.png", (sfVector2f){ 0.7, 0.7}, (sfVector2f){-100, -100});
    gameSprites->enemyBlue = sfSprite_createFromFile("enemyGreen.png", (sfVector2f){ 0.7, 0.7}, (sfVector2f){-100, -100});

    gameSprites->fillLifeBar = sfRectangleShape_create();
    sfRectangleShape_setSize(gameSprites->fillLifeBar, (sfVector2f){436, 35});
    sfRectangleShape_setPosition(gameSprites->fillLifeBar, (sfVector2f){182, 507.5});
    sfRectangleShape_setFillColor(gameSprites->fillLifeBar, sfColor_fromRGB(255,255,255));

    gameSprites->fillLifeBar2 = sfRectangleShape_create();
    sfRectangleShape_setSize(gameSprites->fillLifeBar2, (sfVector2f){ENERGYMAX, ENERGYY});
    sfRectangleShape_setPosition(gameSprites->fillLifeBar2, (sfVector2f){182, 507.5});
    sfRectangleShape_setFillColor(gameSprites->fillLifeBar2, sfColor_fromRGB(100,0,0));

    // GUI
    gameSprites->base = sfRectangleShape_create();
    sfRectangleShape_setSize(gameSprites->base, (sfVector2f){WIDTH, 100});
    sfRectangleShape_setPosition(gameSprites->base, (sfVector2f){0, 500});
    sfRectangleShape_setFillColor(gameSprites->base, sfColor_fromRGB(150,0,0));
}
