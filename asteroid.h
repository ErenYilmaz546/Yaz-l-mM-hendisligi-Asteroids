#ifndef ASTEROID_H
#define ASTEROID_H
#include "raylib.h"
#define MAX_ASTEROIDS 64

/* Asteroit boyut kategorileri */
typedef enum { AST_LARGE, AST_MEDIUM, AST_SMALL } AsteroidSize;

/* Asteroit verilerini tutan yapı */
typedef struct {
    Vector2 position;       /* Ekrandaki konumu */
    Vector2 velocity;       /* İlerleme hızı ve yönü */
    float rotation;         /* Mevcut dönüş açısı */
    float rotationSpeed;    /* Kendi etrafında dönme hızı */
    float radius;           /* Çarpışma yarıçapı */
    AsteroidSize size;      /* Büyüklük seviyesi */
    bool active;            /* Ekranda var mı? */
    int vertexCount;        /* Çizim için köşe sayısı */
    float offsets[12];      /* Çıkıntılı görünüm için köşe sapmaları */
} Asteroid;

/* asteroit dizisini sıfırlar */
void Asteroids_Init(Asteroid asteroids[MAX_ASTEROIDS]);
//yeni asteroid ekler
void Asteroids_Spawn(Asteroid asteroids[MAX_ASTEROIDS], Vector2 pos, AsteroidSize size);
//yeni astroid dalgası yaratır
void Asteroids_SpawnWave(Asteroid asteroids[MAX_ASTEROIDS], int count, Vector2 avoidPos, float avoidRadius);
//asterışd harekentini günceller
void Asteroids_Update(Asteroid asteroids[MAX_ASTEROIDS]);
//asteroid cizimini yaparr
void Asteroids_Draw(const Asteroid asteroids[MAX_ASTEROIDS]);
//vurulkan asteroidi daha kücük parcalara böler
void Asteroids_Break(Asteroid asteroids[MAX_ASTEROIDS], int index);
//aktif asteroit sayisini dömdürür
int  Asteroids_CountActive(const Asteroid asteroids[MAX_ASTEROIDS]);

#endif
