#include "global.h"
#include "utility.h"
#include "enemies.h"
#include "score.h"
#include "layout.h"
#include "sprites.h"

int main()
{
    // Loads all of the game's sprites
    loadgameObjects(&gameObjects);

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

    // Cleaning everything
    cleanGameStuff(&gameObjects, window);

    return 0;
}
