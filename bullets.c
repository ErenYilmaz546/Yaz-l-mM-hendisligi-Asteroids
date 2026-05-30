#include "bullets.h"
#include <math.h>


static void WrapPosition(Vector2 *pos) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    if (pos->x < 0) pos->x += sw;
    if (pos->x > sw) pos->x -= sw;
    if (pos->y < 0) pos->y += sh;
    if (pos->y > sh) pos->y -= sh;
}


void Bullets_Init(Bullet bullets[MAX_BULLETS]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        bullets[i].active = false;
    }
}


void Bullets_Spawn(Bullet bullets[MAX_BULLETS], Vector2 pos, float angleDeg) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) {

            float rad = angleDeg * (3.14159265f / 180.0f);
            Vector2 dir = { cosf(rad), sinf(rad) };

            bullets[i].position = pos;
            bullets[i].velocity.x = dir.x * BULLET_SPEED;
            bullets[i].velocity.y = dir.y * BULLET_SPEED;
            bullets[i].lifetime = BULLET_LIFETIME;
            bullets[i].active = true;
            return;
        }
    }
}


void Bullets_Update(Bullet bullets[MAX_BULLETS]) {
    float dt = 1.0f / 60.0f; // Sabit zaman adımı

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;


        bullets[i].lifetime -= dt;
        if (bullets[i].lifetime <= 0.0f) {
            bullets[i].active = false;
            continue;
        }


        bullets[i].position.x += bullets[i].velocity.x * dt;
        bullets[i].position.y += bullets[i].velocity.y * dt;


        WrapPosition(&bullets[i].position);
    }
}


void Bullets_Draw(const Bullet bullets[MAX_BULLETS]) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullets[i].active) continue;
        DrawCircleV(bullets[i].position, BULLET_RADIUS, RAYWHITE);
    }
}
