/*
    Este Programa foi feito para o nosso trabalho final da cadeira de Algoritmos e Programa��o da UFRGS, em 2018/1.
    � um clone do jogo Megamania, originalmente lan�ado para Atari 2600, utilizando a linguagem C e uma biblioteca gr�fica.

    A l�gica de execu��o do programa, junto com o resto da descri��o do trabalho, est� descrita no documento do trabalho
    sob o t�tulo "A L�gica do Jogo".

    Este arquivo, junto com layout.h, descreve as �fun��es layout� e outras fun��es relacionadas aos layouts,
    todas com o prefixo �Layout_�.

    Autores: Henry de Avila e Marcos Landi

    08/07/2018
*/
#include "layout.h"

void Layout_Stage(sfRenderWindow* window, TYPE_LEVEL level)
{
    /// Initializing stuff when the layout opens

    // Event to check if the button to close the window is clicked
    sfEvent event;

    // Time variables
    // Dtime logic: dtime is used to keep all the movement in the layout stable, no matter what's the fps, which varies according
    // to computer capabilities. dtime is the time since the last frame ran, in seconds, so if you multiply any movement value by it,
    // the movement will be whatever it was, in pixel/second. For example, if you add 300*dtime to the position of a sprite each frame,
    // the sprite will move 300 pixels each second, doesn't matter the fps.
    sfClock* clock = sfClock_create();
    sfTime time = sfClock_getElapsedTime(clock);
    sfTime lasttime = time;
    float dtime = 0;

    // Flags
    int isFireable = 1;
    int pPressedLastTime = 0;
    int shouldLoop = 1; // Used to indicate whether it should continue looping or not
    int isB = 0; // If direction is 'B', it's 1. Else, 0.
    int invencibilityOn = 0; // If it's 0, cheat is disabled. Else, is enabled.
    int iPressedLastTime = 0;

    // Animations
    int animating = 1; // 1 - started stage, 2 - just died
    float scoreAtWin = 0; // To keep track of the actual "score gain" when the player goes to next level
    // These are used to control the amount of time the animations run:
    float timeOfDeath = 0; // How many seconds the death animation has been running
    float timeOfLife = 0; // How many seconds the "being born" animation has been running
    float timeOfPhaseOut = 0; // How many seconds the "getting out of level" animation has been running

    // Enemy
    int count = 0;
    char directionEnemyMove = level.direction;
    int positionEnemyDead; // The dead enemy in this frame
    float numberToSin = 0; // If the direction is 'S', this number will be used to move in Y coordinate as sin function.

    // Text for score
    char scoreString[7];
    itoa(score, scoreString, 10);
    sfText* textForScore;
    textForScore = Utility_TextCreate(scoreString, "Quantify Bold v2.6.ttf", 40, sfColor_fromRGB(255, 255, 255), (sfVector2f){650, 550});

    // Energy Bar
    energy = 0;

    // Checking if level's direction is 'B'
    if(directionEnemyMove == 'B')
        isB = 1;

    // Initializing position
    sfSprite_setPosition(gameObjects.ship.shipSprite, (sfVector2f){WIDTH/2, 450});

    /// Loop of the layout
    // The shouldLoop is off when the stage ends or repeats, that is, when the player dies, or when he wins
    while(shouldLoop)
    {
        // Code to close the window
        while(sfRenderWindow_pollEvent(window, &event))
        {
            // Close window : exit
            if (event.type == sfEvtClosed)
            {
                sfRenderWindow_close(window);
                exit(0);
            }
        }

        /// Logic of the layout

        /// Pause Logic
        // Makes sure you don't pause and unpause every frame when the button is pressed
        if(sfKeyboard_isKeyPressed(sfKeyP) && sfRenderWindow_hasFocus(window))
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

        /// Cheats logic
        // Makes sure you don't turn on and turn off every frame when the button is pressed
        if(sfKeyboard_isKeyPressed(sfKeyI) && sfRenderWindow_hasFocus(window))
        {
            if(!iPressedLastTime)
                invencibilityOn = !invencibilityOn;

            if(invencibilityOn)
                printf("Invencibility ON\n");
            else
                printf("Invencibility OFF\n");

            iPressedLastTime = 1;
        }
        else
            iPressedLastTime = 0;

        if(!level.paused)
        {
            ///Player Interactions
            // They gotta be inside this condition here, so the player can't interact while it's paused.

            // Ship - Player movement
            if((sfKeyboard_isKeyPressed(sfKeyLeft)||sfKeyboard_isKeyPressed(sfKeyA)) && sfSprite_getPosition(gameObjects.ship.shipSprite).x > 40 && sfRenderWindow_hasFocus(window))
                sfSprite_move(gameObjects.ship.shipSprite, (sfVector2f){-300*dtime, 0});
            if((sfKeyboard_isKeyPressed(sfKeyRight)||sfKeyboard_isKeyPressed(sfKeyD)) && sfSprite_getPosition(gameObjects.ship.shipSprite).x < 760 && sfRenderWindow_hasFocus(window))
                sfSprite_move(gameObjects.ship.shipSprite, (sfVector2f){300*dtime, 0});

            // Action of firing the blast
            if(sfKeyboard_isKeyPressed(sfKeySpace) && isFireable && sfRenderWindow_hasFocus(window))
            {
                sfSprite_setPosition(gameObjects.fire, (sfVector2f){sfSprite_getPosition(gameObjects.ship.shipSprite).x, sfSprite_getPosition(gameObjects.ship.shipSprite).y - 40});
            }
        }

        // Fire - moves the blast
        if(!outOfScreen(gameObjects.fire, HEIGHT, WIDTH))
        {
            sfSprite_setPosition(gameObjects.fire, (sfVector2f){sfSprite_getPosition(gameObjects.ship.shipSprite).x, sfSprite_getPosition(gameObjects.fire).y -600*dtime});
            isFireable = 0;
        }
        else
            isFireable = 1;

        // Fire - check collisions
        positionEnemyDead = Enemies_isAtSamePoint(gameObjects.enemies, nEnemies, gameObjects.fire); // PositionEnemyDead will update every frame

        if(positionEnemyDead != -1)
        {
            gameObjects.enemies[positionEnemyDead].isAlive = 0;    // Killing the enemy
            sfSprite_setPosition(gameObjects.fire, (sfVector2f){-40, -40}); // Setting another position to fire
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

        Enemies_Move(level, gameObjects.enemies, nEnemies, dtime, &numberToSin);

        // Enemy fire
        // Did the time come for some enemy to fire?
        // No enemy can shoot if the player is being animated
        if(level.lastShot >= TIME_TO_FALL/level.levelSpeed && !animating)
        {
                Enemies_Shooting(gameObjects.enemies, nEnemies, liveEnemies, level.levelSpeed);
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
            if(energy <= 0 && !invencibilityOn)
            {
                numberlifes--;  // -1 life ;-;
                animating = 2;
            }

            if(!invencibilityOn)
                energy -= BARSPEED*dtime; // To empty the life bar
            sfRectangleShape_setSize(gameObjects.fillLifeBar2, (sfVector2f){energy, ENERGYY});

            if(Enemies_MovingFires(ENEMYFIRE_SPEED, gameObjects.enemies, nEnemies, dtime, gameObjects.ship) && !invencibilityOn)
                energy = 0;
        }
        /// Animation
        // That is, animation of the energy bar and ship when it dies or when it goes to a new level
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
            Enemies_destroyFires(gameObjects.enemies, nEnemies); ///TODO: define this
            sfSprite_setPosition(gameObjects.fire, (sfVector2f) {-40,-40});

            // If level has just started - "being born"
            if(animating == 1)
            {

                energy += ENERGYMAX*dtime;
                if(energy > ENERGYMAX)
                    energy = ENERGYMAX;
                sfRectangleShape_setSize(gameObjects.fillLifeBar2, (sfVector2f){energy, ENERGYY});

                // Plays the animation for 1 second
                if(timeOfLife < 1)
                {
                    sfSprite_setColor(gameObjects.ship.shipSprite, sfColor_fromRGBA(255,255,255, timeOfLife*255));
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
                    sfSprite_setColor(gameObjects.ship.shipSprite, sfColor_fromRGBA(255,(1-timeOfDeath)*255, (1-timeOfDeath)*255, (1-timeOfDeath)*255));
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
                        sfSprite_setPosition(gameObjects.ship.shipSprite, (sfVector2f) {WIDTH/2, 450});
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
                sfRectangleShape_setSize(gameObjects.fillLifeBar2, (sfVector2f){energy, ENERGYY});

                // If you have full energy, the animation takes 1 second. If not, it takes a fraction of a second
                if(timeOfPhaseOut < energy/ENERGYMAX)
                {
                    // Some math to figure out the ship's alpha value, so that it ends when the energy bar is emptied.
                    sfSprite_setColor(gameObjects.ship.shipSprite, sfColor_fromRGBA(255,255,255, (1-(ENERGYMAX*timeOfPhaseOut/energy))*255));
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
        sfRenderWindow_drawSprite(window, gameObjects.background, NULL);
        sfRenderWindow_drawSprite(window, gameObjects.ship.shipSprite, NULL);
        sfRenderWindow_drawSprite(window, gameObjects.fire, NULL);
        Enemies_DrawFires(window, gameObjects.enemyFire, gameObjects.enemies, nEnemies);
        sfRenderWindow_drawRectangleShape(window, gameObjects.base, NULL);
        drawLifes(window, gameObjects.life, &numberlifes);
        sfRenderWindow_drawSprite(window, gameObjects.lifebar, NULL);
        sfRenderWindow_drawRectangleShape(window, gameObjects.fillLifeBar, NULL);
        sfRenderWindow_drawRectangleShape(window, gameObjects.fillLifeBar2, NULL);
        Enemies_Draw(window, gameObjects.enemies, nEnemies, gameObjects);
        sfRenderWindow_drawText(window, textForScore, NULL);
        sfRenderWindow_display(window);
    }
}

void Layout_GameOver(sfRenderWindow* window, sfEvent event)
{
    sfText* scoreLayout;
    sfText* scoreNb;

    /// Creating texts
    // S C O R E
    scoreLayout = Score_CreateLayout((sfVector2f){WIDTH/2, HEIGHT/6}, 40);
    // Score numbers
    scoreNb = Score_TextCreate(score, (sfVector2f){WIDTH/2, HEIGHT/4}, 30);

    do
    {
        /// Code to close the window
        while(sfRenderWindow_pollEvent(window, &event))
        {
            // Close window : exit
            if (event.type == sfEvtClosed)
            {
                sfRenderWindow_close(window);
                exit(0);
            }
        }

        sfSleep(sfMilliseconds(10));
        sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
        sfRenderWindow_drawSprite(window, gameObjects.gameover, NULL);
        sfRenderWindow_drawText(window, scoreLayout, NULL);
        sfRenderWindow_drawText(window, scoreNb, NULL);
        sfRenderWindow_display(window);
    } while(!(sfMouse_isButtonPressed(sfMouseLeft) && sfRenderWindow_hasFocus(window) && sfMouse_getPosition(window).y > 0));
    //Mouse position Y is used to drag the window when the gameover screen is on.

    // Setting the enemies to the first level (if you reach Layout_GameOver, you've died 3 times)
    Enemies_Set(&levelbuffer, 1, gameObjects.enemies, &nEnemies, &liveEnemies);
    score = 0;
    numberlifes = 3;
}

void Layout_GameMenu(sfRenderWindow* window)
{
    int i;
    int flagButton = -1; // 0 - Play, 1 - Options, 2 - Credits
    int gameoverFlag = 0; // If it's 0, you don't want to play again after the game over screen. If it's 1, you want.
    int stillGameLevels = 1; // If it's 1, there are more levels to play. Else, it's 0.

    FILE *highscores;
    int positionScore = 0; // If it's 0, your score didn't enter in the highscores
    sfEvent event;

    TYPE_MENU patternMenu;

    // Initializing megamania logo
    patternMenu.megamaniaLogo = Utility_TextCreate("M E G A M A N I A", "Quantify Bold v2.6.ttf", 60, sfColor_fromRGB(255,255,255), (sfVector2f){WIDTH/2, HEIGHT/6});

    // Initializing play button
    patternMenu.buttons[0] = Utility_CreateButton("P L A Y", 40, (sfVector2f){WIDTH/2, 320}, (sfVector2f){BUTTON_WIDTH, BUTTON_HEIGHT}, sfColor_fromRGB( 18, 16, 18));

    // Initializing option button
    patternMenu.buttons[1] = Utility_CreateButton("H I G H S C O R E S", 40, (sfVector2f){WIDTH/2, 430}, (sfVector2f){BUTTON_WIDTH, BUTTON_HEIGHT}, sfColor_fromRGB( 18, 16, 18));

    // Initializing credits button
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
                        // Initializing flag
                        stillGameLevels = 1;
                        do
                        {
                            i = 1;
                            while(numberlifes > 0 && stillGameLevels)
                            {
                                // Setting the Level 1's enemies
                                stillGameLevels = Enemies_Set(&levelbuffer, i, gameObjects.enemies, &nEnemies, &liveEnemies);

                                if(stillGameLevels)
                                {
                                    // Beginning the Level 1
                                    Layout_Stage(window, levelbuffer);

                                    i++;
                                }
                            }

                            gameoverFlag = 0; // Making possible to enter in a game over, and then you finish the levels

                            if(numberlifes <= 0) // It means that the player dead 3 times, then, he can back and play again the first level
                            {
                                // Show Game Over screen
                                Layout_GameOver(window, event); // Here, you can click and you will starts from beginning again
                                gameoverFlag = 1;
                                // Setting the enemies to beginning definitions
                                liveEnemies = nEnemies;
                            }
                        }while(gameoverFlag);

                        // Verifying if there is a new highscore
                        positionScore = Score_AddHighScore(highscores, score);

                        // Winner screen
                        Layout_JustWon(window, gameObjects.background, positionScore);

                        score = 0;
                        flagButton = -1;
                        break;

            case 1:     Layout_Highscores(window, gameObjects.background);
                        flagButton = -1;
                        break;

            case 2:     Layout_Credits(window, gameObjects.background);
                        flagButton = -1;
                        break;

            default:    break;
        }

        /// Drawing on the screen
        sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
        // Background
        sfRenderWindow_drawSprite(window, gameObjects.menuBackground, NULL);
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

void Layout_Highscores(sfRenderWindow* window, sfSprite* background)
{
    int i; // Count
    int flagButton = 0;

    FILE *highscores;

    sfText* scoreSign;
    sfText* scoreText1;
    sfText* scoreText2;
    sfText* scoreText3;
    sfText* scoreText4;
    sfText* scoreText5;
    char strings[5][30];
    char positions[5][30];

    sfEvent event;

    TYPE_BUTTON backButton;

    for(i = 0; i < 5; i++)
    {
        itoa(i+1, positions[i], 10);
        strcat(positions[i], " -  ");
    }

    highscores = fopen("highscores.txt", "r");

    if(!highscores)
        printf("Open highscores.txt error!\n");
    else
    {
        // Setting the back button
        backButton = Utility_CreateButton("B A C K", 40, (sfVector2f){WIDTH - 120, 500}, (sfVector2f){200, 100}, sfColor_fromRGB(18, 16, 18));
        // S C O R E
        scoreSign = Utility_TextCreate("H I G H S C O R E", "Quantify Bold v2.6.ttf", 40, sfColor_fromRGB(255, 255, 255), (sfVector2f){WIDTH/2, HEIGHT/6});
        // Highscores
        for(i = 0; i < 5; i++)
        {
            fscanf(highscores, "%s", strings[i]);
            strcat(positions[i], strings[i]);
        }
        scoreText1 = Utility_TextCreate(positions[0], "Quantify Bold v2.6.ttf", 30, sfColor_fromRGB(255, 255, 255), (sfVector2f){WIDTH/2, HEIGHT/3});
        scoreText2 = Utility_TextCreate(positions[1], "Quantify Bold v2.6.ttf", 30, sfColor_fromRGB(255, 255, 255), (sfVector2f){WIDTH/2, HEIGHT/3 + 50});
        scoreText3 = Utility_TextCreate(positions[2], "Quantify Bold v2.6.ttf", 30, sfColor_fromRGB(255, 255, 255), (sfVector2f){WIDTH/2, HEIGHT/3 + 100});
        scoreText4 = Utility_TextCreate(positions[3], "Quantify Bold v2.6.ttf", 30, sfColor_fromRGB(255, 255, 255), (sfVector2f){WIDTH/2, HEIGHT/3 + 150});
        scoreText5 = Utility_TextCreate(positions[4], "Quantify Bold v2.6.ttf", 30, sfColor_fromRGB(255, 255, 255), (sfVector2f){WIDTH/2, HEIGHT/3 + 200});

        fclose(highscores);

        // Displaying the window
        do
        {
                /// Code to close the window
            while(sfRenderWindow_pollEvent(window, &event))
            {
                // Close window : exit
                if (event.type == sfEvtClosed)
                {
                    sfRenderWindow_close(window);
                    exit(0);
                }
            }

            /// Code to check if mouse is on the button
            flagButton = Utility_isOnButton(&backButton, window);

            /// Drawing
            sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
            // Background
            sfRenderWindow_drawSprite(window, background, NULL);
            // S C O R E
            sfRenderWindow_drawText(window, scoreSign, NULL);
            // Highscores
            sfRenderWindow_drawText(window, scoreText1, NULL);
            sfRenderWindow_drawText(window, scoreText2, NULL);
            sfRenderWindow_drawText(window, scoreText3, NULL);
            sfRenderWindow_drawText(window, scoreText4, NULL);
            sfRenderWindow_drawText(window, scoreText5, NULL);
            // Back button
            sfRenderWindow_drawRectangleShape(window, backButton.base, NULL);
            sfRenderWindow_drawText(window, backButton.text, NULL);
            sfRenderWindow_display(window);
        }while(!flagButton);
    }
}

void Layout_Credits(sfRenderWindow* window, sfSprite* background)
{
    sfEvent event;

    TYPE_BUTTON backButton; // "Back to menu" button
    int flagButton = 0; // If it's 0, you didn't click on the button. Else, you did.

    sfText* creators;   // It talks about who creates this game
    sfText* spritesPack;    // Where we pick these sprites
    sfText* why;        // Why we did it

    /// Setting texts
        // Set creators
    creators = Utility_TextCreate("C R E A T O R S: \n Marcos Samuel Landi\n Henry Bernardo K. de Avila.", "Quantify Bold v2.6.ttf",
                           20, sfColor_fromRGB(255,255,255), (sfVector2f){WIDTH*3/8, 200});

        // Set spritesPack
    spritesPack = Utility_TextCreate("S P R I T E ' S  P A C K :\n Space Shooter (Redux, plus fonts and sounds)\n by Kenney Vleugels (www.kenney.nl).", "Quantify Bold v2.6.ttf",
                           20, sfColor_fromRGB(255,255,255), (sfVector2f){(WIDTH*3/8 + 82), 300});

        // Set why
    why = Utility_TextCreate("W H Y  H A V E  Y O U  D O N E  T H I S ?\n Well, this game is the final project (coursework)\n of the UFRGS' Programming and Algorythms course.", "Quantify Bold v2.6.ttf",
                           20, sfColor_fromRGB(255,255,255), (sfVector2f){(WIDTH*3/8 + 110.5), 400});

    // Setting back's button
    backButton = Utility_CreateButton("B A C K", 40, (sfVector2f){WIDTH - 200, 100}, (sfVector2f){200, 100}, sfColor_fromRGB(18, 16, 18));

    do
    {
        /// Code to close the window
        while(sfRenderWindow_pollEvent(window, &event))
        {
            // Close window : exit
            if (event.type == sfEvtClosed)
            {
                sfRenderWindow_close(window);
                exit(0);
            }
        }

        flagButton = Utility_isOnButton(&backButton, window);

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

void Layout_JustWon(sfRenderWindow* window, sfSprite* background, int flagRank)
{
    sfEvent event;

    TYPE_BUTTON backButton; // "Back to menu" button
    int flagButton;

    sfText* scoreLayout;
    sfText* scoreNb;
    sfText* congratulations;
    char rankCongrat[50];
    char nbrRank[20];

    // Resetting lives for the next playthrough
    numberlifes = 3;

    // Creating congratulations string to put in the text
    strcpy(rankCongrat, "Congratulations!! You have the ");
    itoa(flagRank, nbrRank, 10);
    strcat(nbrRank, " highest score.");
    strcat(rankCongrat, nbrRank);

    // S C O R E
    scoreLayout = Score_CreateLayout((sfVector2f){WIDTH/2, HEIGHT/6}, 40);
    // Score number
    scoreNb = Score_TextCreate(score, (sfVector2f){WIDTH/2, HEIGHT/4}, 30);

    // Create congratulations text
    if(flagRank)
    {
        congratulations = Utility_TextCreate(rankCongrat, "Quantify Bold v2.6.ttf", 30, sfColor_fromRGB(255,255,255), (sfVector2f){WIDTH/2, HEIGHT/2});
    }

    // Setting back's button
    backButton = Utility_CreateButton("B A C K  T O  M E N U", 40, (sfVector2f){WIDTH - 210, HEIGHT - 100}, (sfVector2f){400, 100}, sfColor_fromRGB(18, 16, 18));

    do
    {
        /// Code to close the window
        while(sfRenderWindow_pollEvent(window, &event))
        {
            // Close window : exit
            if (event.type == sfEvtClosed)
            {
                sfRenderWindow_close(window);
                exit(0);
            }
        }

        // Checking if mouse is on button
        flagButton = Utility_isOnButton(&backButton, window);

        /// Drawing on the screen
        sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
            // Background
        sfRenderWindow_drawSprite(window, background, NULL);
            // Back button
        sfRenderWindow_drawRectangleShape(window, backButton.base, NULL);
        sfRenderWindow_drawText(window, backButton.text, NULL);
            //S C O R E
        sfRenderWindow_drawText(window, scoreLayout, NULL);
            //Score
        sfRenderWindow_drawText(window, scoreNb, NULL);
        if(flagRank)
            sfRenderWindow_drawText(window, congratulations, NULL);
        sfRenderWindow_display(window);
    }while(!flagButton);
}
