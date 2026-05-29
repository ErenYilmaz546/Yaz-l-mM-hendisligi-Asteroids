#include "raylib.h"
#include "game.h"
#include <stdlib.h>
#include <time.h>

int main(void) {
    /* Rastgele sayı üreticisi */
    srand((int)time(NULL));

    /* 60 FPS hedefine sahip pencereyi oluşturur */
    InitWindow(1000, 850, "Asteroids - Programlama II");
    SetTargetFPS(60);

    /* Oyun nesnesini oluştur ve başlangıç ayarlarını yap */
    Game game;
    Game_Init(&game);

    /* Pencere kapatılana kadar dönecek ana oyun döngüsü */
    while (!WindowShouldClose()) {
        /* Mantık güncellemeleri (Klavye, hareket, çarpışma) */
        Game_Update(&game);

        /* Çizim işlemleri */
        BeginDrawing();
        Game_Draw(&game);
        EndDrawing();
    }

    /* Belleği temizle ve çık */
    CloseWindow();
    return 0;
}
