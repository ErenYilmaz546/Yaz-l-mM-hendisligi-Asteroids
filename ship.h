#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"


typedef struct {
    Vector2 position;     // Ekranda bulunduğu konum
    Vector2 velocity;     // İlerleme hızı ve yönü
    float rotation;       // Geminin baktığı açı
    float invulTimer;     // Dokunulmazlık
    float respawnTimer;   // Yeniden doğma
    bool alive;           // Yaşmak
    bool thrusting;       // Gaza basma
} Player;

void Player_Init(Player *p);
void Player_Update(Player *p);
void Player_Draw(const Player *p);
void Player_Kill(Player *p);
void Player_Respawn(Player *p);

#endif
