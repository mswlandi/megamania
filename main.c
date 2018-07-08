#include "global.h"
#include "utility.h"
#include "enemies.h"
#include "score.h"
#include "layout.h"
#include "sprites.h"

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
    Layout_GameMenu(window);

    // Cleanup resources
    sfSprite_destroy(gameSprites.ship.shipSprite);
    sfSprite_destroy(gameSprites.fire);
    sfRenderWindow_destroy(window);


    return 0;
}
