#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGTH (600)

#define PLAYER_X (50)
#define PLAYER_Y (500)

#define MAX_ENEMY (100)


typedef struct Enemy {

    Vector2 pos;
    Vector2 size;
}Enemy;


typedef struct GameState
{
    Vector2 playerPos;
    Vector2 playerSize;
    float playerVelocity;
    bool playerOnGround;

    float gravity;
    float spaceTime;
    float gameTime;
    float score;


    int enemyCount;
    bool enemyCreating;
    float spawnTime;
    float enemySpeed;
    int maxEnemy;
    Enemy enemies[MAX_ENEMY];




}GameState;


int main(void)
{

    //    TODO: Update enemy epeed with score
    //    TODO: check collision but convert vector2's to rectangle
    //    TODO: random max enemy

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "Dinosaur Clone");

    GameState gameState = {

        .playerPos = {PLAYER_X, PLAYER_Y},
        .playerSize = {40, 40},
        .gravity = 1500.0f,
        .enemySpeed = 400.0f,
        .maxEnemy = 1,
    };

    Camera2D camera;
    camera.target = gameState.playerPos;
    camera.offset = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGTH / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;


    while (!WindowShouldClose())
    {

        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(GRAY);


        gameState.gameTime += GetFrameTime();
        gameState.score += 0.01;


        // Update player
        {

            gameState.playerPos.y += gameState.playerVelocity * GetFrameTime();
            gameState.playerVelocity += gameState.gravity * GetFrameTime();

            if (IsKeyDown(KEY_SPACE) && gameState.playerOnGround) {
                gameState.spaceTime += GetFrameTime();
            }
            gameState.gravity = 1500.0f;

            if (IsKeyDown(KEY_DOWN) && !gameState.playerOnGround) {
                gameState.gravity = 10000.0f;
            }

            if (IsKeyReleased(KEY_SPACE) && gameState.playerOnGround) {
                if (gameState.spaceTime > 0.25f) {
                    gameState.playerVelocity = -600.0f;
                }
                else {
                    gameState.playerVelocity = -500.0f;
                }
                gameState.spaceTime = 0.0f;
            }

            if (gameState.playerPos.y >= 500) {
                gameState.playerPos.y = 500;
                gameState.playerOnGround = true;
            }
            else {
                gameState.playerOnGround = false;
            }

            camera.target = (Vector2){ gameState.playerPos.x + 350 ,300 };

            DrawRectangleV(gameState.playerPos, gameState.playerSize, RED);
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
                Vector2 size = { 0, 0 };
                Vector2 pos = { GetRandomValue(SCREEN_WIDTH + gameState.playerPos.x, SCREEN_WIDTH + gameState.playerPos.x + 100), 500 };


                switch (sizeOption) {

                case 0:
                    size = (Vector2){ 80, 40 };
                    break;

                case 1:
                    size = (Vector2){ 40, 40 };
                    break;

                case 2:
                    size = (Vector2){ 40, 40 };
                    pos = (Vector2){ GetRandomValue(SCREEN_WIDTH + gameState.playerPos.x, SCREEN_WIDTH + gameState.playerPos.x + 100), GetRandomValue(425, 450) };
                    enemyAddPos = size.x;
                    break;
                }

                Enemy enemy = {

                    .pos = pos,
                    .size = size
                };



                enemy.pos.x += enemyAddPos;
                enemyAddPos = enemy.size.x;

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

                DrawRectangleV(enemy->pos, enemy->size, BLUE);
                enemy->pos.x -= gameState.enemySpeed * GetFrameTime();
                
                if (gameState.enemyCount > 0) {
                    if (gameState.playerPos.x - 100 > enemy->pos.x + enemy->size.x) {
                        *enemy = gameState.enemies[--gameState.enemyCount];
                    }
                }


            }

        }


        // Draw Essentials
        {
            int  yPosLine = SCREEN_HEIGTH - gameState.playerSize.y - gameState.playerSize.y / 2.0f;
            DrawLine(gameState.playerPos.x - SCREEN_WIDTH, yPosLine, gameState.playerPos.x + SCREEN_WIDTH, yPosLine, RED);
            DrawText(TextFormat("Score: %.0f", gameState.score), 0, 0, 20, BLACK);
        }



        EndMode2D();
        EndDrawing();

    }
    CloseWindow();

    return 0;

}
