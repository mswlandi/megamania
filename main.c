#define WIDTH 800
#define HEIGHT 600
#define ENERGYMAX 436
#define ENERGYY 35
#define BARSPEED 50

#include <SFML/Graphics.h>
#include <string.h>
#include <stdio.h>

//Defining some useful functions
//This bundles together a sprite with a texture, and the origin in the middle
sfSprite* sfSprite_createFromFile(const char* filename, sfVector2f scale, sfVector2f pos);
void drawLifes(sfRenderWindow* window, sfSprite* life, int* numL);

void layoutStage1(sfRenderWindow* window);
void layoutGameOver(sfRenderWindow* window);
void loadGameSprites(const char* src);

//Struct that holds all of the game's sprites
struct
{
    sfSprite* ship;
    sfSprite* fire;
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

//Mouse variables
float mouseX;
float mouseY;

int main()
{
    //Video variables
    sfVideoMode mode = {WIDTH, HEIGHT, 32};
    sfRenderWindow* window;

    //Create the main window
    window = sfRenderWindow_create(mode, "Megamania", sfResize | sfClose, NULL);

    loadGameSprites("certinho");
    layoutStage(window);

    //Cleanup resources
    sfSprite_destroy(gameSprites.ship);
    sfSprite_destroy(gameSprites.fire);
    sfRenderWindow_destroy(window);

    return 0;
}

void layoutStage(sfRenderWindow* window)
{
    ///Initializing stuff when the layout opens
    //Time variables
    sfClock* clock = sfClock_create();
    sfTime time = sfClock_getElapsedTime(clock);
    sfTime lasttime = time;
    float dtime = 0;

    sfEvent event;

    //Flags
    int isFireable = 1; //Fire flag

    sfSprite_setPosition(gameSprites.ship, (sfVector2f){WIDTH/2, 450});

    ///Loop of the layout
    while (sfRenderWindow_isOpen(window))
    {
        //Code to close the window
        while (sfRenderWindow_pollEvent(window, &event))
        {
            //Close window : exit
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        ///Logic of the layout
        //When the energy ends
        if(energy <= 0)
        {
            numberlifes--;
            if(numberlifes == 0)
            {
                layoutGameOver(window);

                //If you don't restart the time, it keeps running and bugs the energy
                sfClock_restart(clock);
                time = sfClock_getElapsedTime(clock);
                lasttime = time;
                dtime = sfTime_asSeconds(time) - sfTime_asSeconds(lasttime);
            }
            energy = ENERGYMAX;
            sfSprite_setPosition(gameSprites.ship, (sfVector2f){WIDTH/2, 450});
        }

        //Ship - Player movement
        if((sfKeyboard_isKeyPressed(sfKeyLeft)||sfKeyboard_isKeyPressed(sfKeyA)) && sfSprite_getPosition(gameSprites.ship).x > 40)
            sfSprite_move(gameSprites.ship, (sfVector2f){-300*dtime, 0});
        if((sfKeyboard_isKeyPressed(sfKeyRight)||sfKeyboard_isKeyPressed(sfKeyD)) && sfSprite_getPosition(gameSprites.ship).x < 760)
            sfSprite_move(gameSprites.ship, (sfVector2f){300*dtime, 0});



        //Fire - moves the blast
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
        sfRenderWindow_display(window);

        //Calculate dtime
        time = sfClock_getElapsedTime(clock);
        dtime = sfTime_asSeconds(time) - sfTime_asSeconds(lasttime);
        lasttime = time;
    }
}

void layoutGameOver(sfRenderWindow* window)
{
    sfSprite* gameover = sfSprite_createFromFile("gameover.png",
                                                 (sfVector2f){1,1},
                                                 (sfVector2f){WIDTH/2, HEIGHT/2});
    sfEvent event;

    do
    {
        sfSleep(sfMilliseconds(10));
        sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
        sfRenderWindow_drawSprite(window, gameover, NULL);
        sfRenderWindow_display(window);
    } while(!(sfMouse_isButtonPressed(sfMouseLeft) && sfWindow_hasFocus(window)) && numberlifes == 0);

    numberlifes = 3;
}

void loadGameSprites(const char* src)
{
    //Ship
    gameSprites.ship = sfSprite_createFromFile("nave.png",
                                               (sfVector2f){0.8,0.8},
                                               (sfVector2f){WIDTH/2, 450});

    //Fire
    gameSprites.fire = sfSprite_createFromFile("fire.png",
                                               (sfVector2f){0.5,0.5},
                                               (sfVector2f){-40, -40});

    //Background
    gameSprites.background = sfSprite_createFromFile("background.png",
                                                     (sfVector2f){3.125, 2.3475},
                                                     (sfVector2f){WIDTH/2, HEIGHT/2});

    //Lifes
    gameSprites.life = sfSprite_createFromFile("life.png",
                                               (sfVector2f){1,1},
                                               (sfVector2f){350,575});

    //Life bar
    gameSprites.lifebar = sfSprite_createFromFile("lifebar.png",
                                                  (sfVector2f){2, 1},
                                                  (sfVector2f){WIDTH/2, 525});

    gameSprites.fillLifeBar = sfRectangleShape_create();
    sfRectangleShape_setSize(gameSprites.fillLifeBar, (sfVector2f){436, 35});
    sfRectangleShape_setPosition(gameSprites.fillLifeBar, (sfVector2f){182, 507.5});
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

sfSprite* sfSprite_createFromFile(const char* filename, sfVector2f scale, sfVector2f pos)
{
    sfTexture* texture;
    sfSprite* sprite;

    char img[6]="imgs/";

    texture = sfTexture_createFromFile(strcat(img,filename), NULL);
    sprite = sfSprite_create();

    sfSprite_setTexture(sprite, texture, sfTrue);
    sfSprite_setOrigin(sprite, (sfVector2f){sfSprite_getLocalBounds(sprite).width/2,
                                            sfSprite_getLocalBounds(sprite).height/2});
    sfSprite_scale(sprite, scale);
    sfSprite_setPosition(sprite, pos);

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
