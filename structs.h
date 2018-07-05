#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

#include <SFML/Graphics.h>

/// Structs

// Struct for button
typedef struct str_button
{
    sfText* text;
    sfRectangleShape* base;
} TYPE_BUTTON;

// Struct for menu
typedef struct str_menu
{
    sfFont* font;
    sfText* megamaniaLogo;
    TYPE_BUTTON buttons[3]; // 0 - PLAY / 1 - OPTIONS / 2 - CREDITS
    sfSprite* menuBackground;
} TYPE_MENU;

// Struct for fire
typedef struct str_fire
{
    int flag; // If it's 1, this fire is enabled. Else, it's 0.
    float posX;
    float posY;
}TYPE_FIRE;

// Struct for enemy
typedef struct str_enemy
{
    float posX;
    float posY;
    sfVector2f initialPos;
    int color;
    int flag; // If this enemy is alive, it's 1.
    TYPE_FIRE fire;
} TYPE_ENEMIES;

// Struct for player ship
typedef struct str_playership
{
    sfSprite* shipSprite;
    float posX;
    float posY;
} TYPE_PLAYERSHIP;

// Struct that holds all of the game's sprites
typedef struct str_sprites
{
    TYPE_PLAYERSHIP ship;
    sfSprite* fire;
    sfSprite* enemyFire;
    sfSprite* background;
    sfSprite* life;
    sfSprite* lifebar;
    sfSprite* gameover;
    sfSprite* enemyBlack;
    sfSprite* enemyRed;
    sfSprite* enemyBlue;
    sfSprite* enemyGreen;
    sfRectangleShape* fillLifeBar;
    sfRectangleShape* fillLifeBar2;
    sfRectangleShape* base;
    TYPE_ENEMIES enemies[];
} TYPE_ALLSPRITES;

// Struct which holds all about the logic of level
typedef struct str_level
{
    char* mapName;
    int levelSpeed;
    char direction; // Can be 'R', to right, 'L', to left, or 'B', to both synchronously.
    int numberEnemies;
    int paused;
} TYPE_LEVEL;

#endif // STRUCTS_H_INCLUDED
