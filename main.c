#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include <math.h>

//mermş özellikleri

typedef struct Mermi{
    Vector2 position;
    Vector2 speed;
    bool aktifMi;
   }Mermi;







int main()
{
    //Pencere kurulumu
    const int screenWidth = 800;
    const int screenHeight = 600;

    //Pencere olusturma
    InitWindow(screenWidth, screenHeight, "Astreoids Projesi");

    //fps ayari
    SetTargetFPS(60);

    //ok imlecini gizleme (crosshair icin)
    HideCursor();

    //geminin baslangic konumu
     Vector2 shipPosition =
     {
       (float)screenWidth/2,
       (float)screenHeight/2};
         float shipSpeed = 5.0f;
    //Mermi Değiskeni
      Mermi lazer = {0};
      lazer.aktifMi = false;


    //ana oyun döngüsü
    //carpıya veya ESC'ye basılana kadar saniyede 60 kez döner
    while(!WindowShouldClose()){

    //--Güncelleme Kısmıı--
      if(IsKeyDown(KEY_RIGHT))
    shipPosition.x += shipSpeed;
      if(IsKeyDown(KEY_LEFT))
    shipPosition.x -= shipSpeed;
      if(IsKeyDown(KEY_DOWN))
    shipPosition.y += shipSpeed;
      if(IsKeyDown(KEY_UP))
    shipPosition.y -= shipSpeed;

    //mouseun anlik konumu
     Vector2 mousePosition = GetMousePosition();

    //Sol tıka basilirsa ve o an ekrada uçan baska mermi yoksa ateş et

      if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !lazer.aktifMi){
               lazer.aktifMi = true;
               lazer.position = shipPosition;
      }



    //--Cizim Kısmı

    BeginDrawing();

    //Ekranı her kareden önce tamamen siyah renk ile temizle

    ClearBackground(BLACK);

    //Ship sekli
       DrawTriangle(
        (Vector2){shipPosition.x,
                   shipPosition.y - 20},
        (Vector2){shipPosition.x - 15,
                   shipPosition.y + 15},
        (Vector2){shipPosition.x + 15,
                   shipPosition.y + 15},
                 RAYWHITE
                 );

    //crosshair cizimi
      DrawLine(mousePosition.x - 10,
               mousePosition.y,
               mousePosition.x + 10,
               mousePosition.y,RED);
      DrawLine(mousePosition.x,
               mousePosition.y - 10,
               mousePosition.x,
               mousePosition.y + 10,RED);




    //Ekrana test yazısı yazdır
     if(GetTime() < 3){
      DrawText("Asteroids Oyununa Hos Geldin!",220,280,20,GREEN);}
     if(GetTime() > 3)
      DrawText("Skor: 0", 20, 20, 20, GREEN);
     EndDrawing();
    }

    //3.Kapanış ve temizlik
    //Döngü bitince pencereyi kapat ve belleği boşalt

    CloseWindow();
    return 0;
}
