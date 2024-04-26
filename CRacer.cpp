// CRacer.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <raylib.h>
#include <stdlib.h> 
#include <math.h> 


typedef struct Car {
    Vector2 position;
    Vector2 speed;
    float acceleration;
    float rotation;
    Texture2D sprite;
} Car;
void main()
{
    setlocale(LC_ALL, "Russian");
    // Инициализация окна
    const int windowWidth = 1024;
    const int windowHeight = 768;
    InitWindow(windowWidth, windowHeight, "CRacer");

    // Загрузка спрайтов
    Texture2D playerCarSprite = LoadTexture("resource/redcar.png"); // Замените на путь к вашему спрайту
    Texture2D enemyCar1Sprite = LoadTexture("resource/purpcar.png"); // Замените на путь к вашему спрайту
    Texture2D enemyCar2Sprite = LoadTexture("resource/bluecar.png"); // Замените на путь к вашему спрайту
    // Инициализация машинок
    Car playerCar = { { windowWidth / 2.0f, windowHeight / 2.0f }, { 0.0f, 0.0f }, 0.0f, 0.0f, playerCarSprite };
    Car enemyCar1 = { { windowWidth / 2.0f - playerCarSprite.width, windowHeight / 2.0f }, { 0.0f, 0.15f }, 0.0f, 0.0f, enemyCar1Sprite }; // Уменьшенная скорость
    Car enemyCar2 = { { windowWidth / 2.0f + playerCarSprite.width, windowHeight / 2.0f }, { 0.0f, 0.225f }, 0.0f, 0.0f, enemyCar2Sprite }; // Уменьшенная скорость

    // Состояние игры
    bool isGameStarted = false;

    // Главный цикл игры
    while (!WindowShouldClose())
    {
        // Обновление
        if (isGameStarted)
        {
            // Управление машинкой игрока
            if (IsKeyDown(KEY_RIGHT)) playerCar.rotation += 0.1f; // Уменьшенная скорость поворота
            if (IsKeyDown(KEY_LEFT)) playerCar.rotation -= 0.1f; // Уменьшенная скорость поворота

            // Скорость
            playerCar.speed.x = sin(playerCar.rotation * DEG2RAD) * 0.3f; // Уменьшенная скорость
            playerCar.speed.y = cos(playerCar.rotation * DEG2RAD) * 0.3f; // Уменьшенная скорость

            // Ускорение
            if (IsKeyDown(KEY_UP))
            {
                if (playerCar.acceleration < 1) playerCar.acceleration += 0.015f; // Уменьшенное ускорение
            }
            else
            {
                if (playerCar.acceleration > 0) playerCar.acceleration -= 0.0075f; // Уменьшенное ускорение
                else if (playerCar.acceleration < 0) playerCar.acceleration = 0;
            }

            // Движение
            playerCar.position.x += (playerCar.speed.x * playerCar.acceleration);
            playerCar.position.y -= (playerCar.speed.y * playerCar.acceleration);

            // Обновление позиций машинок противников
            enemyCar1.position.y += enemyCar1.speed.y;
            enemyCar2.position.y += enemyCar2.speed.y;

            // Если машинки противников достигли нижней границы, переместить их наверх
            if (enemyCar1.position.y > windowHeight) enemyCar1.position.y = -enemyCar1.sprite.height;
            if (enemyCar2.position.y > windowHeight) enemyCar2.position.y = -enemyCar2.sprite.height;
        }
        else
        {
            // Обработка нажатия кнопки "Играть"
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                Vector2 mousePosition = GetMousePosition();
                if (mousePosition.x >= windowWidth / 2 - 50 && mousePosition.x <= windowWidth / 2 + 50 &&
                    mousePosition.y >= windowHeight / 2 - 20 && mousePosition.y <= windowHeight / 2 + 20)
                {
                    isGameStarted = true;
                }
            }
        }

        // Отрисовка
        BeginDrawing();

        ClearBackground(RAYWHITE);

        if (isGameStarted)
        {
            // Отрисовка машинок
            DrawTextureEx(playerCar.sprite, playerCar.position, playerCar.rotation, 1.0f, WHITE);
            DrawTextureEx(enemyCar1.sprite, enemyCar1.position, enemyCar1.rotation, 1.0f, WHITE);
            DrawTextureEx(enemyCar2.sprite, enemyCar2.position, enemyCar2.rotation, 1.0f, WHITE);
        }
        else
        {
            // Отрисовка кнопки "Играть"
            DrawRectangle(windowWidth / 2 - 50, windowHeight / 2 - 20, 100, 40, LIGHTGRAY);
            DrawText("play", windowWidth / 2 - MeasureText("play", 20) / 2, windowHeight / 2 - 10, 20, BLACK);
        }

        EndDrawing();
    }

    // Освобождение ресурсов
    UnloadTexture(playerCar.sprite);
    UnloadTexture(enemyCar1.sprite);
    UnloadTexture(enemyCar2.sprite);
    CloseWindow();
   
}

