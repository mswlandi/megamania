#define WIDTH 800
#define HEIGHT 600
#define ENERGYMAX 436
#define ENERGYY 35
#include <SFML/Graphics.h>
#include <string.h>
#include <stdio.h>

//Defining some useful functions
//This bundles together a sprite with a texture, and the origin in the middle
sfSprite* sfSprite_createFromFile(const char* filename);
void drawLifes(sfWindow* window, sfSprite* life1, sfSprite* life2, sfSprite* life3, int* numL);

void layoutStage1(sfRenderWindow* window);
void layoutGameOver(sfRenderWindow* window);

///Multi-Layout Variables

//Struct that holds all of the game's spr
sfSprite* ship;
sfSprite* fire;
sfSprite* background;
sfSprite* life1;
sfSprite* life2;
sfSprite* life3;
sfSprite* lifebar;
sfRectangleShape* fillLifeBar;
sfRectangleShape* fillLifeBar2;
sfRectangleShape* base;

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

    ///Loads the sprites
    //Ship
    ship = sfSprite_createFromFile("nave.png");
    sfSprite_scale(ship, (sfVector2f){0.8,0.8}); //nave.png width: 80px
    sfSprite_setPosition(ship, (sfVector2f){WIDTH/2, 450});

    //Fire
    fire = sfSprite_createFromFile("fire.png");
    sfSprite_scale(fire, (sfVector2f){0.5,0.5});
    sfSprite_setPosition(fire, (sfVector2f){-40, -40});

    //Background
    background = sfSprite_createFromFile("background.png");
    sfSprite_scale(background, (sfVector2f){3.125, 2.3475});
    sfSprite_setPosition(background, (sfVector2f){WIDTH/2, HEIGHT/2});

    //Lifes
    life1 = sfSprite_createFromFile("life.png");
    sfSprite_setPosition(life1, (sfVector2f){350, 575});
    life2 = sfSprite_createFromFile("life.png");
    sfSprite_setPosition(life2, (sfVector2f){400, 575});
    life3 = sfSprite_createFromFile("life.png");
    sfSprite_setPosition(life3, (sfVector2f){450, 575});

    //Life bar
    lifebar = sfSprite_createFromFile("lifebar.png");
    sfSprite_setOrigin(lifebar, (sfVector2f){111, 19.5});
    sfSprite_scale(lifebar, (sfVector2f){2, 1});
    sfSprite_setPosition(lifebar, (sfVector2f){WIDTH/2, 525});

    fillLifeBar = sfRectangleShape_create();
    sfRectangleShape_setSize(fillLifeBar, (sfVector2f){436, 35});
    sfRectangleShape_setOrigin(fillLifeBar, (sfVector2f){218, 17.5});
    sfRectangleShape_setPosition(fillLifeBar, (sfVector2f){WIDTH/2, 525});
    sfRectangleShape_setFillColor(fillLifeBar, sfColor_fromRGB(255,255,255));

    fillLifeBar2 = sfRectangleShape_create();
    sfRectangleShape_setSize(fillLifeBar2, (sfVector2f){energy, ENERGYY});
    sfRectangleShape_setPosition(fillLifeBar2, (sfVector2f){182, 507.5});
    sfRectangleShape_setFillColor(fillLifeBar2, sfColor_fromRGB(100,0,0));

    //GUI
    base = sfRectangleShape_create();
    sfRectangleShape_setSize(base, (sfVector2f){WIDTH, 100});
    sfRectangleShape_setPosition(base, (sfVector2f){0, 500});
    sfRectangleShape_setFillColor(base, sfColor_fromRGB(150,0,0));

    layoutStage1(window);

    //Cleanup resources
    sfSprite_destroy(ship);
    sfSprite_destroy(fire);
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
    sfSprite_setPosition(ship, (sfVector2f){WIDTH/2, 450});

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
            sfSprite_setPosition(ship, (sfVector2f){WIDTH/2, 450});
        }

        //Ship
        if((sfKeyboard_isKeyPressed(sfKeyLeft)||sfKeyboard_isKeyPressed(sfKeyA)) && sfSprite_getPosition(ship).x > 40)
            sfSprite_move(ship, (sfVector2f){-300*dtime, 0});
        if((sfKeyboard_isKeyPressed(sfKeyRight)||sfKeyboard_isKeyPressed(sfKeyD)) && sfSprite_getPosition(ship).x < 760)
            sfSprite_move(ship, (sfVector2f){300*dtime, 0});


        //Fire
        if(sfKeyboard_isKeyPressed(sfKeySpace) && isFireable)
        {
            sfSprite_setPosition(fire, (sfVector2f){sfSprite_getPosition(ship).x, sfSprite_getPosition(ship).y - 40});
        }
        if(sfSprite_getPosition(fire).y >= -40)
        {
            sfSprite_setPosition(fire, (sfVector2f){sfSprite_getPosition(ship).x, sfSprite_getPosition(fire).y -600*dtime});
            isFireable = 0;
        }
        else
            isFireable = 1;

        energy -= 100*dtime; // To empty the life bar
        sfRectangleShape_setSize(fillLifeBar2, (sfVector2f){energy, ENERGYY});

        ///Actual drawing

        sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
        sfRenderWindow_drawSprite(window, background, NULL);
        sfRenderWindow_drawSprite(window, ship, NULL);
        sfRenderWindow_drawSprite(window, fire, NULL);
        sfRenderWindow_drawRectangleShape(window, base, NULL);
        drawLifes(window, life1, life2, life3, &numberlifes);
        sfRenderWindow_drawSprite(window, lifebar, NULL);
        sfRenderWindow_drawRectangleShape(window, fillLifeBar, NULL);
        sfRenderWindow_drawRectangleShape(window, fillLifeBar2, NULL);
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
        sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
        sfRenderWindow_drawSprite(window, gameover, NULL);
        sfRenderWindow_display(window);
    } while(!(sfMouse_isButtonPressed(sfMouseLeft)) && numberlifes == 0);

    numberlifes = 3;
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

void drawLifes(sfWindow* window, sfSprite* life1, sfSprite* life2, sfSprite* life3, int* numL)
{
    if(*numL == 3)
    {
            sfRenderWindow_drawSprite(window, life1, NULL);
            sfRenderWindow_drawSprite(window, life2, NULL);
            sfRenderWindow_drawSprite(window, life3, NULL);
    }
    if(*numL == 2)
    {
            sfRenderWindow_drawSprite(window, life1, NULL);
            sfRenderWindow_drawSprite(window, life2, NULL);
    }
    if(*numL == 1)
    {
            sfRenderWindow_drawSprite(window, life1, NULL);
    }
}
