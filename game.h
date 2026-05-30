#ifndef GAME_H
#define GAME_H
#include "ship.h"
#include "asteroid.h"
#include "bullets.h"

typedef enum { STATE_MENU, STATE_PLAYING, STATE_GAME_OVER } GameState;


typedef struct {
    GameState state;
    Player    player;
    Asteroid  asteroids[MAX_ASTEROIDS];
    Bullet    bullets[MAX_BULLETS];

    int       score;
    int       hiScore;
    int       lives;
    int       wave;
} Game;

void Game_Init(Game *g);
void Game_StartNew(Game *g);
void Game_Update(Game *g);
void Game_Draw(const Game *g);

#endif
