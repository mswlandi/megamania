/*
    Este Programa foi feito para o nosso trabalho final da cadeira de Algoritmos e Programação da UFRGS, em 2018/1.
    É um clone do jogo Megamania, originalmente lançado para Atari 2600, utilizando a linguagem C e uma biblioteca gráfica.

    A lógica de execução do programa, junto com o resto da descrição do trabalho, está descrita no documento do trabalho
    sob o título "A Lógica do Jogo".

    Neste arquivo, junto com global.c, é onde são declaradas as estruturas e variáveis globais usadas no programa.
    Também é onde são definidas constantes e incluídas algumas bibliotecas padrão do C, utilizadas nos outros arquivos.

    Autores: Henry de Avila e Marcos Landi

    08/07/2018
*/
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
    int isOnScreen; // flag: whether it's on or not (on screen)
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
    int isAlive; // flag: whether or not it's alive
    TYPE_FIRE fire;
} TYPE_ENEMIES;

// Struct for player ship
typedef struct str_playership
{
    sfSprite* shipSprite;
    float posX;
    float posY;
} TYPE_PLAYERSHIP;

// Struct that holds all of the game's sprites and some game objects
typedef struct str_objects
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
} TYPE_GAMEOBJECTS;

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
TYPE_GAMEOBJECTS gameObjects;


// Levels
TYPE_LEVEL level1;

TYPE_LEVEL level2;

#endif // UTILITY_H_INCLUDED
