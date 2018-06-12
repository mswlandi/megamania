// Constants
#define WIDTH 800
#define HEIGHT 600
#define ENERGYMAX 436
#define ENERGYY 35
#define BARSPEED 10.9
#define MAXENEMIES 20
#define DIST_ENEMY_X 20
#define DIST_ENEMY_Y 50

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
    float posX;
    float posY;
    int color;
    int flag; // If this enemy is alive, it's 1,
} TYPE_ENEMIES;

// Struct for player ship
typedef struct str_playership
{
    sfSprite* shipSprite;
    float posX;
    float posY;
} TYPE_PLAYERSHIP;

// Struct that holds all of the game's sprites
typedef struct str_sprites
{
    TYPE_PLAYERSHIP ship;
    sfSprite* fire;
    sfSprite* background;
    sfSprite* life;
    sfSprite* lifebar;
    TYPE_ENEMIES enemies[MAXENEMIES];
    sfRectangleShape* fillLifeBar;
    sfRectangleShape* fillLifeBar2;
    sfRectangleShape* base;
} TYPE_ALLSPRITES;

// Struct which holds all about the logic of level
typedef struct str_level
{
    char* mapName;
    int levelSpeed;
    char direction; // Can be 'R', to right, 'L', to left, or 'B', to both synchronously.
    int numberEnemies;
} TYPE_LEVEL;

/// Defining some useful functions

// This bundles together a sprite with a texture, and the origin in the middle.
sfSprite* sfSprite_createFromFile(const char* filename, sfVector2f scale, sfVector2f pos);
// This draws the lifes on the screen (uses sfSprite_createFromFile).
void drawLifes(sfRenderWindow* window, sfSprite* life, int* numL);
// This creates one enemy and returns it.
TYPE_ENEMIES createEnemy(int color, int posX, int posY);
// This set the enemies by a level, which has the name of the map in its content
void setEnemies(TYPE_LEVEL* level);
// This draws the enemies, one by one, if these enemies have 1 in flag.
void drawEnemies(sfRenderWindow* window, TYPE_ENEMIES enemies[MAXENEMIES], int sizeArray);
// This loads all the stage (player ship, enemies, background, logic, ..., using a lot of others functions)
void layoutStage(sfRenderWindow* window, TYPE_LEVEL level);
// This loads Game Over screen
void layoutGameOver(sfRenderWindow* window, sfEvent event);
// This load the sprites of the game by a level. src can be "certinho" or "zuadasso".
void loadGameSprites(const char src[], TYPE_LEVEL* level);
// This returns the number of enemy in the array, if the sprite given has the same position than that enemy. Else, it returns -1.
int isAtSamePoint(TYPE_ENEMIES* enemies, int *tamArray, sfSprite* sprite);
// This sets all the enmies's flags to 1, given an array
void returnToLife(TYPE_ENEMIES enemies[MAXENEMIES], int sizeArray);


/// Variables

// General variables
float energy = ENERGYMAX;
int numberlifes = 3;
int nEnemies = 0;
int score = 0;

// Mouse variables
float mouseX;
float mouseY;

// Sprites
TYPE_ALLSPRITES gameSprites; // It's a global variable yet, but we want to put it in the main function, maybe


// Levels
TYPE_LEVEL level1;

TYPE_LEVEL level2;

TYPE_LEVEL chosenLevel; // We didn't use it yet


int main()
{
    // Initializing map names
    level1.mapName = "map_1.txt";
    level2.mapName = "map_2.txt";

    // Video variables
    sfVideoMode mode = {WIDTH, HEIGHT, 32};
    sfRenderWindow* window;

    // Create the main window
    window = sfRenderWindow_create(mode, "Megamania", sfResize | sfClose, NULL);

    // Playing level
    layoutStage(window, level1);

    // Cleanup resources
    sfSprite_destroy(gameSprites.ship.shipSprite);
    sfSprite_destroy(gameSprites.fire);
    sfRenderWindow_destroy(window);


    return 0;
}

void layoutStage(sfRenderWindow* window, TYPE_LEVEL level)
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

    // Enemy dead in this frame
    int positionEnemyDead;

    // Font for score
    sfFont* font;
    font = sfFont_createFromFile("Quantify Bold v2.6.ttf");

    // String for score
    char scoreString[7];
    itoa(score, scoreString, 10);

    // Text for score
    sfText* textForScore;
    textForScore = sfText_create();
        // Initializing textForScore
    sfText_setOrigin(textForScore, (sfVector2f){sfText_getLocalBounds(textForScore).width/2, sfText_getLocalBounds(textForScore).height/2});
    sfText_setFont(textForScore, font);
    sfText_setPosition(textForScore, (sfVector2f){650, 550});
    sfText_setFillColor(textForScore, sfColor_fromRGB(255, 255, 255));
    sfText_setCharacterSize(textForScore, 40);
    sfText_setString(textForScore, scoreString);

    /// Loading sprites
    loadGameSprites("certinho", &level);

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
            sfSprite_setPosition(gameSprites.fire, (sfVector2f){ -40, -40});
        }

        // Ship - Player movement
        if((sfKeyboard_isKeyPressed(sfKeyLeft)||sfKeyboard_isKeyPressed(sfKeyA)) && sfSprite_getPosition(gameSprites.ship.shipSprite).x > 40 && sfRenderWindow_hasFocus(window))
            sfSprite_move(gameSprites.ship.shipSprite, (sfVector2f){-300*dtime, 0});
        if((sfKeyboard_isKeyPressed(sfKeyRight)||sfKeyboard_isKeyPressed(sfKeyD)) && sfSprite_getPosition(gameSprites.ship.shipSprite).x < 760 && sfRenderWindow_hasFocus(window))
            sfSprite_move(gameSprites.ship.shipSprite, (sfVector2f){300*dtime, 0});


        // Fire - check collisions
        positionEnemyDead = isAtSamePoint(gameSprites.enemies, &nEnemies, gameSprites.fire); // PositionEnemyDead will update every frame

        if(positionEnemyDead != -1)
        {
            gameSprites.enemies[positionEnemyDead].flag = 0;    // Killing the enemy
            sfSprite_setPosition(gameSprites.fire, (sfVector2f){-40, -40}); // Setting another position to fire
            isFireable = 1;     // Making possible to fire again
            score += 20;
        }

        // Fire - moves the blast
        if(sfKeyboard_isKeyPressed(sfKeySpace) && isFireable && sfRenderWindow_hasFocus(window))
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

//a
        // Text for score
        itoa(score, scoreString, 10);
        sfText_setString(textForScore, scoreString);

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
        drawEnemies(window, gameSprites.enemies, nEnemies);
        sfRenderWindow_drawText(window, textForScore, NULL);

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

    returnToLife(gameSprites.enemies, nEnemies);
    score = 0;

    numberlifes = 3;
}

void loadGameSprites(const char src[], TYPE_LEVEL* level)
{
    if(strcmp(src, "certinho") == 0)
    {
        // Ship
        gameSprites.ship.shipSprite = sfSprite_createFromFile("nave.png",
                                               (sfVector2f){0.8,0.8},
                                               (sfVector2f){WIDTH/2, 450});

        // Enemies
        setEnemies(level); // In this function all about enemies and the level is done

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
        sfSprite_setPosition(life,(sfVector2f){350 + 50*i, 575});       // The first life's position is ( 350, 575)
        sfRenderWindow_drawSprite(window, gameSprites.life, NULL);
    }
}

void setEnemies(TYPE_LEVEL* level)
{
    FILE *map;

    map = fopen(level->mapName, "r"); // It will open the file whose name is a parameter (dir: bin/debug)
    puts(level->mapName);

    int sizeFile;
    int posXAux = 0;    // Initializing it
    int posYAux = 0;    // Initializing it
    int speed;          // It's the first char in the file map
    char direction;     // It's the second char in the file map
    int i;
    char buffer = '\n'; // Initializing it

    sizeFile = 149; // It was arbitrary

    rewind(map);

    fscanf(map, "%d", &speed); // It will put the first char, as a number, in speed
    level->levelSpeed = speed;

    fseek(map, 1, SEEK_SET);    // Jumping to second char of the file
    fscanf(map, "%c", &direction);  // Putting the second char of the file, which is the direction of movement of the enemies, in direction
    level->direction = direction;

    for(i = 0; i < sizeFile; i++)
    {
       buffer = getc(map);
       switch(buffer)
       {
           case '\n':   posYAux += DIST_ENEMY_Y;
                        posXAux = 0;                // It means that, in the file, we passed the an enemy's line, then we have to put the next enemies in other Y
                        break;

           case ' ':    posXAux += DIST_ENEMY_X;    // The blanks in the file means the distance between the enemies. This is the reason why we are adding posXAux
                        break;

           case 'x':    gameSprites.enemies[nEnemies] = createEnemy(rand()%4, posXAux, posYAux);    // We are using rand, but the intention is to use a defined color
                        nEnemies++;
                        break;

           default:     break;
       }
    }
}

TYPE_ENEMIES createEnemy(int color, int posX, int posY)
{
    TYPE_ENEMIES enemy;

    enemy.posX = posX;  // Making easier to lead with coordinates of enemy
    enemy.posY = posY;
    enemy.flag = 1;     // Seting him to alive (1)

    switch(color)
    {
            case 0: enemy.enemySprite = sfSprite_createFromFile("enemyBlack.png", (sfVector2f){ 0.7, 0.7}, (sfVector2f){posX, posY});
                    break;
            case 1: enemy.enemySprite = sfSprite_createFromFile("enemyBlue.png", (sfVector2f){ 0.7, 0.7}, (sfVector2f){posX, posY});
                    break;
            case 2: enemy.enemySprite = sfSprite_createFromFile("enemyRed.png", (sfVector2f){ 0.7, 0.7}, (sfVector2f){posX, posY});
                    break;
            case 3: enemy.enemySprite = sfSprite_createFromFile("enemyGreen.png", (sfVector2f){ 0.7, 0.7}, (sfVector2f){posX, posY});
                    break;


    }
    return enemy;
}

void drawEnemies(sfRenderWindow* window, TYPE_ENEMIES enemies[MAXENEMIES], int sizeArray)
{
    int i;

    for(i = 0; i < sizeArray; i++)
    {
        if(enemies[i].flag == 1)            // This function only draws alive enemys
            sfRenderWindow_drawSprite(window, (gameSprites.enemies[i]).enemySprite, NULL);
    }
}

int isAtSamePoint(TYPE_ENEMIES* enemies, int *sizeArray, sfSprite* sprite)
{
    int i;

    int numberOfEnemyDead = -1; // If there isn't an enemy dead in this array, it will be -1.
                           // Else, it will be the number of the enemy dead in it's array.

    float sizeEnemyX = sfSprite_getLocalBounds(enemies[0].enemySprite).width/2;         // It's divided by two 'cause this function gives the entire size, and we just want a half
    float sizeEnemyY = sfSprite_getLocalBounds(enemies[0].enemySprite).height/2;

    sfVector2f positionSprite = sfSprite_getPosition(sprite);

    for(i = 0; i < *sizeArray; i++)
    {
        if((((enemies[i].posX + sizeEnemyX) > positionSprite.x) && ((enemies[i].posX - sizeEnemyX) < positionSprite.x))         // The position of fire sprite has to be the same. Then, it has to be in
           && (((enemies[i].posY + sizeEnemyY) > positionSprite.y) && ((enemies[i].posY - sizeEnemyY) < positionSprite.y))      //  [centerEnemy.x + sizeEnemyX, centerEnemy.x - sizeEnemyX]. (It works at the same way with Y)
           && (enemies[i].flag == 1))
        {
            numberOfEnemyDead = i;
            i = *sizeArray;         // To finish the loop
        }
    }
    return numberOfEnemyDead;
}

void returnToLife(TYPE_ENEMIES enemies[MAXENEMIES], int sizeArray)
{
    int i; // Count

    for(i = 0; i < sizeArray; i++)
    {
        enemies[i].flag = 1;            // It's setting the enemy's flag to 1, which means that he is alive
    }
}

