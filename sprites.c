// Video's constants
#define WIDTH 800
#define HEIGHT 600

#include "sprites.h"

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

 int sfSprite_CollisionPoint(sfSprite* sprite, sfVector2f point)
{
    int answer = 0;

    float spriteWidth = sfSprite_getLocalBounds(sprite).width;
    float spriteHeight = sfSprite_getLocalBounds(sprite).height;

    float posX = sfSprite_getPosition(sprite).x;
    float posY = sfSprite_getPosition(sprite).y;

    if(point.x >= (posX - spriteWidth/2.0) &&
       point.x <= (posX + spriteWidth/2.0) &&
       point.y >= (posY - spriteHeight/2.0)&&
       point.y <= (posY + spriteHeight/2.0))
       answer = 1;

    return answer;
}

int outOfScreen(sfSprite* sprite, float hScreen, float wScreen)
{
    int answer = 0;

    if(sfSprite_getPosition(sprite).x > wScreen || sfSprite_getPosition(sprite).x < 0 ||
       sfSprite_getPosition(sprite).y > hScreen || sfSprite_getPosition(sprite).y < -40)
        answer = 1;

    return answer;
}

void drawLifes(sfRenderWindow* window, sfSprite* life, int* numL)
{
    int i;

    for(i=0; i < *numL; i++)
    {
        sfSprite_setPosition(life,(sfVector2f){350 + 50*i, 575});       // The first life's position is ( 350, 575)
        sfRenderWindow_drawSprite(window, life, NULL);
    }
}
