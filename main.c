#include "raylib.h"
#include "game.h"
#include <stdlib.h>
#include <time.h>

int main(void) {

    srand((int)time(NULL));


    InitWindow(1000, 850, "Asteroids - Programlama II");
    SetTargetFPS(60);
    Game game;
    Game_Init(&game);


    while (!WindowShouldClose()) {

        Game_Update(&game);
        BeginDrawing();
        Game_Draw(&game);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
