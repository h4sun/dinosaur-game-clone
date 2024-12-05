#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGTH 600

#define PLAYER_X 50
#define PLAYER_Y 500


typedef struct GameState
{
    Vector2 playerPos;
    Vector2 playerSize;
    float playerVelocity;
    bool playerOnGround;


    Vector2 enemyPos;
    Vector2 enemySize;

    float gravity;
    float spaceTime;
    int linePos;

}GameState;


int main(void)
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "Dinosaur Clone");

    GameState gameState = {

        .playerPos = {PLAYER_X, PLAYER_Y},
        .playerSize = {40, 40},
        .gravity = 1000.0f,
        .enemySize = {20, 60},
        .linePos = 0
        
    };

    Camera2D camera;
    camera.target = gameState.playerPos;
    camera.offset = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGTH / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;


    while (!WindowShouldClose())
    {


        // Update player
        {

            gameState.playerPos.y += gameState.playerVelocity * GetFrameTime();
            gameState.playerVelocity += gameState.gravity * GetFrameTime();

            if (IsKeyDown(KEY_SPACE) && gameState.playerOnGround) {
                gameState.spaceTime += GetFrameTime();
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



        }

        {
            camera.target = (Vector2){ gameState.playerPos.x + 350 ,300};

        }



        // Enemies
        {
            gameState.enemyPos.x = GetRandomValue(gameState.playerPos.x + 100, gameState.playerPos.x + SCREEN_WIDTH);



        }


        // Update Line
        {

            gameState.playerPos.x += 400 * GetFrameTime();

        }

        BeginDrawing();
        ClearBackground(GRAY);
        BeginMode2D(camera);

        // Draw player
        {
            DrawRectangleV(gameState.playerPos, gameState.playerSize, RED);
            DrawLine(gameState.playerPos.x - SCREEN_WIDTH, 540, gameState.playerPos.x + SCREEN_WIDTH, 540, RED);
            DrawRectangle(1000, 400, 20, 60, BLUE);

            //DrawText(TextFormat("velocity : %f", gameState.playerVelocity), gameState.playerPos.x, gameState.playerPos.y - 20, 20, BLACK);
            //DrawText(TextFormat("player y : %f", gameState.playerPos.y), gameState.playerPos.x, gameState.playerPos.y - 40, 20, BLACK);
            //DrawText(TextFormat("holding space : %f", gameState.spaceTime), gameState.playerPos.x, gameState.playerPos.y - 60, 20, BLACK);
            //DrawText(TextFormat("on ground : %d", gameState.playerOnGround), gameState.playerPos.x, gameState.playerPos.y - 80, 20, BLACK);
        }
        EndMode2D();
        EndDrawing();

    }
    CloseWindow();

    return 0;

}
