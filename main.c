#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGTH (600)

#define PLAYER_X (50)
#define PLAYER_Y (500)

#define MAX_ENEMY (10)


typedef struct Enemy {
    float x;
    float y;
    float width;
    float height;
}Enemy;


typedef struct GameState
{
    Rectangle player;
    float playerVelocity;
    bool playerOnGround;

    Enemy enemies[MAX_ENEMY];
    int enemyCount;
    float spawnTime;
    float enemySpeed;

    float score;
    float gravity;
    float gameTime;
    float holdingSpaceTime;

    enum GameScreen { MAIN = 0, GAMEPLAY, END } gameScreen;

}GameState;

static const GameState DefaultState = {

        .player = {PLAYER_X, PLAYER_Y, 40, 40},
        .gravity = 1500.0f,
        .enemySpeed = 400.0f,
        .gameScreen = MAIN,
};


void draw_text_centered(const char* text, Vector2 pos, float fontSize)
{
    const Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, 1);
    pos.x -= textSize.x / 2.0f;
    DrawText(text, pos.x, pos.y, fontSize, WHITE);
}   




int main(void)
{

    //    TODO: Update enemy epeed with score
    //    TODO: check collision but convert vector2's to rectangle
    //    TODO: random max enemy

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "Dinosaur Clone");

    GameState gameState = DefaultState;

    Camera2D camera;
    camera.target = (Vector2){ gameState.player.x, gameState.player.y };
    camera.offset = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGTH / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;


    while (!WindowShouldClose())
    {

        BeginDrawing();
        ClearBackground(GRAY);


        switch (gameState.gameScreen)
        {
            case MAIN:

                ClearBackground(BLACK);
                draw_text_centered("Press 'Enter' to start!", (Vector2) { SCREEN_WIDTH / 2.0f, 200.0f }, 50);

                if (IsKeyPressed(KEY_ENTER))
                {
                    gameState.gameScreen = GAMEPLAY;
                }

                break;
            case GAMEPLAY:

                gameState.gameTime += GetFrameTime();
                gameState.score += 0.01;

                BeginMode2D(camera);

                // Update player
                {

                    gameState.player.y += gameState.playerVelocity * GetFrameTime();
                    gameState.playerVelocity += gameState.gravity * GetFrameTime();

                    if (IsKeyDown(KEY_SPACE) && gameState.playerOnGround) {
                        gameState.holdingSpaceTime += GetFrameTime();
                    }
                    gameState.gravity = 1500.0f;

                    if (IsKeyDown(KEY_DOWN) && !gameState.playerOnGround) {
                        gameState.gravity = 10000.0f;
                    }

                    if (IsKeyReleased(KEY_SPACE) && gameState.playerOnGround) {
                        if (gameState.holdingSpaceTime > 0.25f) {
                            gameState.playerVelocity = -600.0f;
                        }
                        else {
                            gameState.playerVelocity = -500.0f;
                        }
                        gameState.holdingSpaceTime = 0.0f;
                    }

                    if (gameState.player.y >= 500) {
                        gameState.player.y = 500;
                        gameState.playerOnGround = true;
                    }
                    else {
                        gameState.playerOnGround = false;
                    }

                    camera.target = (Vector2){ gameState.player.x + 350 ,300 };

                    DrawRectangle(gameState.player.x, gameState.player.y, gameState.player.width, gameState.player.height, RED);
                }


                // Update Enemies
                {

                    float spawnFrequency = 0.7;
                    gameState.spawnTime += GetFrameTime();


                    while (gameState.spawnTime >= spawnFrequency) {

                        if (gameState.enemyCount >= MAX_ENEMY) {
                            break;
                        }


                        int sizeOption = GetRandomValue(0, 2);
                        int enemyAddPos = 0;

                        int enemyX = GetRandomValue(SCREEN_WIDTH + gameState.player.x, SCREEN_WIDTH + gameState.player.x + 100);
                        int enemyY = 500;
                        int enemyWidth = 0;
                        int enemyHeight = 0;

                        switch (sizeOption) {

                        case 0:
                            enemyWidth = 80;
                            enemyHeight = 40;
                            break;

                        case 1:
                            enemyWidth = 40;
                            enemyHeight = 40;
                            break;

                        case 2:
                            enemyWidth = 40;
                            enemyHeight = 40;
                            enemyX = GetRandomValue(SCREEN_WIDTH + gameState.player.x, SCREEN_WIDTH + gameState.player.x + 100);
                            enemyY = GetRandomValue(425, 450);
                            enemyAddPos = enemyWidth;
                            break;
                        }

                        Enemy enemy = {

                            .x = enemyX,
                            .y = enemyY,
                            .width = enemyWidth,
                            .height = enemyHeight,
                        };



                        enemy.x += enemyAddPos;
                        enemyAddPos = enemy.width;

                        gameState.enemies[gameState.enemyCount++] = enemy;

                        gameState.spawnTime -= spawnFrequency;


                    }

                    int result = (int)gameState.score % 100;

                    if (result == 0 && gameState.score <= 10000) {
                        gameState.enemySpeed += 0.1f;
                        spawnFrequency -= 0.01;
                    }


                    for (int enemyId = 0; enemyId < MAX_ENEMY; enemyId++)
                    {

                        Enemy* enemy = &gameState.enemies[enemyId];

                        DrawRectangle(enemy->x, enemy->y, enemy->width, enemy->height, BLUE);
                        enemy->x -= gameState.enemySpeed * GetFrameTime();

                        if (gameState.enemyCount > 0) {
                            if (gameState.player.x - 100 > enemy->x + enemy->width) {
                                *enemy = gameState.enemies[--gameState.enemyCount];
                            }
                        }

                        Rectangle enemyRec = { enemy->x, enemy->y, enemy->width, enemy->height };


                        if (CheckCollisionRecs(gameState.player, enemyRec)) {

                            gameState.gameScreen = END;

                        }

                    }


                }

                // Draw Essentials
                {
                    int  yPosLine = SCREEN_HEIGTH - gameState.player.height - gameState.player.height / 2.0f;
                    DrawLine(gameState.player.x - SCREEN_WIDTH, yPosLine, gameState.player.x + SCREEN_WIDTH, yPosLine, RED);
                    DrawText(TextFormat("Score: %.0f", gameState.score), 0, 0, 20, BLACK);
                }

                EndMode2D();
                break;

                case END:

                    ClearBackground(BLACK);
                    DrawText(TextFormat("Your score : %.0f", gameState.score), 150, 200.0f, 50, WHITE);

                    if (IsKeyPressed(KEY_ENTER))
                    {
                        gameState = DefaultState;
                        gameState.gameScreen = GAMEPLAY;
                    }

                    break;
        }

        EndDrawing();

    }
    CloseWindow();

    return 0;

}
