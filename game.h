#ifndef GAME_H
#define GAME_H
#include "ship.h"
#include "asteroid.h"

/* Oyunun mevcut durumları (Menü, Oynanış, Oyun Sonu) */
typedef enum { STATE_MENU, STATE_PLAYING, STATE_GAME_OVER } GameState;

/* Tüm oyun sistemini tek çatı altında toplayan ana yapı */
typedef struct {
    GameState state;
    Player    player;
    Asteroid  asteroids[MAX_ASTEROIDS];

    int       score;
    int       hiScore;
    int       lives;
    int       wave; /* Geçilen asteroit dalga sayısı */
} Game;

/* Ana oyun fonksiyonları */
void Game_Init(Game *g);
void Game_StartNew(Game *g);
void Game_Update(Game *g);
void Game_Draw(const Game *g);

#endif
