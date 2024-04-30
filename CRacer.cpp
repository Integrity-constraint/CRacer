#include "raylib.h"
#include <stdlib.h> // Для функции rand() и srand()
#include <time.h>   // Для функции time()

Vector2 GetRandomCarPosition(int windowWidth, int carWidth) {
    int x = rand() % (windowWidth - carWidth); // Случайная позиция по оси X
    return Vector2{ static_cast<float>(x), -static_cast<float>(carWidth) }; // Начинаем сверху экрана
}

int main() {
    // Инициализация генератора случайных чисел
    srand(static_cast<unsigned int>(time(NULL)));

    // Инициализация окна
    const int windowWidth = 1024;
    const int windowHeight = 768;
    InitWindow(windowWidth, windowHeight, "CRacer");

    // Загрузка спрайтов
    Texture2D playerCar = LoadTexture("resource/player.png");
    Texture2D enemyCar1 = LoadTexture("resource/enemy1.png");
    Texture2D enemyCar2 = LoadTexture("resource/enemy2.png");
    Texture2D menuBackground = LoadTexture("resource/back.png");
    Texture2D racingTrackBackground = LoadTexture("resource/racingtrack.png");

    // Позиции машин
    Vector2 playerCarPosition = { windowWidth / 2.0f, windowHeight - playerCar.height }; // Позиция игрока на нижней части экрана
    Vector2 enemyCar1Position = GetRandomCarPosition(windowWidth, enemyCar1.width);
    Vector2 enemyCar2Position = GetRandomCarPosition(windowWidth, enemyCar2.width);

    // Скорости машин противников
    float enemyCar1Speed = 200.0f; // Скорость падения в пикселях в секунду
    float enemyCar2Speed = 250.0f; // Скорость падения в пикселях в секунду

    // Скорость игрока
    float playerCarSpeed = 300.0f; // Скорость движения игрока в пикселях в секунду

    // Загрузка музыки
    Music soundtrack = LoadMusicStream("resource/engine.mp3");
    soundtrack.looping = true; // Теперь музыка будет повторяться

    // Состояние игры
    bool isGameStarted = false;
    Color neonViolet = { 148, 0, 211, 255 };

    // Главный цикл игры
    while (!WindowShouldClose()) {
        // Обновление музыки
        UpdateMusicStream(soundtrack);

        // Обновление
        if (isGameStarted) {
            // Управление машиной игрока
            if (IsKeyDown(KEY_RIGHT)) playerCarPosition.x += playerCarSpeed * GetFrameTime();
            if (IsKeyDown(KEY_LEFT)) playerCarPosition.x -= playerCarSpeed * GetFrameTime();
            if (IsKeyDown(KEY_UP)) playerCarPosition.y -= playerCarSpeed * GetFrameTime();
            if (IsKeyDown(KEY_DOWN)) playerCarPosition.y += playerCarSpeed * GetFrameTime();

            // Обновление позиций машин противников
            enemyCar1Position.y += enemyCar1Speed * GetFrameTime();
            enemyCar2Position.y += enemyCar2Speed * GetFrameTime();

            // Если машины достигли нижней границы, переместить их наверх
            if (enemyCar1Position.y > windowHeight) {
                enemyCar1Position = GetRandomCarPosition(windowWidth, enemyCar1.width);
            }
            if (enemyCar2Position.y > windowHeight) {
                enemyCar2Position = GetRandomCarPosition(windowWidth, enemyCar2.width);
            }
        }
        else {
            // Обработка нажатия кнопки "Играть"
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePosition = GetMousePosition();
                if (mousePosition.x >= windowWidth / 2 - 50 && mousePosition.x <= windowWidth / 2 + 50 &&
                    mousePosition.y >= windowHeight / 2 - 20 && mousePosition.y <= windowHeight / 2 + 20) {
                    isGameStarted = true;
                    // Запуск музыки при начале игры
                    PlayMusicStream(soundtrack);
                }
            }
        }

        // Отрисовка
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (isGameStarted) {
            // Отрисовка фона трассы
            DrawTexture(racingTrackBackground, 0, 0, WHITE);

            // Отрисовка машин
            DrawTexture(playerCar, (int)playerCarPosition.x, (int)playerCarPosition.y, WHITE);
            DrawTexture(enemyCar1, (int)enemyCar1Position.x, (int)enemyCar1Position.y, WHITE);
            DrawTexture(enemyCar2, (int)enemyCar2Position.x, (int)enemyCar2Position.y, WHITE);
        }
        else {
            // Отрисовка меню с фоновым изображением
            DrawTexture(menuBackground, 0, 0, WHITE);
            // Отрисовка кнопки "Играть" с новым цветом
            DrawRectangle(windowWidth / 2 - 50, windowHeight / 2 - 20, 100, 40, neonViolet);
            DrawText("play", windowWidth / 2 - MeasureText("play", 20) / 2, windowHeight / 2 - 10, 20, BLACK);
        }

        EndDrawing();
    }

    // Освобождение ресурсов
    UnloadTexture(playerCar);
    UnloadTexture(enemyCar1);
    UnloadTexture(enemyCar2);
    UnloadTexture(menuBackground);
    UnloadTexture(racingTrackBackground);
    UnloadMusicStream(soundtrack); // Освобождаем музыку

    CloseWindow();

    return 0;
}