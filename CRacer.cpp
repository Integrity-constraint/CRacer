#include "raylib.h"
#include <stdlib.h> // Для функции rand()

int main()
{
    // Инициализация окна
    const int windowWidth = 1024;
    const int windowHeight = 768;
    InitWindow(windowWidth, windowHeight, "CRacer");

    // Загрузка спрайтов
    Texture2D playerCar = LoadTexture("resource/bluecar.png"); // Замените на путь к вашему спрайту
    Texture2D enemyCar1 = LoadTexture("resource/purpcar.png"); // Замените на путь к вашему спрайту
    Texture2D enemyCar2 = LoadTexture("resource/redcar.png"); // Замените на путь к вашему спрайту
    Texture2D menuBackground = LoadTexture("resource/back.png"); // Замените на путь к вашему фоновому изображению

    // Позиции машин
    Vector2 playerCarPosition = { windowWidth / 2.0f, windowHeight / 2.0f };
    Vector2 enemyCar1Position = { windowWidth / 2.0f - playerCar.width, windowHeight / 2.0f };
    Vector2 enemyCar2Position = { windowWidth / 2.0f + playerCar.width, windowHeight / 2.0f };

    // Скорости машин противников
    Vector2 enemyCar1Speed = { 0.0f, 0.2f }; // Уменьшенная скорость
    Vector2 enemyCar2Speed = { 0.0f, 0.2f }; // Уменьшенная скорость

    // Скорость машины игрока
    float playerCarSpeed = 0.2f; // Уменьшенная скорость

    // Состояние игры
    bool isGameStarted = false;

    // Главный цикл игры
    while (!WindowShouldClose())
    {
        // Обновление
        if (isGameStarted)
        {
            // Управление машиной игрока
            if (IsKeyDown(KEY_RIGHT)) playerCarPosition.x += playerCarSpeed;
            if (IsKeyDown(KEY_LEFT)) playerCarPosition.x -= playerCarSpeed;
            if (IsKeyDown(KEY_UP)) playerCarPosition.y -= playerCarSpeed;
            if (IsKeyDown(KEY_DOWN)) playerCarPosition.y += playerCarSpeed;

            // Обновление позиций машин противников
            enemyCar1Position.y += enemyCar1Speed.y;
            enemyCar2Position.y += enemyCar2Speed.y;

            // Добавляем случайные повороты
            enemyCar1Position.x += (rand() % 3 - 1) * enemyCar1Speed.y; // -1, 0, или 1
            enemyCar2Position.x += (rand() % 3 - 1) * enemyCar2Speed.y; // -1, 0, или 1

            // Если машины противников достигли нижней границы, переместить их наверх
            if (enemyCar1Position.y > windowHeight) enemyCar1Position.y = -enemyCar1.height;
            if (enemyCar2Position.y > windowHeight) enemyCar2Position.y = -enemyCar2.height;
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
            // Отрисовка машин
            DrawTexture(playerCar, playerCarPosition.x, playerCarPosition.y, WHITE);
            DrawTexture(enemyCar1, enemyCar1Position.x, enemyCar1Position.y, WHITE);
            DrawTexture(enemyCar2, enemyCar2Position.x, enemyCar2Position.y, WHITE);
        }
        else
        {
            // Отрисовка меню с фоновым изображением
            DrawTexture(menuBackground, 0, 0, WHITE);
            // Отрисовка кнопки "Играть"
            DrawRectangle(windowWidth / 2 - 50, windowHeight / 2 - 20, 100, 40, LIGHTGRAY);
            DrawText("play", windowWidth / 2 - MeasureText("play", 20) / 2, windowHeight / 2 - 10, 20, BLACK);
        }

        EndDrawing();
    }

    // Освобождение ресурсов
    UnloadTexture(playerCar);
    UnloadTexture(enemyCar1);
    UnloadTexture(enemyCar2);
    UnloadTexture(menuBackground); // Освобождаем ресурсы фона
    CloseWindow();

    return 0;
}