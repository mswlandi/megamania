#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

/// Constants
// Video's constants
#define WIDTH 800
#define HEIGHT 600
// Energy's bar constants
#define ENERGYMAX 436
#define ENERGYY 35
#define BARSPEED 10.9 // 10.9 - standard
// Enemies' constants
#define MAXENEMIES 20
#define DIST_ENEMY_X 10
#define DIST_ENEMY_Y 50
#define SPEED_ENEMY 200
#define ENEMYFIRE_SPEED 300
#define FRAMES_TO_DIR 2000
#define TIME_TO_FALL 1.65 // How much time, approximately, it takes for an enemy fire to reach the ground
// Button's constants
#define BUTTON_WIDTH 400
#define BUTTON_HEIGHT 100
// Levels' constants
#define MAX_LEVELS

// SFML headers
#include <SFML/Graphics.h>

// Default headers
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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
    sfSprite* menuBackground;
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
    float lastShot;
} TYPE_LEVEL;

/// Variables

// General variables
float energy;
int numberlifes;
int nEnemies;
int liveEnemies;
int score;

// Mouse variables
float mouseX;
float mouseY;

// Sprites
TYPE_ALLSPRITES gameSprites; // It's a global variable yet, but we want to put it in the main function, maybe


// Levels
TYPE_LEVEL level1;

TYPE_LEVEL level2;

TYPE_LEVEL chosenLevel; // We didn't use it yet

#endif // UTILITY_H_INCLUDED
