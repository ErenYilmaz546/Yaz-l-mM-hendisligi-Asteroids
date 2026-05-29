#include "ship.h"
#include <math.h>
#include <stdlib.h>

#define PLAYER_INVUL_TIME 2.0f
#define PLAYER_RESPAWN_TIME 3.0f
#define PLAYER_TURN_SPEED 4.0f
#define PLAYER_THRUST 0.1f
#define PLAYER_DRAG 0.99f
#define PLAYER_MAX_SPEED 8.0f

/* Rastgele ondalıklı sayı üretir */
static float RandFloat(float min, float max) {
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

/* Ekranın bir ucundan çıkan nesneyi diğer ucundan sokar */
static void WrapPosition(Vector2 *pos) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    if (pos->x < 0) pos->x += sw;
    if (pos->x > sw) pos->x -= sw;
    if (pos->y < 0) pos->y += sh;
    if (pos->y > sh) pos->y -= sh;
}

/* Oyuncuyu ekranın ortasına yerleştirir  değerleri sıfırlar */
void Player_Init(Player *p) {
    p->position     = (Vector2){ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
    p->velocity     = (Vector2){ 0.0f, 0.0f };
    p->rotation     = -90.f;;
    p->invulTimer   = PLAYER_INVUL_TIME;
    p->respawnTimer = 0.0f;
    p->alive        = true;
    p->thrusting    = false;
}

/* Oyuncuyu öldürür ve yeniden doğma sayacını başlatır */
void Player_Kill(Player *p) {
    p->alive        = false;
    p->thrusting    = false;
    p->velocity     = (Vector2){ 0.0f, 0.0f };
    p->respawnTimer = PLAYER_RESPAWN_TIME;
}

/* Oyuncuyu yeniden merkezde canlandırır */
void Player_Respawn(Player *p) {
    p->position   = (Vector2){ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
    p->velocity   = (Vector2){ 0.0f, 0.0f };
    p->rotation   = 0.0f;
    p->alive      = true;
    p->invulTimer = PLAYER_INVUL_TIME;
}

/* Her karede oyuncunun girişlerini ve fiziğini günceller */
void Player_Update(Player *p) {
    /* Ölü ise sayacı azalt ve bitince dirilt */
    if (!p->alive) {
        p->respawnTimer -= 0.0166f;
        if (p->respawnTimer <= 0.0f) Player_Respawn(p);
        return;
    }

    /* Dokunulmazlık sayacını azalt */
    if (p->invulTimer > 0.0f) p->invulTimer -= 0.0166f;

    /* Sağa/Sola dönüş işlemleri */
    if (IsKeyDown(KEY_LEFT)) p->rotation -= PLAYER_TURN_SPEED;
    if (IsKeyDown(KEY_RIGHT)) p->rotation += PLAYER_TURN_SPEED;

    /* İleri itki (Gaz verme) işlemleri */
    p->thrusting = IsKeyDown(KEY_UP);
    if (p->thrusting) {
        float rad = p->rotation * (float)(3.14159265f / 180.0f);
        Vector2 dir = { cosf(rad), sinf(rad) }; /*kendi ekseni etrafında dönmesi*/
        p->velocity.x += dir.x * PLAYER_THRUST;
        p->velocity.y += dir.y * PLAYER_THRUST;
    }


    /* Konumu hıza göre güncelle */
    p->position.x += p->velocity.x;
    p->position.y += p->velocity.y;

    /* Sınır kontrolü */
    WrapPosition(&p->position);
}

/* Gemiyi ekrana çizer */
void Player_Draw(const Player *p) {
    if (!p->alive) return;

    /* Dokunulmazlık süresinde yanıp sönme efekti */
    if (p->invulTimer > 0.0f) {
        if (((int)(p->invulTimer * 10.0f)) % 2 == 0) return;
    }

    /* Geminin 3 köşesi ( */
    Vector2 local[3] = {
        {  16.0f, 0.0f },
        { -10.0f, -10.0f },
        { -10.0f, 10.0f }
    };

    /* Açıyı radyana çevirir */
    float rad = p->rotation * (float)(3.14159265f / 180.0f);
    float cs = cosf(rad), sn = sinf(rad);

    /* Lokal koordinatları döndürerek dünya konumuna çevir */
    Vector2 world[3];
    for (int i = 0; i < 3; i++) {
        world[i].x = p->position.x + (local[i].x * cs - local[i].y * sn);
        world[i].y = p->position.y + (local[i].x * sn + local[i].y * cs);
    }

    /* Üçgenin kenarları */
    DrawLineV(world[0], world[1], RAYWHITE);
    DrawLineV(world[1], world[2], RAYWHITE);
    DrawLineV(world[2], world[0], RAYWHITE);


}
