/// Constants
    // Video's constants
#define WIDTH 800
#define HEIGHT 600
// Energy's bar constants
#define ENERGYMAX 436
#define ENERGYY 35
#define BARSPEED 0 // 10.9 - standard
// Enemies' constants
#define MAXENEMIES 20
#define DIST_ENEMY_X 10
#define DIST_ENEMY_Y 50
#define SPEED_ENEMY 200
// Button's constants
#define BUTTON_WIDTH 400
#define BUTTON_HEIGHT 100


// SFML headers
#include <SFML/Graphics.h>

// Default headers
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include <time.h>
#include <math.h>

/// Structs

// Struct for button
typedef struct str_button
{
    sfText* text;
    sfRectangleShape* base;
} TYPE_BUTTON;

// Struct for menu
typedef struct str_menu
{
    sfFont* font;
    sfText* megamaniaLogo;
    TYPE_BUTTON buttons[3]; // 0 - PLAY / 1 - OPTIONS / 2 - CREDITS
    sfSprite* menuBackground;
} TYPE_MENU;

// Struct for enemy
typedef struct str_enemy
{
    float posX;
    float posY;
    sfVector2f initialPos;
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
    sfSprite* gameover;
    sfSprite* enemyBlack;
    sfSprite* enemyRed;
    sfSprite* enemyBlue;
    sfSprite* enemyGreen;
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
    int paused;
} TYPE_LEVEL;

/// Defining some useful functions

// This bundles together a sprite with a texture, and the origin in the middle.
sfSprite* sfSprite_createFromFile(char* spriteMode, const char* filename, sfVector2f scale, sfVector2f pos);
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
void loadGameSprites(const char spriteMode[], TYPE_LEVEL* level);
// This returns the number of enemy in the array, if the sprite given has the same position than that enemy. Else, it returns -1.
int isAtSamePoint(TYPE_ENEMIES* enemies, int *tamArray, sfSprite* sprite);
// This creates a button with some text
TYPE_BUTTON createButton(char stringText[50], float textSize, sfVector2f position, sfVector2f baseSize, sfColor cBase);
// This is the menu of the game
void gameMenu(sfRenderWindow* window);
// This show the credits
void showCredits (sfRenderWindow* window);
float scoreByEnergyBar (float energy, float maxEnergy);
void moveEnemies(TYPE_LEVEL level, TYPE_ENEMIES enemies[MAXENEMIES], int sizeArray, float dtime);
//int fillEnergyBarAnimation(sfRenderWindow* window, TYPE_ALLSPRITES sprites, int dtime, float * energy);
/// Variables

// General variables
float energy = ENERGYMAX;
int numberlifes = 3;
int nEnemies = 0;
int liveEnemies = 0;
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
    gameMenu(window);

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
    int isFireable = 1;
    int pPressedLastTime = 0;
    int shouldLoop = 1; // Used to indicate whether it should continue looping or not

    // Animations
    int animating = 1; // 1 - started stage, 2 - just died
    // These are used to control the amount of time the animations run:
    float timeOfDeath = 0; // How many seconds the death animation has been running
    float timeOfLife = 0; // How many seconds the "being born" animation has been running
    float timeOfPhaseOut = 0; //How many seconds the "getting out of level" animation has been running
    float scoreAtWin = 0;

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

    // Energy Bar
    energy = 0;

    /// Loop of the layout
    while(shouldLoop)
    {
        /// Code to close the window
        while(sfRenderWindow_pollEvent(window, &event))
        {
            // Close window : exit
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        /// Logic of the layout

        // When the energy ends, you lose a life (not valid if it's doing the animation)
        if(energy <= 0 && !animating)
        {
            numberlifes--;  // -1 life ;-;
            animating = 2;
        }

        /// Pause Logic
        // Makes sure you don't pause and unpause every frame when the button is pressed
        if(sfKeyboard_isKeyPressed(sfKeyP))
        {
            if(!pPressedLastTime)
            {
                level.paused = !level.paused;
            }
            pPressedLastTime = 1;
        }
        else
        {
            pPressedLastTime = 0;
        }

        if(!level.paused)
        {
            ///Player Interactions
            //they gotta be inside this condition here, so the player can't interact while it's paused.

            // Ship - Player movement
            if((sfKeyboard_isKeyPressed(sfKeyLeft)||sfKeyboard_isKeyPressed(sfKeyA)) && sfSprite_getPosition(gameSprites.ship.shipSprite).x > 40 && sfRenderWindow_hasFocus(window))
                sfSprite_move(gameSprites.ship.shipSprite, (sfVector2f){-300*dtime, 0});
            if((sfKeyboard_isKeyPressed(sfKeyRight)||sfKeyboard_isKeyPressed(sfKeyD)) && sfSprite_getPosition(gameSprites.ship.shipSprite).x < 760 && sfRenderWindow_hasFocus(window))
                sfSprite_move(gameSprites.ship.shipSprite, (sfVector2f){300*dtime, 0});

            // Action of firing the blast
            if(sfKeyboard_isKeyPressed(sfKeySpace) && isFireable && sfRenderWindow_hasFocus(window))
            {
                sfSprite_setPosition(gameSprites.fire, (sfVector2f){sfSprite_getPosition(gameSprites.ship.shipSprite).x, sfSprite_getPosition(gameSprites.ship.shipSprite).y - 40});
            }
        }

        // Fire - moves the blast
        if(sfSprite_getPosition(gameSprites.fire).y >= -40)
        {
            sfSprite_setPosition(gameSprites.fire, (sfVector2f){sfSprite_getPosition(gameSprites.ship.shipSprite).x, sfSprite_getPosition(gameSprites.fire).y -600*dtime});
            isFireable = 0;
        }
        else
            isFireable = 1;
        // Fire - check collisions
        positionEnemyDead = isAtSamePoint(gameSprites.enemies, &nEnemies, gameSprites.fire); // PositionEnemyDead will update every frame

        if(positionEnemyDead != -1)
        {
            gameSprites.enemies[positionEnemyDead].flag = 0;    // Killing the enemy
            sfSprite_setPosition(gameSprites.fire, (sfVector2f){-40, -40}); // Setting another position to fire
            isFireable = 1;     // Making possible to fire again
            liveEnemies--;
            score += 20;
        }

        // Enemies
        moveEnemies(level, gameSprites.enemies, nEnemies, dtime);

        // Dying condition, plays the dying animation
        if(numberlifes == 0 && timeOfDeath < 1)
            animating = 2;

        // Killed all enemies? play the "get them points son" animation
        if(liveEnemies == 0 && !animating)
        {
            animating = 3;
            scoreAtWin = score + scoreByEnergyBar(energy, ENERGYMAX);
        }

        // Energy bar
        if(!animating)
        {
            energy -= BARSPEED*dtime; // To empty the life bar
            sfRectangleShape_setSize(gameSprites.fillLifeBar2, (sfVector2f){energy, ENERGYY});
        }
        /// Animation
        else
        {
            // No matter what's the animation:
            // Pause the game
            level.paused = 1;
            // Calculate dtime inside
            time = sfClock_getElapsedTime(clock);
            dtime = sfTime_asSeconds(time) - sfTime_asSeconds(lasttime);
            lasttime = time;

            // If level has just started - "being born"
            if(animating == 1)
            {
                energy += ENERGYMAX*dtime;
                if(energy > ENERGYMAX)
                    energy = ENERGYMAX;
                sfRectangleShape_setSize(gameSprites.fillLifeBar2, (sfVector2f){energy, ENERGYY});

                // Plays the animation for 1 second
                if(timeOfLife < 1)
                {
                    sfSprite_setColor(gameSprites.ship.shipSprite, sfColor_fromRGBA(255,255,255, timeOfLife*255));
                }
                else if(energy == ENERGYMAX)
                {
                    // End the animation, reset timeOfLife and unpause the game
                    animating = 0;
                    timeOfLife = 0;
                    level.paused = 0;
                }
                timeOfLife += dtime;
            }
            // If player has died
            else if(animating == 2)
            {
                // Plays the animation for 1 second
                if(timeOfDeath < 1)
                {
                    sfSprite_setColor(gameSprites.ship.shipSprite, sfColor_fromRGBA(255,(1-timeOfDeath)*255, (1-timeOfDeath)*255, (1-timeOfDeath)*255));
                }
                else
                {
                    // End the animation, unpause the game, reset timeOfDeath and exit loop (replay stage)
                    animating = 0;
                    level.paused = 0;
                    timeOfDeath = 0;
                    if(numberlifes == 0)
                        shouldLoop = 0;
                    else
                    {
                        animating = 1;
                        sfSprite_setPosition(gameSprites.ship.shipSprite, (sfVector2f) {WIDTH/2, 450});
                    }
                }
                timeOfDeath += dtime;
            }
            // If you've just beat a stage - "getting out of level"
            else if(animating == 3)
            {
                // Decrease the energy bar, increase the score.
                energy -= ENERGYMAX*dtime;
                score += scoreByEnergyBar(ENERGYMAX*dtime, ENERGYMAX);

                // Makes sure the energy never gets below 0, and that you don't get more points than you should
                if(energy < 0)
                {
                    score += scoreByEnergyBar(-energy, ENERGYMAX);
                    energy = 0;
                }
                sfRectangleShape_setSize(gameSprites.fillLifeBar2, (sfVector2f){energy, ENERGYY});

                // If you have full energy, the animation takes 1 second. If not, it takes a fraction of a second
                if(timeOfPhaseOut < energy/ENERGYMAX)
                {
                    // Some math to figure out the ship's alpha value, so that it ends when the energy bar is emptied.
                    sfSprite_setColor(gameSprites.ship.shipSprite, sfColor_fromRGBA(255,255,255, (1-(ENERGYMAX*timeOfPhaseOut/energy))*255));
                }
                else if(energy == 0)
                {
                    // End the animation, unpause the game, reset timeOfPhaseOut and exit loop (go to next stage)
                    animating = 0;
                    level.paused = 0;
                    timeOfPhaseOut = 0;
                    shouldLoop = 0;
                    score = scoreAtWin;
                }
                timeOfPhaseOut += dtime;
            }
        }


        // Text for score
        itoa(score, scoreString, 10);
        sfText_setString(textForScore, scoreString);

        /// Calculate dtime
        if(level.paused)
        {
            dtime = 0;
            // Still gotta calculate the time passed, so dtime doesn't get crazy
            time = sfClock_getElapsedTime(clock);
            lasttime = time;
        }
        else
        {
            time = sfClock_getElapsedTime(clock);
            dtime = sfTime_asSeconds(time) - sfTime_asSeconds(lasttime);
            lasttime = time;
        }

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
    }
    //score += scoreByEnergyBar(energy, ENERGYMAX);
}

void layoutGameOver(sfRenderWindow* window, sfEvent event)
{
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
        sfRenderWindow_drawSprite(window, gameSprites.gameover, NULL);
        sfRenderWindow_display(window);
    } while(!(sfMouse_isButtonPressed(sfMouseLeft) && sfRenderWindow_hasFocus(window) && sfMouse_getPosition(window).y > 0));
    //Mouse position Y is used to drag the window when the gameover screen is on.

    // Setting config as the first level
    setEnemies(&level1);
    score = 0;
    numberlifes = 3;
}

void loadGameSprites(const char spriteMode[], TYPE_LEVEL* level)
{
    // Loads the sprites according with the sprite mode ("certinho" or "zuadasso")
    //char spriteMode[15];
    //strcpy(spriteMode, src);
    //strcat(spriteMode,"/");

    //char spritePath[50];
    // We have to call this every time we create a new sprite, to reset the path to the spriteMode
    //strcpy(spritePath, spriteMode);

    gameSprites.ship.shipSprite = sfSprite_createFromFile(spriteMode, "nave.png",
                                                         (sfVector2f){0.8,0.8},
                                                         (sfVector2f){WIDTH/2, 450});

    // Fire
    gameSprites.fire = sfSprite_createFromFile(spriteMode, "fire.png",
                                            (sfVector2f){0.5,0.5},
                                            (sfVector2f){-40, -40});

    // Background
    gameSprites.background = sfSprite_createFromFile(spriteMode, "background.png",
                                                  (sfVector2f){3.125, 2.3475},
                                                  (sfVector2f){WIDTH/2, HEIGHT/2});

    // Lifes
    gameSprites.life = sfSprite_createFromFile(spriteMode, "life.png",
                                            (sfVector2f){1,1},
                                            (sfVector2f){350,575});

    // Life bar
    gameSprites.lifebar = sfSprite_createFromFile(spriteMode, "lifebar.png",
                                               (sfVector2f){2, 1},
                                               (sfVector2f){WIDTH/2, 525});

    gameSprites.gameover = sfSprite_createFromFile(spriteMode, "gameover.png",
                                                  (sfVector2f){1,1},
                                                  (sfVector2f){WIDTH/2, HEIGHT/2});

    gameSprites.enemyBlack = sfSprite_createFromFile(spriteMode, "enemyBlack.png", (sfVector2f){ 0.7, 0.7}, (sfVector2f){-100, -100});
    gameSprites.enemyRed = sfSprite_createFromFile(spriteMode, "enemyBlue.png", (sfVector2f){ 0.7, 0.7}, (sfVector2f){-100, -100});
    gameSprites.enemyGreen = sfSprite_createFromFile(spriteMode, "enemyRed.png", (sfVector2f){ 0.7, 0.7}, (sfVector2f){-100, -100});
    gameSprites.enemyBlue = sfSprite_createFromFile(spriteMode, "enemyGreen.png", (sfVector2f){ 0.7, 0.7}, (sfVector2f){-100, -100});

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

sfSprite* sfSprite_createFromFile(char* spriteMode, const char* filename, sfVector2f scale, sfVector2f pos)
{
    sfTexture* texture;
    sfSprite* sprite;

    char img[40];
    strcpy(img, "imgs/");
    strcat(img, spriteMode);
    strcat(img,"/");

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

    nEnemies = 0;
    liveEnemies = 0;

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

           case 'x':    gameSprites.enemies[nEnemies] = createEnemy(rand()%4, posXAux, posYAux);    // We are using rand, but the intention is to use a defined color
                        gameSprites.enemies[nEnemies].initialPos = (sfVector2f){posXAux, posYAux};
                        nEnemies++;
                        liveEnemies++;
                        break;

           default:     break;
       }
    }
    fclose(map);
}

TYPE_ENEMIES createEnemy(int color, int posX, int posY)
{
    TYPE_ENEMIES enemy;

    enemy.color = color;
    enemy.posX = posX;  // Making easier to lead with coordinates of enemy
    enemy.posY = posY;
    enemy.flag = 1;     // Seting him to alive (1)

    return enemy;
}

void drawEnemies(sfRenderWindow* window, TYPE_ENEMIES enemies[MAXENEMIES], int sizeArray)
{
    int i;
    sfVector2f bufferPos;

    for(i = 0; i < sizeArray; i++)
    {
        bufferPos.x = gameSprites.enemies[i].posX;
        bufferPos.y = gameSprites.enemies[i].posY;

        // This function only draws alive enemys
        if(enemies[i].flag == 1)
        {
            switch(gameSprites.enemies[i].color)
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

int isAtSamePoint(TYPE_ENEMIES* enemies, int *sizeArray, sfSprite* sprite)
{
    int i;

    int numberOfEnemyDead = -1; // If there isn't an enemy dead in this array, it will be -1.
                                // Else, it will be the number of the enemy dead in it's array.

    // It's divided by two 'cause this function gives the entire size, and we just want a half
    float sizeEnemyX = sfSprite_getLocalBounds(gameSprites.enemyBlack).width/2;
    float sizeEnemyY = sfSprite_getLocalBounds(gameSprites.enemyBlack).height/2;

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

void moveEnemies(TYPE_LEVEL level, TYPE_ENEMIES enemies[MAXENEMIES], int sizeArray, float dtime)
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

void gameMenu(sfRenderWindow* window)
{
    int i; // Count
    int flagButton = -1; // 0 - Play / 1 - Options / 2 - Credits
    int gameoverFlag = 0; // If it's 0, you don't want to play again after the game over screen. If it's 1, you want.
    sfEvent event;

    TYPE_MENU patternMenu;

    patternMenu.font = sfFont_createFromFile("Quantify Bold v2.6.ttf");

    /// Initializing background
    patternMenu.menuBackground = sfSprite_createFromFile("certinho", "background.png",
                                                        (sfVector2f){3.125, 2.3475},
                                                        (sfVector2f){WIDTH/2, HEIGHT/2});

    /// Initializing megamania logo
    patternMenu.megamaniaLogo = sfText_create();
    sfText_setCharacterSize(patternMenu.megamaniaLogo, 60);
    sfText_setString(patternMenu.megamaniaLogo, "M E G A M A N I A");
    sfText_setFont(patternMenu.megamaniaLogo, patternMenu.font);
    sfText_setOrigin(patternMenu.megamaniaLogo, (sfVector2f){sfText_getLocalBounds(patternMenu.megamaniaLogo).width/2, sfText_getLocalBounds(patternMenu.megamaniaLogo).height/2});
    sfText_setPosition(patternMenu.megamaniaLogo, (sfVector2f){WIDTH/2, HEIGHT/6});

    /// Initializing play button
    patternMenu.buttons[0] = createButton("P L A Y", 40, (sfVector2f){WIDTH/2, 320}, (sfVector2f){BUTTON_WIDTH, BUTTON_HEIGHT}, sfColor_fromRGB( 18, 16, 18));

    /// Initializing option button
    patternMenu.buttons[1] = createButton("O P T I O N S", 40, (sfVector2f){WIDTH/2, 430}, (sfVector2f){BUTTON_WIDTH, BUTTON_HEIGHT}, sfColor_fromRGB( 18, 16, 18));

    /// Initializing credits button
    patternMenu.buttons[2] = createButton("C R E D I T S", 40, (sfVector2f){WIDTH/2, 540}, (sfVector2f){BUTTON_WIDTH, BUTTON_HEIGHT}, sfColor_fromRGB( 18, 16, 18));

    /// Loop of the screen
    while(sfRenderWindow_isOpen(window))
    {
        /// Code to close the window
        while(sfRenderWindow_pollEvent(window, &event))
        {
            // Close window : exit
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        /// Checking if mouse is on a button
        for(i = 0; i < 3; i++)
        {
            if((sfMouse_getPosition(window).y <= (370 + i*110) && sfMouse_getPosition(window).y >= (270 + i*110)) &&
               (sfMouse_getPosition(window).x <= 600 && sfMouse_getPosition(window).x >= 200))
            {
                sfRectangleShape_setOutlineColor(patternMenu.buttons[i].base, sfColor_fromRGB( 255, 255, 255));
                // Check click
                if(sfMouse_isButtonPressed(sfMouseLeft))
                {
                    flagButton = i;
                }
            }
            else
                sfRectangleShape_setOutlineColor(patternMenu.buttons[i].base, sfColor_fromRGB( 0, 0, 0));
        }
        switch(flagButton)
        {
            case 0:         /// Level 1
                            // Loading sprites of the game
                            loadGameSprites("certinho", &level1);
                        do
                        {
                            // Setting the Level 1's enemies
                            setEnemies(&level1);

                            // Beginning the Level 1
                            layoutStage(window, level1);

                            /// Level 2
                            if(numberlifes > 0) // It means that the player did not dead 3 times in the first level
                            {
                                // Setting the Level 2's enemies
                                setEnemies(&level2);

                                // Beginning the Level 2
                                layoutStage(window, level2);
                            }

                            gameoverFlag = 0; // Making possible to enter in a game over, and then you finish the levels

                            if(numberlifes <= 0) // It means that the player dead 3 times, then, he can back and play again the first level
                            {
                                // Show Game Over screen
                                layoutGameOver(window, event); // Here, you can click and you will starts from beginning again
                                gameoverFlag = 1;
                                // Setting the enemies to beginning definitions
                                liveEnemies = nEnemies;
                            }
                        }while(gameoverFlag);
                        score = 0;
                        flagButton = -1;
                        break;

            case 1:     break;

            case 2:     showCredits(window);
                        flagButton = -1;
                        break;

            default:    break;
        }

        /// Drawing on the screen
        sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
        // Background
        sfRenderWindow_drawSprite(window, patternMenu.menuBackground, NULL);
        // Logo
        sfRenderWindow_drawText(window, patternMenu.megamaniaLogo, NULL);
        // Buttons
        for(i = 0; i < 3; i++)
        {
            sfRenderWindow_drawRectangleShape(window, patternMenu.buttons[i].base, NULL);
            sfRenderWindow_drawText(window, patternMenu.buttons[i].text, NULL);
        }
        sfRenderWindow_display(window);
    }
}

TYPE_BUTTON createButton(char stringText[50], float textSize, sfVector2f position, sfVector2f baseSize, sfColor cBase)
{
    TYPE_BUTTON button;

    sfFont* font;
    font = sfFont_createFromFile("Quantify Bold v2.6.ttf"); // Font of the utton

        // Text of button
    button.text = sfText_create();
    sfText_setCharacterSize(button.text, textSize);
    sfText_setString(button.text, stringText);
    sfText_setFont(button.text, font);
    sfText_setOrigin(button.text, (sfVector2f){sfText_getLocalBounds(button.text).width/2, sfText_getLocalBounds(button.text).height/2});   // The origin will be at the center of the text
    sfText_setPosition(button.text, position);
        // Base of button
    button.base = sfRectangleShape_create();
    sfRectangleShape_setSize(button.base, baseSize);
    sfRectangleShape_setOrigin(button.base, (sfVector2f){baseSize.x/2, baseSize.y/2});  // The origin will be at the center of the sprite
    sfRectangleShape_setFillColor(button.base, cBase);
    sfRectangleShape_setOutlineColor(button.base, sfColor_fromRGB( 0, 0, 0));   // Setting the outlinecolor to black
    sfRectangleShape_setOutlineThickness(button.base, 1);
    sfRectangleShape_setPosition(button.base, (sfVector2f){ position.x, position.y});

    return button;
}

void showCredits (sfRenderWindow* window)
{
    sfEvent event;

    TYPE_BUTTON backButton;
    int flagButton = 0; // If it's 0, you didn't click on the button. Else, you did.

    sfFont* font;

    sfText* creators;   // It talks about who creates this game
    sfText* spritesPack;    // Where we pick these sprites
    sfText* why;        // Why we did it

    sfSprite* background;

    background = sfSprite_createFromFile("certinho", "background.png",
                                         (sfVector2f){3.125, 2.3475},
                                         (sfVector2f){WIDTH/2, HEIGHT/2});

    // Setting font
    font = sfFont_createFromFile("Quantify Bold v2.6.ttf"); // Font of the utton

    /// Setting texts
        // Set creators
    creators = sfText_create();
    sfText_setCharacterSize(creators, 20);
    sfText_setString(creators, "C R E A T O R S :\nProgrammer: Marcos Samuel Landi\nProgrammer: Henry Bernardo K. de Avila.");
    sfText_setFont(creators, font);
    sfText_setOrigin(creators, (sfVector2f){sfText_getLocalBounds(creators).width/2, sfText_getLocalBounds(creators).height/2});   // The origin will be at the center of the text
    sfText_setPosition(creators, (sfVector2f){WIDTH/2, 200});
        // Set spritesPack
    spritesPack = sfText_create();
    sfText_setCharacterSize(spritesPack, 20);
    sfText_setString(spritesPack, "S P R I T E ' S  P A C K :\nSpace Shooter (Redux, plus fonts and sounds)\nby Kenney Vleugels (www.kenney.nl).");
    sfText_setFont(spritesPack, font);
    sfText_setOrigin(spritesPack, (sfVector2f){sfText_getLocalBounds(spritesPack).width/2, sfText_getLocalBounds(spritesPack).height/2});   // The origin will be at the center of the text
    sfText_setPosition(spritesPack, (sfVector2f){WIDTH/2, 300});
        // Set why
    why = sfText_create();
    sfText_setCharacterSize(why, 20);
    sfText_setString(why, "W H Y  H A V E  Y O U  D O N E  T H I S ?\nWell, this game is the final project of Programming and Algorythms, a subject in UFRGS.");
    sfText_setFont(why, font);
    sfText_setOrigin(why, (sfVector2f){sfText_getLocalBounds(why).width/2, sfText_getLocalBounds(why).height/2});   // The origin will be at the center of the text
    sfText_setPosition(why, (sfVector2f){WIDTH/2, 400});

    // Setting back's button
    backButton = createButton("B A C K", 40, (sfVector2f){WIDTH - 200, 100}, (sfVector2f){200, 100}, sfColor_fromRGB(18, 16, 18));
//
    do
    {
        /// Code to close the window
        while(sfRenderWindow_pollEvent(window, &event))
        {
            // Close window : exit
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        if((sfMouse_getPosition(window).y <= 150 && sfMouse_getPosition(window).y >= 50) &&
                   (sfMouse_getPosition(window).x <= 700 && sfMouse_getPosition(window).x >= 500))
        {
            sfRectangleShape_setOutlineColor(backButton.base, sfColor_fromRGB( 255, 255, 255));
            // Check click
            if(sfMouse_isButtonPressed(sfMouseLeft))
            {
                flagButton = 1;
            }
        }
        else
            sfRectangleShape_setOutlineColor(backButton.base, sfColor_fromRGB( 0, 0, 0));


        /// Drawing on the screen
        sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
            // Background
        sfRenderWindow_drawSprite(window, background, NULL);
            // BackButton
        sfRenderWindow_drawRectangleShape(window, backButton.base, NULL);
        sfRenderWindow_drawText(window, backButton.text, NULL);
            // Why
        sfRenderWindow_drawText(window, why, NULL);
            // Sprites pack
        sfRenderWindow_drawText(window, spritesPack, NULL);
            // Creators
        sfRenderWindow_drawText(window, creators, NULL);
        sfRenderWindow_display(window);
    }while(!flagButton);
}

float scoreByEnergyBar (float energy, float maxEnergy)
{
    float answer;

    answer = energy/(maxEnergy/40) * 50;    // We want the energy bar to have 40 parts, so we divide it by 40 and divide the current energy by this
                                            // fraction, that tells us how many "slices" of energy we have. each slice is worth 50 energy (which is defined at the PDF)
    return answer;
}

