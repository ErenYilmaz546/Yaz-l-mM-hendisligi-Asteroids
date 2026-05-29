#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"

/* Oyuncu (gemi) verilerini tutan yapı */
typedef struct {
    Vector2 position;     /* Ekranda bulunduğu konum */
    Vector2 velocity;     /* İlerleme hızı ve yönü */
    float rotation;       /* Geminin baktığı açı */
    float invulTimer;     /* Dokunulmazlık sayacı */
    float respawnTimer;   /* Yeniden doğma sayacı */
    bool alive;           /* Yaşıyor mu? */
    bool thrusting;       /* Gaza basıyor mu? */
} Player;

/* Temel oyuncu fonksiyonları */
void Player_Init(Player *p);
void Player_Update(Player *p);
void Player_Draw(const Player *p);
void Player_Kill(Player *p);
void Player_Respawn(Player *p);

#endif
