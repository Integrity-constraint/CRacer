#include "raylib.h"
#include <stdlib.h> // Для функции rand() и srand()
#include <time.h>   // Для функции time()
#include <vector>
#include <sstream>
#include <iomanip>

Vector2 GetRandomCarPosition(int windowWidth, int carWidth) {
    int x = rand() % (windowWidth - carWidth); // Случайная позиция по оси X
    return Vector2{ static_cast<float>(x), -static_cast<float>(carWidth) }; // Начинаем сверху экрана
}

bool isColliding(const Vector2& pos1, int width1, int height1, const Vector2& pos2, int width2, int height2) {
    return (pos1.x < pos2.x + width2 &&
        pos1.x + width1 > pos2.x &&
        pos1.y < pos2.y + height2 &&
        pos1.y + height1 > pos2.y);
}

int main() {
    // Инициализация генератора случайных чисел
    srand(static_cast<unsigned int>(time(NULL)));
    InitAudioDevice();
    // Инициализация окна
    const int windowWidth = 1024;
    const int windowHeight = 768;
    InitWindow(windowWidth, windowHeight, "CRacer");

    // Загрузка спрайтов
    Texture2D playerCar = LoadTexture("resource/player.png");
    std::vector<Texture2D> enemyCars;
    for (int i = 1; i <= 7; ++i) {
        std::stringstream ss;
        ss << "resource/enemy" << i << ".png";
        enemyCars.push_back(LoadTexture(ss.str().c_str()));
    }
    Texture2D menuBackground = LoadTexture("resource/back.png");
    Texture2D racingTrackBackground = LoadTexture("resource/racingtrack.png");

    // Позиции машин
    Vector2 playerCarPosition = { windowWidth / 2.0f, windowHeight - playerCar.height }; // Позиция игрока на нижней части экрана
    std::vector<Vector2> enemyCarPositions;
    for (int i = 0; i < 7; ++i) {
        enemyCarPositions.push_back(GetRandomCarPosition(windowWidth, enemyCars[i].width));
    }

    // Скорости машин противников
    std::vector<float> enemyCarSpeeds = { 340.0f, 350.0f, 340.0f, 360.0f, 370.0f, 340.0f, 340.0f };

    // Скорость игрока
    float playerCarSpeed = 300.0f; // Скорость движения игрока в пикселях в секунду

    // Загрузка музыки
    Music soundtrack = LoadMusicStream("resource/engine.mp3");
    soundtrack.looping = true; // Теперь музыка будет повторяться

    // Состояние игры
    bool isGameStarted = false;
    Color neonViolet = { 148, 0, 211, 255 };

    // Текущее время игры
    double gameTime = 0.0;
    bool hasCollided = false;
    // Главный цикл игры
    while (!WindowShouldClose()) {
        // Обновление музыки
        UpdateMusicStream(soundtrack);

        // Обновление
        if (isGameStarted && !hasCollided) {
            // Управление машиной игрока
            if (IsKeyDown(KEY_RIGHT)) playerCarPosition.x += playerCarSpeed * GetFrameTime();
            if (IsKeyDown(KEY_LEFT)) playerCarPosition.x -= playerCarSpeed * GetFrameTime();
            if (IsKeyDown(KEY_UP)) playerCarPosition.y -= playerCarSpeed * GetFrameTime();
            if (IsKeyDown(KEY_DOWN)) playerCarPosition.y += playerCarSpeed * GetFrameTime();

            for (size_t i = 0; i < enemyCarPositions.size(); ++i) {
                if (isColliding(playerCarPosition, playerCar.width, playerCar.height, enemyCarPositions[i], enemyCars[i].width, enemyCars[i].height)) {
                    // Столкновение обнаружено
                    hasCollided = true;
                    break;
                }
            }

            // Обновление позиций машин противников
            for (size_t i = 0; i < enemyCarPositions.size(); ++i) {
                enemyCarPositions[i].y += enemyCarSpeeds[i] * GetFrameTime();

                // Если машина достигла нижней границы, переместить ее наверх
                if (enemyCarPositions[i].y > windowHeight) {
                    bool isSafePosition = false;
                    while (!isSafePosition) {
                        Vector2 newPosition = GetRandomCarPosition(windowWidth, enemyCars[i].width);
                        isSafePosition = true;
                        for (size_t j = 0; j < enemyCarPositions.size(); ++j) {
                            if (i != j && isColliding(newPosition, enemyCars[i].width, enemyCars[i].height, enemyCarPositions[j], enemyCars[j].width, enemyCars[j].height)) {
                                isSafePosition = false;
                                break;
                            }
                        }
                        if (isSafePosition) {
                            enemyCarPositions[i] = newPosition;
                        }
                    }
                }
            }

            // Обновление времени игры
            gameTime += GetFrameTime();
        }
        else {

            if (hasCollided && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mousePosition = GetMousePosition();
                if (mousePosition.x >= windowWidth / 2 - 50 && mousePosition.x <= windowWidth / 2 + 50 &&
                    mousePosition.y >= windowHeight / 2 + 20 && mousePosition.y <= windowHeight / 2 + 60) {
                    // Возвращаем игру в начальное состояние
                    isGameStarted = false;
                    hasCollided = false;
                    // Возвращаем игрока к начальной позиции
                    playerCarPosition = Vector2{ static_cast<float>(windowWidth) / 2.0f, static_cast<float>(windowHeight) - playerCar.height };
                    // Сбрасываем таймер
                    gameTime = 0.0;

                    // Перемещаем машинки в новые случайные позиции
                    for (size_t i = 0; i < enemyCarPositions.size(); ++i) {
                        enemyCarPositions[i] = GetRandomCarPosition(windowWidth, enemyCars[i].width);
                    }

                    // Останавливаем музыку
                    StopMusicStream(soundtrack);
                }
            }

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
            for (size_t i = 0; i < enemyCarPositions.size(); ++i) {
                DrawTexture(enemyCars[i], (int)enemyCarPositions[i].x, (int)enemyCarPositions[i].y, WHITE);
            }

            // Отрисовка таймера
            int minutes = (int)gameTime / 60;
            int seconds = (int)gameTime % 60;
            std::stringstream timerText;
            timerText << std::setfill('0') << std::setw(2) << minutes << ":" << std::setw(2) << seconds;
            DrawText(timerText.str().c_str(), 10, 10, 20, WHITE);

            if (hasCollided) {
                DrawText("Game Over", windowWidth / 2 - MeasureText("Game Over", 40) / 2, windowHeight / 2 - 60, 40, RED);
                DrawRectangle(windowWidth / 2 - 50, windowHeight / 2 + 20, 100, 40, neonViolet);
                DrawText("back", windowWidth / 2 - MeasureText("back", 20) / 2, windowHeight / 2 + 40, 20, BLACK);
            }

        }
        else {
            // Отрисовка меню с фоновым изображением
            DrawTexture(menuBackground, 0, 0, WHITE);
            // Отрисовка кнопки "Играть" с новым цветом
            DrawRectangle(windowWidth / 2 - 50, windowHeight / 2 - 20, 100, 40, neonViolet);
            DrawText("play", windowWidth / 2 - MeasureText("play", 20) / 2, windowHeight / 2 - 10, 20, WHITE);

           
            const char* text = "CRacer";
            int textWidth = MeasureText(text, 60); // Увеличиваем размер текста
            int textHeight = 60; // Устанавливаем высоту текста
            DrawText(text, (windowWidth - textWidth) / 2, 50, textHeight, WHITE);


        }

        EndDrawing();
    }

    // Освобождение ресурсов
    UnloadTexture(playerCar);
    // Освобождаем текстуры для машин противников
    for (auto& enemyCar : enemyCars) {
        UnloadTexture(enemyCar);
    }
    UnloadTexture(menuBackground);
    UnloadTexture(racingTrackBackground);
    UnloadMusicStream(soundtrack); // Освобождаем музыку

    CloseWindow();

    return 0;
}