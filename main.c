#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include <stdbool.h>

#define WIDTH 800
#define HEIGTH 600

#define PLAYER_X 100
#define PLAYER_Y 500


typedef struct GameState
{
    Vector2 playerPos;
    Vector2 playerSize;
    float playerVelocity;
    bool playerOnGround;
    
    
    float gravity;
    float elapsedTime;
    
}GameState;


int main(void)
{
    
    InitWindow(WIDTH, HEIGTH, "Dinosaur Clone");
    
    GameState gameState = {
        
        .playerPos = {PLAYER_X, PLAYER_Y},
        .playerSize = {40, 40},
        .gravity = 1000.0f,
        .playerOnGround = true
    };
    
    
    while(!WindowShouldClose())
    {
        
        
        // Update player
        {
         
            
            gameState.playerPos.y += gameState.playerVelocity * GetFrameTime();
            gameState.playerVelocity += gameState.gravity * GetFrameTime();
            
            if (IsKeyDown(KEY_SPACE) && gameState.playerOnGround) {
                gameState.playerVelocity = -1000.0f;
                gameState.playerOnGround = false;
            }
            
            if (gameState.playerPos.y < 350) {
                gameState.playerVelocity = 50.0f;
            }
            
           
            
            if(gameState.playerPos.y > 500){
                gameState.playerPos.y = 500;
                gameState.playerOnGround = true;
            }
            
            
//            if (defaultState.playerVelocity == false) {
//                defaultState.playerPos.y += GetFrameTime() * defaultState.gravity;
//            }
            
//            else{
//                defaultState.playerVelocity = false;
//            }

                    
        }
        
        
        BeginDrawing();
        ClearBackground(GRAY);
        
        // Draw player
        {
            DrawRectangleV(gameState.playerPos, gameState.playerSize, RED);
            DrawText(TextFormat("velocity : %f", gameState.playerVelocity), gameState.playerPos.x, gameState.playerPos.y - 20, 20, BLACK);
            DrawText(TextFormat("velocity : %d", gameState.playerOnGround), gameState.playerPos.x, gameState.playerPos.y - 60, 20, BLACK);
            DrawText(TextFormat("player y : %f", gameState.playerPos.y), gameState.playerPos.x, gameState.playerPos.y - 40, 20, BLACK);

        }
        
        
        
        EndDrawing();
        
    }
    
    CloseWindow();
    
    
    return 0;
    
    

    

    
}
