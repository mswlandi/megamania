// Constants
#define WIDTH 800
#define HEIGHT 600
#define ENERGYMAX 436
#define ENERGYY 35
#define BARSPEED 50
#define MAXENEMIES 20

// SFML headers
#include <SFML/Graphics.h>

// Default headers
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/// Structs

// Struct for enemy
typedef struct str_enemy
{
    sfSprite* enemySprite;
    int posX;
    int posY;
    int color;
} type_enemies;

// Struct for player ship
typedef struct str_playership
{
    sfSprite* shipSprite;
    int posX;
    int posY;
} type_playerShip;

// Struct that holds all of the game's sprites
typedef struct str_sprites
{
    type_playerShip ship;
    sfSprite* fire;
    sfSprite* background;
    sfSprite* life;
    sfSprite* lifebar;
    type_enemies enemies[MAXENEMIES];
    sfRectangleShape* fillLifeBar;
    sfRectangleShape* fillLifeBar2;
    sfRectangleShape* base;
} type_allSprites;

// Struct which holds all about the logic of level
typedef struct str_level
{
    int speedOfLevel;
    char direction; // Can be 'R', to right, 'L', to left, or 'B', to both synchronously.
    int numberEnemies;
}levelType;

/// Defining some useful functions

// This bundles together a sprite with a texture, and the origin in the middle
sfSprite* sfSprite_createFromFile(const char* filename, sfVector2f scale, sfVector2f pos);
// This draws the lifes on the screen (uses sfSprite_createFromFile)
void drawLifes(sfRenderWindow* window, sfSprite* life, int* numL);
// This creates the enemies, one by one.
void createEnemies(void);
// This draws the enemies, one by one.
void drawEnemies(sfRenderWindow* window);
// This loads all the stage (player ship, enemies, background, logic, ..., using a lot of others functions)
void layoutStage(sfRenderWindow* window);
// This loads Game Over screen
void layoutGameOver(sfRenderWindow* window, sfEvent event);
// This load the sprites of the game. Can be "certinho" or "zuadasso".
void loadGameSprites(const char* src);


/// Variables

// General variables
float energy = ENERGYMAX;
int numberlifes = 3;

// Mouse variables
float mouseX;
float mouseY;

// Sprites
type_allSprites gameSprites;


int main()
{
    // Video variables
    sfVideoMode mode = {WIDTH, HEIGHT, 32};
    sfRenderWindow* window;

    // Create the main window
    window = sfRenderWindow_create(mode, "Megamania", sfResize | sfClose, NULL);

    loadGameSprites("certinho");
    layoutStage(window);

    // Cleanup resources
    sfSprite_destroy(gameSprites.ship.shipSprite);
    sfSprite_destroy(gameSprites.fire);
    sfRenderWindow_destroy(window);


    return 0;
}

void layoutStage(sfRenderWindow* window)
{
    /// Initializing stuff when the layout opens
    // Time variables
    sfClock* clock = sfClock_create();
    sfTime time = sfClock_getElapsedTime(clock);
    sfTime lasttime = time;
    float dtime = 0;

    sfEvent event;

    // Flags
    int isFireable = 1; //Fire flag


    /// Initializing positions
    sfSprite_setPosition(gameSprites.ship.shipSprite, (sfVector2f){WIDTH/2, 450});

    /// Loop of the layout
    while(sfRenderWindow_isOpen(window))
    {
        /// Code to close the window
        while(sfRenderWindow_pollEvent(window, &event))
        {
            // Close window : exit
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        /// Logic of the layout
        // When the energy ends
        if(energy <= 0)
        {
            numberlifes--;
            if(numberlifes == 0)
            {
                layoutGameOver(window, event);

                // If you don't restart the time, it keeps running and bugs the energy
                sfClock_restart(clock);
                time = sfClock_getElapsedTime(clock);
                lasttime = time;
                dtime = sfTime_asSeconds(time) - sfTime_asSeconds(lasttime);
            }

            energy = ENERGYMAX;
            sfSprite_setPosition(gameSprites.ship.shipSprite, (sfVector2f){WIDTH/2, 450});
        }

        // Ship - Player movement
        if((sfKeyboard_isKeyPressed(sfKeyLeft)||sfKeyboard_isKeyPressed(sfKeyA)) && sfSprite_getPosition(gameSprites.ship.shipSprite).x > 40)
            sfSprite_move(gameSprites.ship.shipSprite, (sfVector2f){-300*dtime, 0});
        if((sfKeyboard_isKeyPressed(sfKeyRight)||sfKeyboard_isKeyPressed(sfKeyD)) && sfSprite_getPosition(gameSprites.ship.shipSprite).x < 760)
            sfSprite_move(gameSprites.ship.shipSprite, (sfVector2f){300*dtime, 0});


        // Fire - moves the blast
        if(sfKeyboard_isKeyPressed(sfKeySpace) && isFireable)
        {
            sfSprite_setPosition(gameSprites.fire, (sfVector2f){sfSprite_getPosition(gameSprites.ship.shipSprite).x, sfSprite_getPosition(gameSprites.ship.shipSprite).y - 40});
        }
        if(sfSprite_getPosition(gameSprites.fire).y >= -40)
        {
            sfSprite_setPosition(gameSprites.fire, (sfVector2f){sfSprite_getPosition(gameSprites.ship.shipSprite).x, sfSprite_getPosition(gameSprites.fire).y -600*dtime});
            isFireable = 0;
        }
        else
            isFireable = 1;

        energy -= BARSPEED*dtime; // To empty the life bar
        sfRectangleShape_setSize(gameSprites.fillLifeBar2, (sfVector2f){energy, ENERGYY});

        /// Actual drawing

        sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
        sfRenderWindow_drawSprite(window, gameSprites.background, NULL);
        sfRenderWindow_drawSprite(window, gameSprites.ship.shipSprite, NULL);
        sfRenderWindow_drawSprite(window, gameSprites.fire, NULL);
        sfRenderWindow_drawRectangleShape(window, gameSprites.base, NULL);
        drawLifes(window, gameSprites.life, &numberlifes);
        sfRenderWindow_drawSprite(window, gameSprites.lifebar, NULL);
        sfRenderWindow_drawRectangleShape(window, gameSprites.fillLifeBar, NULL);
        sfRenderWindow_drawRectangleShape(window, gameSprites.fillLifeBar2, NULL);
        drawEnemies(window);

        sfRenderWindow_display(window);

        // Calculate dtime
        time = sfClock_getElapsedTime(clock);
        dtime = sfTime_asSeconds(time) - sfTime_asSeconds(lasttime);
        lasttime = time;
    }
}

void layoutGameOver(sfRenderWindow* window, sfEvent event)
{
    sfSprite* gameover = sfSprite_createFromFile("gameover.png",
                                                 (sfVector2f){1,1},
                                                 (sfVector2f){WIDTH/2, HEIGHT/2});

    do
    {
        /// Code to close the window
        while(sfRenderWindow_pollEvent(window, &event))
        {
            // Close window : exit
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        sfSleep(sfMilliseconds(10));
        sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
        sfRenderWindow_drawSprite(window, gameover, NULL);
        sfRenderWindow_display(window);
    } while(!(sfMouse_isButtonPressed(sfMouseLeft) && sfRenderWindow_hasFocus(window) && sfMouse_getPosition(window).y > 0));  //Mouse position Y is used to drag the window when the gameover screen is on.

    numberlifes = 3;
}

void loadGameSprites(const char* src)
{
    if(src == "certinho")
    {
        // Ship
        gameSprites.ship.shipSprite = sfSprite_createFromFile("nave.png",
                                               (sfVector2f){0.8,0.8},
                                               (sfVector2f){WIDTH/2, 450});

        // Enemies
        createEnemies();

        // Fire
        gameSprites.fire = sfSprite_createFromFile("fire.png",
                                               (sfVector2f){0.5,0.5},
                                               (sfVector2f){-40, -40});

        // Background
        gameSprites.background = sfSprite_createFromFile("background.png",
                                                     (sfVector2f){3.125, 2.3475},
                                                     (sfVector2f){WIDTH/2, HEIGHT/2});

        // Lifes
        gameSprites.life = sfSprite_createFromFile("life.png",
                                               (sfVector2f){1,1},
                                               (sfVector2f){350,575});

        // Life bar
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

        // GUI
        gameSprites.base = sfRectangleShape_create();
        sfRectangleShape_setSize(gameSprites.base, (sfVector2f){WIDTH, 100});
        sfRectangleShape_setPosition(gameSprites.base, (sfVector2f){0, 500});
        sfRectangleShape_setFillColor(gameSprites.base, sfColor_fromRGB(150,0,0));
    }
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

void createEnemies(void)
{
    int i; // Count

    srand(time(NULL));


    for(i=0; i < MAXENEMIES; i++)
    {
        gameSprites.enemies[i].color = rand()%4;

        switch(gameSprites.enemies[i].color)
        {
                case 0: gameSprites.enemies[i].enemySprite = sfSprite_createFromFile("enemyBlack.png", (sfVector2f){ 1, 1}, (sfVector2f){gameSprites.enemies[i].posX, gameSprites.enemies[i].posY});
                        break;
                case 1: gameSprites.enemies[i].enemySprite = sfSprite_createFromFile("enemyBlue.png", (sfVector2f){ 1, 1}, (sfVector2f){gameSprites.enemies[i].posX, gameSprites.enemies[i].posY});
                        break;
                case 2: gameSprites.enemies[i].enemySprite = sfSprite_createFromFile("enemyRed.png", (sfVector2f){ 1, 1}, (sfVector2f){gameSprites.enemies[i].posX, gameSprites.enemies[i].posY});
                        break;
                case 3: gameSprites.enemies[i].enemySprite = sfSprite_createFromFile("enemyGreen.png", (sfVector2f){ 1, 1}, (sfVector2f){gameSprites.enemies[i].posX, gameSprites.enemies[i].posY});
                        break;

        }
    }
}

void drawEnemies(sfRenderWindow* window)
{
    int i;

    for(i=0; i < MAXENEMIES; i++)
    {
        sfRenderWindow_drawSprite(window, gameSprites.enemies[i].enemySprite, NULL);
    }
}
