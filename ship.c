#include "ship.h"
#include <math.h>
#include <stdlib.h>

#define PLAYER_INVUL_TIME 2.0f
#define PLAYER_RESPAWN_TIME 3.0f
#define PLAYER_TURN_SPEED 4.0f
#define PLAYER_THRUST 0.1f
#define PLAYER_DRAG 0.99f
#define PLAYER_MAX_SPEED 8.0f


static float RandFloat(float min, float max) {
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}


static void WrapPosition(Vector2 *pos) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    if (pos->x < 0) pos->x += sw;
    if (pos->x > sw) pos->x -= sw;
    if (pos->y < 0) pos->y += sh;
    if (pos->y > sh) pos->y -= sh;
}


void Player_Init(Player *p) {
    p->position     = (Vector2){ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
    p->velocity     = (Vector2){ 0.0f, 0.0f };
    p->rotation     = -90.f;;
    p->invulTimer   = PLAYER_INVUL_TIME;
    p->respawnTimer = 0.0f;
    p->alive        = true;
    p->thrusting    = false;
}


void Player_Kill(Player *p) {
    p->alive        = false;
    p->thrusting    = false;
    p->velocity     = (Vector2){ 0.0f, 0.0f };
    p->respawnTimer = PLAYER_RESPAWN_TIME;
}


void Player_Respawn(Player *p) {
    p->position   = (Vector2){ GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f };
    p->velocity   = (Vector2){ 0.0f, 0.0f };
    p->rotation   = 0.0f;
    p->alive      = true;
    p->invulTimer = PLAYER_INVUL_TIME;
}


void Player_Update(Player *p) {

    if (!p->alive) {
        p->respawnTimer -= 0.0166f;
        if (p->respawnTimer <= 0.0f) Player_Respawn(p);
        return;
    }


    if (p->invulTimer > 0.0f) p->invulTimer -= 0.0166f;


    if (IsKeyDown(KEY_LEFT)) p->rotation -= PLAYER_TURN_SPEED;
    if (IsKeyDown(KEY_RIGHT)) p->rotation += PLAYER_TURN_SPEED;


    p->thrusting = IsKeyDown(KEY_UP);
    if (p->thrusting) {
        float rad = p->rotation * (float)(3.14159265f / 180.0f);
        Vector2 dir = { cosf(rad), sinf(rad) }; /*kendi ekseni etrafında dönmesi*/
        p->velocity.x += dir.x * PLAYER_THRUST;
        p->velocity.y += dir.y * PLAYER_THRUST;
    }
     p->thrusting = IsKeyDown(KEY_UP);
    if (p->thrusting) {
        float rad = p->rotation * (float)(3.14159265f / 180.0f);
        Vector2 dir = { cosf(rad), sinf(rad) };
        p->velocity.x += dir.x * PLAYER_THRUST;
        p->velocity.y += dir.y * PLAYER_THRUST;
    }

    p->velocity.x *= PLAYER_DRAG;
    p->velocity.y *= PLAYER_DRAG;


    p->position.x += p->velocity.x;
    p->position.y += p->velocity.y;




    p->position.x += p->velocity.x;
    p->position.y += p->velocity.y;

    WrapPosition(&p->position);
}


void Player_Draw(const Player *p) {
    if (!p->alive) return;


    if (p->invulTimer > 0.0f) {
        if (((int)(p->invulTimer * 10.0f)) % 2 == 0) return;
    }


    Vector2 local[3] = {
        {  16.0f, 0.0f },
        { -10.0f, -10.0f },
        { -10.0f, 10.0f }
    };


    float rad = p->rotation * (float)(3.14159265f / 180.0f);
    float cs = cosf(rad), sn = sinf(rad);


    Vector2 world[3];
    for (int i = 0; i < 3; i++) {
        world[i].x = p->position.x + (local[i].x * cs - local[i].y * sn);
        world[i].y = p->position.y + (local[i].x * sn + local[i].y * cs);
    }


    DrawLineV(world[0], world[1], RAYWHITE);
    DrawLineV(world[1], world[2], RAYWHITE);
    DrawLineV(world[2], world[0], RAYWHITE);


}

