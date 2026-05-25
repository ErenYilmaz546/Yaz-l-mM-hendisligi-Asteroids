#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
int main()
{
    //Pencere kurulumu
    const int screenWidth = 800;
    const int screenHeight = 600;

    //Pencere olusturma
    InitWindow(screenWidth, screenHeight, "Astreoids Projesi");

    //fps ayari

    SetTargetFPS(60);

    //ana oyun döngüsü
    //carpıya veya ESC'ye basılana kadar saniyede 60 kez döner

    while(!WindowShouldClose()){

    //--Güncelleme Kısmıı--

    //--Tus okumalarıve uzay gemisinin hareketi--

    //--Cizim Kısmı

    BeginDrawing();

    //Ekranı her kareden önce tamamen siyah renk ile temizle

    ClearBackground(BLACK);

    //Ekrana test yazısı yazdır

     DrawText("Asteroids Oyununa Hos Geldin!",220,280,20,GREEN);

     EndDrawing();
    }

    //3.Kapanış ve temizlik
    //Döngü bitince pencereyi kapat ve belleği boşalt

    CloseWindow();
    return 0;
}
