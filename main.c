#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>

#define SCREEN_WIDTH (800)
#define SCREEN_HEIGTH (600)

#define PLAYER_X (50)
#define PLAYER_Y (500)

#define MAX_ENEMY (5)


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


    int enemyCount;
    bool enemyCreating;
    float spawnTime;
    Enemy enemies[MAX_ENEMY];
    


}GameState;


int main(void)
{

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGTH, "Dinosaur Clone");

    GameState gameState = {

        .playerPos = {PLAYER_X, PLAYER_Y},
        .playerSize = {40, 40},
        .gravity = 1000.0f,
        
    };

    Camera2D camera;
    camera.target = gameState.playerPos;
    camera.offset = (Vector2){ SCREEN_WIDTH / 2.0f, SCREEN_HEIGTH / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;


    while (!WindowShouldClose())
    {
        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(GRAY);


        gameState.gameTime += GetFrameTime();
        int maxEnemy = GetRandomValue(1, 2);
        printf("%d", maxEnemy);

        // Update player
        {

            gameState.playerPos.y += gameState.playerVelocity * GetFrameTime();
            gameState.playerVelocity += gameState.gravity * GetFrameTime();

            if (IsKeyDown(KEY_SPACE) && gameState.playerOnGround) {
                gameState.spaceTime += GetFrameTime();
            }

            if (IsKeyReleased(KEY_SPACE) && gameState.playerOnGround) {
                if (gameState.spaceTime > 0.25f) {
                    gameState.playerVelocity = -500.0f;
                }
                else {
                    gameState.playerVelocity = -400.0f;
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

            //gameState.playerPos.x += 400 * GetFrameTime();
            
            
            DrawRectangleV(gameState.playerPos, gameState.playerSize, RED);
        }


        // Camera
        {
            camera.target = (Vector2){ gameState.playerPos.x + 350 ,300};

        }



        // Update Enemies
        {
           
            const float spawnFrequency = 0.1;
            gameState.spawnTime += GetFrameTime();


            while (gameState.spawnTime >= spawnFrequency) {

                if (gameState.enemyCount >= MAX_ENEMY) {
                    break;
                }

                Vector2 pos = { GetRandomValue(SCREEN_WIDTH + gameState.playerPos.x, SCREEN_WIDTH + gameState.playerPos.x + 100), 500 };
                Vector2 size = { 40, 40 };

                Enemy enemy = {

                    .pos = pos,
                    .size = size
                };      

                gameState.enemies[gameState.enemyCount++] = enemy;

                gameState.spawnTime -= spawnFrequency;


            }

            int enemiesToSpawn = GetRandomValue(1, MAX_ENEMY);

            for (int enemyId = 0; enemyId < enemiesToSpawn; enemyId++)
            {

                Enemy* enemy = &gameState.enemies[enemyId];

                DrawRectangleV(enemy->pos, enemy->size, BLUE);
                enemy->pos.x -= 200 * GetFrameTime();

                if (gameState.enemyCount > enemiesToSpawn) {

                    gameState.enemies[gameState.enemyCount++] = (Enemy){

                        .pos = {500,500},
                        .size = {40, 40}
                    };

                }
                else {
                    if (gameState.playerPos.x - 100 > enemy->pos.x) {
                        *enemy = gameState.enemies[--gameState.enemyCount];
                    }
                }


            }
            


         
        }
        

        // Draw line
        {

            int  yPosLine = SCREEN_HEIGTH - gameState.playerSize.y - gameState.playerSize.y / 2.0f;
            DrawLine(gameState.playerPos.x - SCREEN_WIDTH, yPosLine, gameState.playerPos.x + SCREEN_WIDTH, yPosLine, RED);
            
        }

        // Helpfull draw tools

        {
            DrawText(TextFormat("%f", gameState.playerPos.x), gameState.playerPos.x, gameState.playerPos.y - 20, 20, BLACK);

        }



        EndMode2D();
        EndDrawing();

    }
    CloseWindow();

    return 0;

}
