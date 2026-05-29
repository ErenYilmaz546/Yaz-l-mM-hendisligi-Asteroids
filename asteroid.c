#include "asteroid.h"
#include <math.h>
#include <stdlib.h>
#define ASTEROID_LARGE_R 40.0f
#define ASTEROID_MEDIUM_R 20.0f
#define ASTEROID_SMALL_R 10.0f

// Rastgele ondalıklı sayı üretici
static float RandFloat(float min, float max) {
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

// Sınırları saran yardımcı fonksiyon
static void WrapPosition(Vector2 *pos) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();
    if (pos->x < 0) pos->x += sw;
    if (pos->x > sw) pos->x -= sw;
    if (pos->y < 0) pos->y += sh;
    if (pos->y > sh) pos->y -= sh;
}

// İki nokta arasındaki uzaklığın karesini alır (Çarpışma hesabı için)
static float DistanceSq(Vector2 p1, Vector2 p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

// Boyut tipini yarıçap sayısına dönüştürür
static float SizeToRadius(AsteroidSize s) {
    switch (s) {
        case AST_LARGE:  return ASTEROID_LARGE_R;
        case AST_MEDIUM: return ASTEROID_MEDIUM_R;
        case AST_SMALL:  return ASTEROID_SMALL_R;
    }
    return ASTEROID_LARGE_R;
}

// Dizideki ilk boş (aktif olmayan) yeri bulur
static int FindFreeSlot(Asteroid asteroids[MAX_ASTEROIDS]) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active) return i;
    }
    return -1;
}

// Başlangıçta tüm asteroitleri pasif yapar
void Asteroids_Init(Asteroid asteroids[MAX_ASTEROIDS]) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        asteroids[i].active = false;
    }
}

// Yeni bir asteroit oluşturur ve rastgele hız/dönüş atar
void Asteroids_Spawn(Asteroid asteroids[MAX_ASTEROIDS], Vector2 pos, AsteroidSize size) {
    int idx = FindFreeSlot(asteroids);
    if (idx < 0) return; /* Slot kalmadıysa iptal et */

    Asteroid *a = &asteroids[idx];
    a->position = pos;
    a->size     = size;
    a->radius   = SizeToRadius(size);

    float angle = RandFloat(0.0f, 360.0f);
    float speed = RandFloat(1.0f, 2.5f);

    /* Küçük olanlar daha hızlı uçar */
    if (size == AST_MEDIUM) speed *= 1.3f;
    if (size == AST_SMALL)  speed *= 1.7f;

    float rad = angle * (float)(3.14159265f / 180.0f);
    a->velocity.x = cosf(rad) * speed;
    a->velocity.y = sinf(rad) * speed;

    a->rotation      = RandFloat(0.0f, 360.0f);
    a->rotationSpeed = RandFloat(-2.0f, 2.0f);

    // Taş görünümü vermek için köşeleri rastgele girintili yap
    a->vertexCount = 10 + (rand() % 3);
    if (a->vertexCount > 12) a->vertexCount = 12;

    for (int v = 0; v < a->vertexCount; v++) {
        a->offsets[v] = RandFloat(0.78f, 1.18f);
    }

    a->active = true;
}

// Yeni dalgada oyuncunun uzağında belirli sayıda büyük asteroit üretir
void Asteroids_SpawnWave(Asteroid asteroids[MAX_ASTEROIDS], int count, Vector2 avoidPos, float avoidRadius) {
    int placed = 0;
    int tries  = 0;
    while (placed < count && tries < count * 50) {
        tries++;
        Vector2 p = {
            RandFloat(0.0f, (float)GetScreenWidth()),
            RandFloat(0.0f, (float)GetScreenHeight())
        };
        /* Eğer oyuncunun dibindeyse atla, başka bir yer seç */
        if (DistanceSq(p, avoidPos) < avoidRadius * avoidRadius) continue;

        Asteroids_Spawn(asteroids, p, AST_LARGE);
        placed++;
    }
}

// Her karede konumu günceller ve sınırları kontrol eder
void Asteroids_Update(Asteroid asteroids[MAX_ASTEROIDS]) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active) continue;
        asteroids[i].position.x += asteroids[i].velocity.x;
        asteroids[i].position.y += asteroids[i].velocity.y;
        asteroids[i].rotation   += asteroids[i].rotationSpeed;
        WrapPosition(&asteroids[i].position);
    }
}

// Asteroidi çizgiler kullanarak poligonal şekilde ekrana çizer
void Asteroids_Draw(const Asteroid asteroids[MAX_ASTEROIDS]) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (!asteroids[i].active) continue;
        const Asteroid *a = &asteroids[i];

        for (int v = 0; v < a->vertexCount; v++) {
            float t1 =  (float)v        / (float)a->vertexCount;
            float t2 =  (float)(v + 1)  / (float)a->vertexCount;
            float ang1 = a->rotation + t1 * 360.0f;
            float ang2 = a->rotation + t2 * 360.0f;

            int v2 = (v + 1) % a->vertexCount;
            float r1 = a->radius * a->offsets[v];
            float r2 = a->radius * a->offsets[v2];

            float rad1 = ang1 * (float)(3.14159265f / 180.0f);
            float rad2 = ang2 * (float)(3.14159265f / 180.0f);

            Vector2 p1 = { a->position.x + cosf(rad1) * r1, a->position.y + sinf(rad1) * r1 };
            Vector2 p2 = { a->position.x + cosf(rad2) * r2, a->position.y + sinf(rad2) * r2 };

            DrawLineV(p1, p2, LIGHTGRAY);
        }
    }
}

// Vurulan asteroidi yok eder veya 2 küçük parçaya böler
void Asteroids_Break(Asteroid asteroids[MAX_ASTEROIDS], int index) {
    if (index < 0 || index >= MAX_ASTEROIDS) return;
    if (!asteroids[index].active) return;

    Vector2 pos      = asteroids[index].position;
    AsteroidSize sz  = asteroids[index].size;
    asteroids[index].active = false;

    // Parçalama boyutunu belirle
    AsteroidSize newSize;
    if      (sz == AST_LARGE)  newSize = AST_MEDIUM;
    else if (sz == AST_MEDIUM) newSize = AST_SMALL;
    else                       return; /* Küçükler doğrudan yok olur */

    // İki yeni alt parça spawn et
    for (int k = 0; k < 2; k++) {
        Vector2 off = {
            RandFloat(-6.0f, 6.0f),
            RandFloat(-6.0f, 6.0f)
        };
        Vector2 newPos = { pos.x + off.x, pos.y + off.y };
        Asteroids_Spawn(asteroids, newPos, newSize);
    }
}

// Ekranda kaç tane aktif asteroit kaldığını sayar (Dalga bitiş kontrolü için)
int Asteroids_CountActive(const Asteroid asteroids[MAX_ASTEROIDS]) {
    int n = 0;
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].active) n++;
    }
    return n;
}
