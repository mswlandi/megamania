#define WIDTH 800
#define HEIGHT 600
#define ENERGYMAX 436
#define ENERGYY 35
#define BARSPEED 100

#include <SFML/Graphics.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct structForEnemies enemiesSprites;

//Defining some useful functions
//This bundles together a sprite with a texture, and the origin in the middle
sfSprite* sfSprite_createFromFile(const char* filename);
void drawLifes(sfRenderWindow* window, sfSprite* life, int* numL);
void createEnemies();
void drawEnemies(sfRenderWindow* window);
void layoutStage1(sfRenderWindow* window);
void layoutGameOver(sfRenderWindow* window);
void loadGameSprites(const char* src);

///Multi-Layout Variables

//Struct that holds all of the enemies's sprites, including the color of them
struct structForEnemies
{
    sfSprite* enemy1;
    sfSprite* enemy2;
    sfSprite* enemy3;
    sfSprite* enemy4;
    sfSprite* enemy5;
    int cor;
};
//Struct that holds all of the game's sprites
struct
{
    sfSprite* ship;
    sfSprite* fire;
    struct structForEnemies;
    sfSprite* background;
    sfSprite* life;
    sfSprite* lifebar;
    sfRectangleShape* fillLifeBar;
    sfRectangleShape* fillLifeBar2;
    sfRectangleShape* base;
} gameSprites;

//General variables
float energy = ENERGYMAX;
int numberlifes = 3;

//Variable for enemies sprites
enemiesSprites enemies;

//Mouse variables
float mouseX;
float mouseY;

int main()
{
    //Video variables
    sfVideoMode mode = {WIDTH, HEIGHT, 32};
    sfRenderWindow* window;

    enemies.cor = 7;

    //Create the main window
    window = sfRenderWindow_create(mode, "Megamania", sfResize | sfClose, NULL);

    loadGameSprites("certinho");
    layoutStage1(window);

    //Cleanup resources
    sfSprite_destroy(gameSprites.ship);
    sfSprite_destroy(gameSprites.fire);
    sfRenderWindow_destroy(window);


    return 0;
}

void layoutStage1(sfRenderWindow* window)
{
    //Time variables
    sfClock* clock = sfClock_create();
    sfTime time = sfClock_getElapsedTime(clock);
    sfTime lasttime = time;
    float dtime = 0;

    sfEvent event;

    //Flags
    int isFireable = 1; //Fire flag

    ///Initializing positions
    sfSprite_setPosition(gameSprites.ship, (sfVector2f){WIDTH/2, 450});
    createEnemies();
    while (sfRenderWindow_isOpen(window))
    {
        ///Process events
        while (sfRenderWindow_pollEvent(window, &event))
        {
            //Close window : exit
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        //When the energy ends
        if(energy <= 0)
        {
            numberlifes--;
            if(numberlifes == 0)
                layoutGameOver(window);
            energy = ENERGYMAX;
            sfSprite_setPosition(gameSprites.ship, (sfVector2f){WIDTH/2, 450});
        }

        //Ship
        if((sfKeyboard_isKeyPressed(sfKeyLeft)||sfKeyboard_isKeyPressed(sfKeyA)) && sfSprite_getPosition(gameSprites.ship).x > 40)
            sfSprite_move(gameSprites.ship, (sfVector2f){-300*dtime, 0});
        if((sfKeyboard_isKeyPressed(sfKeyRight)||sfKeyboard_isKeyPressed(sfKeyD)) && sfSprite_getPosition(gameSprites.ship).x < 760)
            sfSprite_move(gameSprites.ship, (sfVector2f){300*dtime, 0});


        //Fire
        if(sfKeyboard_isKeyPressed(sfKeySpace) && isFireable)
        {
            sfSprite_setPosition(gameSprites.fire, (sfVector2f){sfSprite_getPosition(gameSprites.ship).x, sfSprite_getPosition(gameSprites.ship).y - 40});
        }
        if(sfSprite_getPosition(gameSprites.fire).y >= -40)
        {
            sfSprite_setPosition(gameSprites.fire, (sfVector2f){sfSprite_getPosition(gameSprites.ship).x, sfSprite_getPosition(gameSprites.fire).y -600*dtime});
            isFireable = 0;
        }
        else
            isFireable = 1;

        energy -= BARSPEED*dtime; // To empty the life bar
        sfRectangleShape_setSize(gameSprites.fillLifeBar2, (sfVector2f){energy, ENERGYY});

        ///Actual drawing

        sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
        sfRenderWindow_drawSprite(window, gameSprites.background, NULL);
        sfRenderWindow_drawSprite(window, gameSprites.ship, NULL);
        sfRenderWindow_drawSprite(window, gameSprites.fire, NULL);
        sfRenderWindow_drawRectangleShape(window, gameSprites.base, NULL);
        drawLifes(window, gameSprites.life, &numberlifes);
        sfRenderWindow_drawSprite(window, gameSprites.lifebar, NULL);
        sfRenderWindow_drawRectangleShape(window, gameSprites.fillLifeBar, NULL);
        sfRenderWindow_drawRectangleShape(window, gameSprites.fillLifeBar2, NULL);
        drawEnemies(window);

        sfRenderWindow_display(window);

        //Calculate dtime
        time = sfClock_getElapsedTime(clock);
        dtime = sfTime_asSeconds(time) - sfTime_asSeconds(lasttime);
        lasttime = time;
    }
}

void layoutGameOver(sfRenderWindow* window)
{
    sfSprite* gameover = sfSprite_createFromFile("gameover.png");
    sfSprite_setOrigin(gameover, (sfVector2f){300, 113});
    sfSprite_setPosition(gameover, (sfVector2f){WIDTH/2, HEIGHT/2});

    do
    {
        sfSleep(sfMilliseconds(10));
        sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
        sfRenderWindow_drawSprite(window, gameover, NULL);
        sfRenderWindow_display(window);
    } while(!(sfMouse_isButtonPressed(sfMouseLeft) && sfRenderWindow_hasFocus(window)) && numberlifes == 0);

    numberlifes = 3;
}

void loadGameSprites(const char* src)
{
    //Ship
    gameSprites.ship = sfSprite_createFromFile("nave.png");
    sfSprite_scale(gameSprites.ship, (sfVector2f){0.8,0.8}); //nave.png width: 80px
    sfSprite_setPosition(gameSprites.ship, (sfVector2f){WIDTH/2, 450});

    //Fire
    gameSprites.fire = sfSprite_createFromFile("fire.png");
    sfSprite_scale(gameSprites.fire, (sfVector2f){0.5,0.5});
    sfSprite_setPosition(gameSprites.fire, (sfVector2f){-40, -40});

    //Background
    gameSprites.background = sfSprite_createFromFile("background.png");
    sfSprite_scale(gameSprites.background, (sfVector2f){3.125, 2.3475});
    sfSprite_setPosition(gameSprites.background, (sfVector2f){WIDTH/2, HEIGHT/2});

    //Lifes
    gameSprites.life = sfSprite_createFromFile("life.png");

    //Life bar
    gameSprites.lifebar = sfSprite_createFromFile("lifebar.png");
    sfSprite_setOrigin(gameSprites.lifebar, (sfVector2f){111, 19.5});
    sfSprite_scale(gameSprites.lifebar, (sfVector2f){2, 1});
    sfSprite_setPosition(gameSprites.lifebar, (sfVector2f){WIDTH/2, 525});

    gameSprites.fillLifeBar = sfRectangleShape_create();
    sfRectangleShape_setSize(gameSprites.fillLifeBar, (sfVector2f){436, 35});
    sfRectangleShape_setOrigin(gameSprites.fillLifeBar, (sfVector2f){218, 17.5});
    sfRectangleShape_setPosition(gameSprites.fillLifeBar, (sfVector2f){WIDTH/2, 525});
    sfRectangleShape_setFillColor(gameSprites.fillLifeBar, sfColor_fromRGB(255,255,255));

    gameSprites.fillLifeBar2 = sfRectangleShape_create();
    sfRectangleShape_setSize(gameSprites.fillLifeBar2, (sfVector2f){energy, ENERGYY});
    sfRectangleShape_setPosition(gameSprites.fillLifeBar2, (sfVector2f){182, 507.5});
    sfRectangleShape_setFillColor(gameSprites.fillLifeBar2, sfColor_fromRGB(100,0,0));

    //GUI
    gameSprites.base = sfRectangleShape_create();
    sfRectangleShape_setSize(gameSprites.base, (sfVector2f){WIDTH, 100});
    sfRectangleShape_setPosition(gameSprites.base, (sfVector2f){0, 500});
    sfRectangleShape_setFillColor(gameSprites.base, sfColor_fromRGB(150,0,0));
}

sfSprite* sfSprite_createFromFile(const char* filename)
{
    sfTexture* texture;
    sfSprite* sprite;

    char img[6]="imgs/";

    texture = sfTexture_createFromFile(strcat(img,filename), NULL);
    sprite = sfSprite_create();

    sfSprite_setTexture(sprite, texture, sfTrue);
    sfSprite_setOrigin(sprite, (sfVector2f){sfSprite_getLocalBounds(sprite).width/2,
                                            sfSprite_getLocalBounds(sprite).height/2});

    return sprite;
}

void drawLifes(sfRenderWindow* window, sfSprite* life, int* numL)
{
    int i;

    for(i=0; i < *numL; i++)
    {
        sfSprite_setPosition(life,(sfVector2f){350 + 50*i, 575});
        sfRenderWindow_drawSprite(window, gameSprites.life, NULL);
    }
}

void createEnemies()
{
    switch(enemies.cor)
    {
            case 0: enemies.enemy1 = sfSprite_createFromFile("enemyBlack1.png");
                    enemies.enemy2 = sfSprite_createFromFile("enemyBlack2.png");
                    enemies.enemy3 = sfSprite_createFromFile("enemyBlack3.png");
                    enemies.enemy4 = sfSprite_createFromFile("enemyBlack4.png");
                    enemies.enemy5 = sfSprite_createFromFile("enemyBlack5.png");
                    break;
            case 1: enemies.enemy1 = sfSprite_createFromFile("enemyBlue1.png");
                    enemies.enemy2 = sfSprite_createFromFile("enemyBlue2.png");
                    enemies.enemy3 = sfSprite_createFromFile("enemyBlue3.png");
                    enemies.enemy4 = sfSprite_createFromFile("enemyBlue4.png");
                    enemies.enemy5 = sfSprite_createFromFile("enemyBlue5.png");
                    break;
            case 2: enemies.enemy1 = sfSprite_createFromFile("enemyGreen1.png");
                    enemies.enemy2 = sfSprite_createFromFile("enemyGreen2.png");
                    enemies.enemy3 = sfSprite_createFromFile("enemyGreen3.png");
                    enemies.enemy4 = sfSprite_createFromFile("enemyGreen4.png");
                    enemies.enemy5 = sfSprite_createFromFile("enemyGreen5.png");
                    break;
            case 3: enemies.enemy1 = sfSprite_createFromFile("enemyRed1.png");
                    enemies.enemy2 = sfSprite_createFromFile("enemyRed2.png");
                    enemies.enemy3 = sfSprite_createFromFile("enemyRed3.png");
                    enemies.enemy4 = sfSprite_createFromFile("enemyRed4.png");
                    enemies.enemy5 = sfSprite_createFromFile("enemyRed5.png");
                    break;
            default:
                    srand(time(NULL));
                    enemies.cor = rand()%4;
                    createEnemies();
                    break;
    }
}

void drawEnemies(sfRenderWindow* window)
{
    sfSprite_setPosition(enemies.enemy1, (sfVector2f){160, 200});
    sfSprite_setPosition(enemies.enemy2, (sfVector2f){2*160, 200});
    sfSprite_setPosition(enemies.enemy3, (sfVector2f){3*160, 200});
    sfSprite_setPosition(enemies.enemy4, (sfVector2f){4*160, 200});
    sfSprite_setPosition(enemies.enemy5, (sfVector2f){5*160, 200});

    sfRenderWindow_drawSprite(window, enemies.enemy1, NULL);
    sfRenderWindow_drawSprite(window, enemies.enemy2, NULL);
    sfRenderWindow_drawSprite(window, enemies.enemy3, NULL);
    sfRenderWindow_drawSprite(window, enemies.enemy4, NULL);
    sfRenderWindow_drawSprite(window, enemies.enemy5, NULL);
}
