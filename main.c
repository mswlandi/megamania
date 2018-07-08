/// Constants
// Video's constants
#define WIDTH 800
#define HEIGHT 600
// Energy's bar constants
#define ENERGYMAX 436
#define ENERGYY 35
#define BARSPEED 10.9 // 10.9 - standard
// Enemies' constants
#define MAXENEMIES 20
#define DIST_ENEMY_X 10
#define DIST_ENEMY_Y 50
#define SPEED_ENEMY 200
#define ENEMYFIRE_SPEED 300
#define FRAMES_TO_DIR 2000
#define TIME_TO_FALL 1.65 // How much time, approximately, it takes for an enemy fire to reach the ground
// Button's constants
#define BUTTON_WIDTH 400
#define BUTTON_HEIGHT 100
// Levels' constants
#define MAX_LEVELS


// SFML headers
#include <SFML/Graphics.h>

// Default headers
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// Our headers
#include "sprites.h"
#include "enemies.h"
#include "structs.h"
#include "score.h"
#include "layout.h"
#include "utility.h"


/// Defining some useful functions

// This loads all the stage (player ship, enemies, background, logic, ..., using a lot of others functions)
void layoutStage(sfRenderWindow* window, TYPE_LEVEL level);
// This loads Game Over screen
void layoutGameOver(sfRenderWindow* window, sfEvent event);
// This returns the number of enemy in the array, if the sprite given has the same position than that enemy. Else, it returns -1.
int isAtSamePoint(TYPE_ENEMIES* enemies, int sizeArray, sfSprite* sprite);
// This is the menu of the game
void gameMenu(sfRenderWindow* window);
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
    // Loads all of the game's sprites
    loadGameSprites(&gameSprites);

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
    int isB = 0; // If direction is 'B', it's 1. Else, 0.

    // Animations
    int animating = 1; // 1 - started stage, 2 - just died
    float scoreAtWin = 0; // To keep track of the actual "score gain" when the player goes to next level
    // These are used to control the amount of time the animations run:
    float timeOfDeath = 0; // How many seconds the death animation has been running
    float timeOfLife = 0; // How many seconds the "being born" animation has been running
    float timeOfPhaseOut = 0; // How many seconds the "getting out of level" animation has been running

    // Enemy move
    int count = 0;
    char directionEnemyMove = level.direction;

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

    // Energy Bar
    energy = 0;

    if(directionEnemyMove == 'B')
        isB = 1;

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
        if(!outOfScreen(gameSprites.fire, HEIGHT, WIDTH))
        {
            sfSprite_setPosition(gameSprites.fire, (sfVector2f){sfSprite_getPosition(gameSprites.ship.shipSprite).x, sfSprite_getPosition(gameSprites.fire).y -600*dtime});
            isFireable = 0;
        }
        else
            isFireable = 1;

        // Fire - check collisions
        positionEnemyDead = isAtSamePoint(gameSprites.enemies, nEnemies, gameSprites.fire); // PositionEnemyDead will update every frame

        if(positionEnemyDead != -1)
        {
            gameSprites.enemies[positionEnemyDead].flag = 0;    // Killing the enemy
            sfSprite_setPosition(gameSprites.fire, (sfVector2f){-40, -40}); // Setting another position to fire
            isFireable = 1;     // Making possible to fire again
            liveEnemies--;
            score += 20;
        }

        /// Enemies
        // Enemies moving in "Both" mode
        if(isB && count < (FRAMES_TO_DIR/level.levelSpeed))
        {
            level.direction = 'R';
            count++;
        }
        else
            if(isB && count >= (FRAMES_TO_DIR/level.levelSpeed))
            {
                level.direction = 'L';
                if(count == ((FRAMES_TO_DIR*2)/level.levelSpeed - 1))
                    count = 0;
                else
                    count++;
            }

        Enemies_Move(level, gameSprites.enemies, nEnemies, dtime);

        // Enemy fire
        // Did the time come for some enemy to fire?
        // No enemy can shoot if the player is being animated
        if(level.lastShot >= TIME_TO_FALL/level.levelSpeed && !animating)
        {
                Enemies_Shooting(gameSprites.enemies, nEnemies, liveEnemies, level.levelSpeed);
                level.lastShot = 0;
        }

        /// Animation Conditions
        // Dying condition, plays the dying animation
        if(numberlifes == 0 && timeOfDeath < 1)
            animating = 2;

        // Killed all enemies? play the "get them points son" animation
        if(liveEnemies == 0 && !animating)
        {
            animating = 3;
            scoreAtWin = score + Score_EnergyBar(energy, ENERGYMAX);
        }

        // Only executes if there's no animation:
        if(!animating)
        {
            // When the energy ends, you lose a life
            if(energy <= 0)
            {
                numberlifes--;  // -1 life ;-;
                animating = 2;
            }

            energy -= BARSPEED*dtime; // To empty the life bar
            sfRectangleShape_setSize(gameSprites.fillLifeBar2, (sfVector2f){energy, ENERGYY});

            if(Enemies_MovingFires(ENEMYFIRE_SPEED, gameSprites.enemies, nEnemies, dtime, gameSprites.ship))
                energy = 0;
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

            // Destroy all of the fires (enemy and player's)
            Enemies_destroyFires(gameSprites.enemies, nEnemies); ///TODO: define this
            sfSprite_setPosition(gameSprites.fire, (sfVector2f) {-40,-40});

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
                score += Score_EnergyBar(ENERGYMAX*dtime, ENERGYMAX);

                // Makes sure the energy never gets below 0, and that you don't get more points than you should
                if(energy < 0)
                {
                    score += Score_EnergyBar(-energy, ENERGYMAX);
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
                    score = scoreAtWin; // Also set the score to the actual value it should be.
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
            level.lastShot += dtime;
        }

        /// Actual drawing
        sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
        sfRenderWindow_drawSprite(window, gameSprites.background, NULL);
        sfRenderWindow_drawSprite(window, gameSprites.ship.shipSprite, NULL);
        sfRenderWindow_drawSprite(window, gameSprites.fire, NULL);
        Enemies_DrawFires(window, gameSprites.enemyFire, gameSprites.enemies, nEnemies);
        sfRenderWindow_drawRectangleShape(window, gameSprites.base, NULL);
        drawLifes(window, gameSprites.life, &numberlifes);
        sfRenderWindow_drawSprite(window, gameSprites.lifebar, NULL);
        sfRenderWindow_drawRectangleShape(window, gameSprites.fillLifeBar, NULL);
        sfRenderWindow_drawRectangleShape(window, gameSprites.fillLifeBar2, NULL);
        Enemies_Draw(window, gameSprites.enemies, nEnemies, gameSprites);
        sfRenderWindow_drawText(window, textForScore, NULL);
        sfRenderWindow_display(window);
    }
}

void layoutGameOver(sfRenderWindow* window, sfEvent event)
{
    sfText* scoreLayout;
    sfText* scoreNb;

    scoreLayout = Score_CreateLayout((sfVector2f){WIDTH/2, HEIGHT/6}, 40);
    scoreNb = Score_TextCreate(score, (sfVector2f){WIDTH/2, HEIGHT/4}, 30);

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
        sfRenderWindow_drawText(window, scoreLayout, NULL);
        sfRenderWindow_drawText(window, scoreNb, NULL);
        sfRenderWindow_display(window);
    } while(!(sfMouse_isButtonPressed(sfMouseLeft) && sfRenderWindow_hasFocus(window) && sfMouse_getPosition(window).y > 0));
    //Mouse position Y is used to drag the window when the gameover screen is on.

    // Setting config as the first level
    Enemies_Set(&level1, gameSprites.enemies, &nEnemies, &liveEnemies);
    score = 0;
    numberlifes = 3;
}

int isAtSamePoint(TYPE_ENEMIES* enemies, int sizeArray, sfSprite* sprite)
{
    int i;

    sfFloatRect enemyRect;
    sfFloatRect spriteRect = sfSprite_getGlobalBounds(sprite);

    int numberOfEnemyDead = -1; // If there isn't an enemy dead in this array, it will be -1.
                                // Else, it will be the number of the enemy dead in it's array.

    float sizeEnemyX = sfSprite_getLocalBounds(gameSprites.enemyBlack).width;
    float sizeEnemyY = sfSprite_getLocalBounds(gameSprites.enemyBlack).height;

    for(i = 0; i < sizeArray; i++)
    {
        // The top-left corner is calculated according to the position,
        // which is at the center of the sprite.
        enemyRect.top = enemies[i].posY - sizeEnemyY/2;
        enemyRect.left = enemies[i].posX - sizeEnemyX/2;
        enemyRect.width = sizeEnemyX;
        enemyRect.height = sizeEnemyY;

        if(sfFloatRect_intersects(&enemyRect, &spriteRect, NULL))
        {
            numberOfEnemyDead = i;
        }
    }

    return numberOfEnemyDead;
}

void gameMenu(sfRenderWindow* window)
{
    int i; // Count
    int flagButton = -1; // 0 - Play / 1 - Options / 2 - Credits
    int gameoverFlag = 0; // If it's 0, you don't want to play again after the game over screen. If it's 1, you want.

    FILE *highscores;
    int positionScore = 0; // If it's 0, your score didn't enter in the highscores
    sfEvent event;

    TYPE_MENU patternMenu;

    patternMenu.font = sfFont_createFromFile("Quantify Bold v2.6.ttf");

    /// Initializing megamania logo
    patternMenu.megamaniaLogo = sfText_create();
    sfText_setCharacterSize(patternMenu.megamaniaLogo, 60);
    sfText_setString(patternMenu.megamaniaLogo, "M E G A M A N I A");
    sfText_setFont(patternMenu.megamaniaLogo, patternMenu.font);
    sfText_setOrigin(patternMenu.megamaniaLogo, (sfVector2f){sfText_getLocalBounds(patternMenu.megamaniaLogo).width/2, sfText_getLocalBounds(patternMenu.megamaniaLogo).height/2});
    sfText_setPosition(patternMenu.megamaniaLogo, (sfVector2f){WIDTH/2, HEIGHT/6});

    /// Initializing play button
    patternMenu.buttons[0] = Utility_CreateButton("P L A Y", 40, (sfVector2f){WIDTH/2, 320}, (sfVector2f){BUTTON_WIDTH, BUTTON_HEIGHT}, sfColor_fromRGB( 18, 16, 18));

    /// Initializing option button
    patternMenu.buttons[1] = Utility_CreateButton("S C O R E B O A R D", 40, (sfVector2f){WIDTH/2, 430}, (sfVector2f){BUTTON_WIDTH, BUTTON_HEIGHT}, sfColor_fromRGB( 18, 16, 18));

    /// Initializing credits button
    patternMenu.buttons[2] = Utility_CreateButton("C R E D I T S", 40, (sfVector2f){WIDTH/2, 540}, (sfVector2f){BUTTON_WIDTH, BUTTON_HEIGHT}, sfColor_fromRGB( 18, 16, 18));

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
            if(Utility_isOnButton(&patternMenu.buttons[i], window))
                flagButton = i;
        }
        switch(flagButton)
        {
            case 0:     /// Level 1
                        do
                        {
                            // Setting the Level 1's enemies
                            Enemies_Set(&level1, gameSprites.enemies, &nEnemies, &liveEnemies);

                            // Beginning the Level 1
                            layoutStage(window, level1);

                            /// Level 2
                            if(numberlifes > 0) // It means that the player did not dead 3 times in the first level
                            {
                                // Setting the Level 2's enemies
                                Enemies_Set(&level2, gameSprites.enemies, &nEnemies, &liveEnemies);

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
                        // Verifying if there is a new highscore
                        positionScore = Score_AddHighScore(highscores, score);
                        if(positionScore)
                            printf("Congratulation!! Your position was: %d\n", positionScore);

                        score = 0;
                        flagButton = -1;
                        break;

            case 1:     Layout_Highscores(window, gameSprites.background);
                        flagButton = -1;
                        break;

            case 2:     Layout_Credits(window, gameSprites.background);
                        flagButton = -1;
                        break;

            default:    break;
        }

        /// Drawing on the screen
        sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
        // Background
        sfRenderWindow_drawSprite(window, gameSprites.menuBackground, NULL);
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
