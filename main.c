#define MIDDLE 1
#include <SFML/Graphics.h>
#include <string.h>

//Defining some useful functions
//This bundles together a sprite with a texture, and the origin in the middle
sfSprite* sfSprite_createFromFile(const char* filename);

int main()
{
    //Declaring some variables
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow* window;
    sfSprite* ship;
    sfSprite* fire;
    sfSprite* background;
    //Time's variables
    sfClock* clock = sfClock_create();
    sfTime time = sfClock_getElapsedTime(clock);
    sfTime lasttime = time;
    float dtime;

    sfEvent event;
    int mouseX, mouseY;
    int isFireable = 1; //Fire flag

    //Create the main window
    window = sfRenderWindow_create(mode, "Megamania", sfResize | sfClose, NULL);

    ///Loads some sprites
    //Ship
    ship = sfSprite_createFromFile("nave.png");
    sfSprite_scale(ship, (sfVector2f){0.25,0.25}); //nave.png width: 80px
    sfSprite_setPosition(ship, (sfVector2f){400, 500});
    //Fire
    fire = sfSprite_createFromFile("fire.png");
    sfSprite_scale(fire, (sfVector2f){0.05,0.05});
    sfSprite_setPosition(fire, (sfVector2f){-40, -40});
    //Background
    background = sfSprite_createFromFile("background.png");
    sfSprite_scale(background, (sfVector2f){1, 1});
    sfSprite_setPosition(background, (sfVector2f){300, 300});

    ///Start the game loop
    while (sfRenderWindow_isOpen(window))
    {
        ///Process events
        while (sfRenderWindow_pollEvent(window, &event))
        {
            //Close window : exit
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        ///Update logicaa
        time = sfClock_getElapsedTime(clock);
        dtime = sfTime_asSeconds(time)-sfTime_asSeconds(lasttime);
        lasttime = time;

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
            sfSprite_move(fire, (sfVector2f){0, -600*dtime});
            isFireable = 0;
        }
        else
            isFireable = 1;

        ///Actual drawing
        sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
        sfRenderWindow_drawSprite(window, background, NULL);
        sfRenderWindow_drawSprite(window, ship, NULL);
        sfRenderWindow_drawSprite(window, fire, NULL);
        sfRenderWindow_display(window);
    }

    //Cleanup resources
    sfSprite_destroy(ship);
    sfSprite_destroy(fire);
    sfRenderWindow_destroy(window);

    return 0;
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
