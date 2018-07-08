#include "utility.h"

sfText* Text_Create(char* string, char* fontName, int charSize, sfColor color, sfVector2f position)
{
    sfText* text;
    sfFont* font;

    font = sfFont_createFromFile(fontName);

    text = sfText_create();
    sfText_setString(text, string);
    sfText_setFont(text, font);
    sfText_setColor(text, color);
    sfText_setCharacterSize(text, charSize);
    sfText_setOrigin(text, (sfVector2f){sfText_getLocalBounds(text).width/2, sfText_getLocalBounds(text).height/2});
    sfText_setPosition(text, position);

    return text;
}

int Utility_isOnButton(TYPE_BUTTON *button, sfRenderWindow* window)
{
    int flagButton = 0;

    if((sfMouse_getPosition(window).y <= (sfRectangleShape_getPosition(button->base).y + sfRectangleShape_getLocalBounds(button->base).height/2)
        && sfMouse_getPosition(window).y >= (sfRectangleShape_getPosition(button->base).y - sfRectangleShape_getLocalBounds(button->base).height/2))
        && (sfMouse_getPosition(window).x <= (sfRectangleShape_getPosition(button->base).x + sfRectangleShape_getLocalBounds(button->base).width/2)
        && sfMouse_getPosition(window).x >= (sfRectangleShape_getPosition(button->base).x - sfRectangleShape_getLocalBounds(button->base).width/2)))
        {
            sfRectangleShape_setOutlineColor(button->base, sfColor_fromRGB( 255, 255, 255));
            // Check click
            if(sfMouse_isButtonPressed(sfMouseLeft))
            {
                flagButton = 1;
            }
        }
        else
            sfRectangleShape_setOutlineColor(button->base, sfColor_fromRGB( 0, 0, 0));

    return flagButton;
}

TYPE_BUTTON Utility_CreateButton(char stringText[50], float textSize, sfVector2f position, sfVector2f baseSize, sfColor cBase)
{
    TYPE_BUTTON button;

        // Text of button
    button.text = Text_Create(stringText,"Quantify Bold v2.6.ttf", 30, sfColor_fromRGB(255,255,255), position);
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
