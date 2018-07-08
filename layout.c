#define WIDTH 800
#define HEIGHT 600

#include "layout.h"

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
    char strings[5][50];
    char positions[5][10];

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
        // Setting back's button
        backButton = Utility_CreateButton("B A C K", 40, (sfVector2f){WIDTH - 120, 500}, (sfVector2f){200, 100}, sfColor_fromRGB(18, 16, 18));
        // S C O R E
        scoreSign = Text_Create("H I G H S C O R E", "Quantify Bold v2.6.ttf", 40, sfColor_fromRGB(255, 255, 255), (sfVector2f){WIDTH/2, HEIGHT/6});
        // Highscores
        for(i = 0; i < 5; i++)
        {
            fscanf(highscores, "%s", strings[i]);
            strcat(positions[i], strings[i]);
        }
        scoreText1 = Text_Create(positions[0], "Quantify Bold v2.6.ttf", 30, sfColor_fromRGB(255, 255, 255), (sfVector2f){WIDTH/2, HEIGHT/3});
        scoreText2 = Text_Create(positions[1], "Quantify Bold v2.6.ttf", 30, sfColor_fromRGB(255, 255, 255), (sfVector2f){WIDTH/2, HEIGHT/3 + 50});
        scoreText3 = Text_Create(positions[2], "Quantify Bold v2.6.ttf", 30, sfColor_fromRGB(255, 255, 255), (sfVector2f){WIDTH/2, HEIGHT/3 + 100});
        scoreText4 = Text_Create(positions[3], "Quantify Bold v2.6.ttf", 30, sfColor_fromRGB(255, 255, 255), (sfVector2f){WIDTH/2, HEIGHT/3 + 150});
        scoreText5 = Text_Create(positions[4], "Quantify Bold v2.6.ttf", 30, sfColor_fromRGB(255, 255, 255), (sfVector2f){WIDTH/2, HEIGHT/3 + 200});


        fclose(highscores);

        // Displaying the window
        do
        {
                /// Code to close the window
            while(sfRenderWindow_pollEvent(window, &event))
            {
                // Close window : exit
                if (event.type == sfEvtClosed)
                    sfRenderWindow_close(window);
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

    TYPE_BUTTON backButton;
    int flagButton = 0; // If it's 0, you didn't click on the button. Else, you did.

    sfText* creators;   // It talks about who creates this game
    sfText* spritesPack;    // Where we pick these sprites
    sfText* why;        // Why we did it

    /// Setting texts
        // Set creators
    creators = Text_Create("C R E A T O R S: \nMarcos Samuel Landi\nHenry Bernardo K. de Avila.", "Quantify Bold v2.6.ttf",
                           20, sfColor_fromRGB(255,255,255), (sfVector2f){WIDTH*3/8, 200});

        // Set spritesPack
    spritesPack = Text_Create("S P R I T E ' S  P A C K :\n Space Shooter (Redux, plus fonts and sounds)\nby Kenney Vleugels (www.kenney.nl).", "Quantify Bold v2.6.ttf",
                           20, sfColor_fromRGB(255,255,255), (sfVector2f){WIDTH/2, 300});

        // Set why
    why = Text_Create("W H Y  H A V E  Y O U  D O N E  T H I S ?\n  Well, this game is the final project of\n Programming and Algorythms, a subject in UFRGS.", "Quantify Bold v2.6.ttf",
                           20, sfColor_fromRGB(255,255,255), (sfVector2f){WIDTH/2, 400});

    // Setting back's button
    backButton = Utility_CreateButton("B A C K", 40, (sfVector2f){WIDTH - 200, 100}, (sfVector2f){200, 100}, sfColor_fromRGB(18, 16, 18));

    do
    {
        /// Code to close the window
        while(sfRenderWindow_pollEvent(window, &event))
        {
            // Close window : exit
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
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

