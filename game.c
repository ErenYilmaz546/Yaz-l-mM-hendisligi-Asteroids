#include "game.h"
#include <stdio.h>
#define START_LIVES 3
#define PLAYER_RADIUS 12.0f

/* İki obje arası çarpışma tespiti için mesafe karesi hesabı */
static float DistanceSq(Vector2 p1, Vector2 p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

/* Oyun ilk açıldığında değerleri sıfırlar */
void Game_Init(Game *g) {
    g->state    = STATE_MENU;
    g->score    = 0;
    g->hiScore  = 0;
    g->lives    = START_LIVES;
    g->wave     = 1;

    Player_Init(&g->player);
    Asteroids_Init(g->asteroids);
}

/* Oynanış başladığında can ve skoru yeniler */
void Game_StartNew(Game *g) {
    g->state = STATE_PLAYING;
    g->score = 0;
    g->lives = START_LIVES;
    g->wave  = 1;

    Player_Init(&g->player);
    Asteroids_Init(g->asteroids);

    /* 1. Dalga: 4 büyük asteroit yarat */
    Asteroids_SpawnWave(g->asteroids, 4, g->player.position, 180.0f);
}

/* Oyuncunun asteroitlere çarpıp çarpmadığını test eder */
static void CheckPlayerAsteroidCollisions(Game *g) {
    if (!g->player.alive) return;
    if (g->player.invulTimer > 0.0f) return; /* Dokunulmazken çarpışmaz */

    for (int a = 0; a < MAX_ASTEROIDS; a++) {
        if (!g->asteroids[a].active) continue;

        /* Oyuncunun yarıçapı ile asteroidin yarıçapı kesişiyorsa */
        float r = g->asteroids[a].radius + PLAYER_RADIUS;
        if (DistanceSq(g->player.position, g->asteroids[a].position) < r * r) {
            Player_Kill(&g->player);
            g->lives--;

            if (g->lives <= 0) {
                g->state = STATE_GAME_OVER;
            }
            return;
        }
    }
}


void Game_Update(Game *g) {

    if (g->state == STATE_MENU) {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            Game_StartNew(g);
        }
        return;
    }


    if (g->state == STATE_GAME_OVER) {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            Game_StartNew(g);
        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            g->state = STATE_MENU;
        }
        return;
    }

    /* Oynanış sırasında ESC basılırsa menüye dön */
    if (IsKeyPressed(KEY_ESCAPE)) {
        g->state = STATE_MENU;
        return;
    }

    /* Alt sistemleri güncelle */
    Player_Update   (&g->player);
    Asteroids_Update( g->asteroids);

    CheckPlayerAsteroidCollisions(g);

    /* Tüm asteroitler bitince yeni dalgayı başlat */
    if (Asteroids_CountActive(g->asteroids) == 0) {
        g->wave++;
    }
}

/* Arayüz yazılarını Skor, Can, Dalga */
static void DrawHud(const Game *g) {
    char buf[64];
    int sw = GetScreenWidth();

    snprintf(buf, sizeof(buf), "SKOR: %d", g->score);
    DrawText(buf, 20, 16, 22, RAYWHITE);

    snprintf(buf, sizeof(buf), "CAN : %d", g->lives);
    DrawText(buf, 20, 44, 22, RAYWHITE);

    snprintf(buf, sizeof(buf), "DALGA: %d", g->wave);
    int w = MeasureText(buf, 22);
    DrawText(buf, sw - w - 20, 16, 22, RAYWHITE);

    snprintf(buf, sizeof(buf), "High Score: %d", g->hiScore);
    w = MeasureText(buf, 22);
    DrawText(buf, sw - w - 20, 44, 22, GOLD);
}

/* Ekranda görünen her şeyin çizim komutları*/
void Game_Draw(const Game *g) {
    ClearBackground(BLACK);
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    /* Menü */
    if (g->state == STATE_MENU) {
        const char *title = "ASTEROIDS";
        int tw = MeasureText(title, 80);
        DrawText(title, (sw - tw) / 2, 180, 80, RAYWHITE);

        const char *sub = "Programlama II - Proje";
        int subw = MeasureText(sub, 24);
        DrawText(sub, (sw - subw) / 2, 280, 24, GRAY);

        const char *play = "BASLA: ENTER / SPACE";
        int pw = MeasureText(play, 26);
        DrawText(play, (sw - pw) / 2, 420, 26, YELLOW);

        const char *hint1 = "--Hareket yön tuslari ile yapilabilir--";
        const char *hint4 = "--Pause: ESC--";
        DrawText(hint1, sw/2 - 160, 490, 20, LIGHTGRAY);
        DrawText(hint4, sw/2 - 160, 550, 20, LIGHTGRAY);
        return;
    }

    /* Oyun içi obje çizimleri */
    Asteroids_Draw(g->asteroids);
    Player_Draw   (&g->player);

    DrawHud(g);

    /* Oyun Sonu ekranı çizimi */
    if (g->state == STATE_GAME_OVER) {


        const char *over = "OYUN BITTI";
        int ow = MeasureText(over, 72);
        DrawText(over, (sw - ow) / 2, 250, 72, RED);

        char buf[64];
        snprintf(buf, sizeof(buf), "Skor: %d", g->score);
        int scorew = MeasureText(buf, 32);
        DrawText(buf, (sw - scorew) / 2, 350, 32, RAYWHITE);

        const char *again = "Tekrar: ENTER    Menu: ESC";
        int aw = MeasureText(again, 24);
        DrawText(again, (sw - aw) / 2, 420, 24, YELLOW);
    }
}
