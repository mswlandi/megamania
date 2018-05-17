#include <stdio.h>
#include <SFML/Graphics.h>

int layout()
{
    sfRectangleShape* base = sfRectangleShape_create();
    sfRectangleShape_scale(base, (sfVector2f){800, 200});
    sfRectangleShape_setOrigin(base, (sfVector2f){400, 400});

}

