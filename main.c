//im zuerinh

#define MIDDLE 1
#include <SFML/Graphics.h>

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
    sfEvent event;
    int mouseX, mouseY;

    //Create the main window
    window = sfRenderWindow_create(mode, "Megamania", sfResize | sfClose, NULL);

    //Load the some sprites
    ship = sfSprite_createFromFile("nave.png");
    sfSprite_scale(ship, (sfVector2f){0.25,0.25});
    fire = sfSprite_createFromFile("fire.png");
    sfSprite_scale(fire, (sfVector2f){0.1,0.1});
    sfSprite_rotate(fire,90);
    sfSprite_setPosition(ship, (sfVector2f){-1,-1});

    //Start the game loop
    while (sfRenderWindow_isOpen(window))
    {
        //Process events
        while (sfRenderWindow_pollEvent(window, &event))
        {
            //Close window : exit
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        //Update logic
        mouseX = sfMouse_getPosition(window).x;
        mouseY = sfMouse_getPosition(window).y;
        sfSprite_setPosition(ship, (sfVector2f){mouseX,500});

        //Actual drawing
        sfRenderWindow_clear(window, sfColor_fromRGB(0,0,0));
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

    texture = sfTexture_createFromFile(filename, NULL);
    sprite = sfSprite_create();

    sfSprite_setTexture(sprite, texture, sfTrue);
    sfSprite_setOrigin(sprite, (sfVector2f){sfSprite_getLocalBounds(sprite).width/2,
                                            sfSprite_getLocalBounds(sprite).height/2});

    return sprite;
}
