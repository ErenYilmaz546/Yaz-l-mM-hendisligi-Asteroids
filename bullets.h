#ifndef BULLETS_H_INCLUDED
#define BULLETS_H_INCLUDED

#include "raylib.h"
#include <stdbool.h>


#define MAX_BULLETS 32
#define BULLET_SPEED 500.0f
#define BULLET_LIFETIME 1.5f
#define BULLET_RADIUS 2.0f

typedef struct {
    Vector2 position;
    Vector2 velocity;
    float   lifetime;
    bool    active;
} Bullet;


void Bullets_Init(Bullet bullets[MAX_BULLETS]);
void Bullets_Spawn(Bullet bullets[MAX_BULLETS], Vector2 pos, float angleDeg);
void Bullets_Update(Bullet bullets[MAX_BULLETS]);
void Bullets_Draw(const Bullet bullets[MAX_BULLETS]);

#endif
