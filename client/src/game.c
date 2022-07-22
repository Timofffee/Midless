/**
 * Copyright (c) 2021-2022 Sirvoid
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <raylib.h>
#include <raymath.h>
#include "rlgl.h"
#include "player.h"
#include "world.h"
#include "screens.h"
#include "block/block.h"
#include "networking/networkhandler.h"
#include "chat.h"
#include "game.h"

GameState Game_State = GAME_STATE_TITLE;

void Game_Init(void) {
    EntityModel_DefineAll();
    Block_BuildDefinition();

    // World Initialization
    World_Init();
    
    char *chunkShaderVs = 
        #include "chunk/chunk_shader.vs"
    ;
    
    char *chunkShaderFs = 
        #include "chunk/chunk_shader.fs"
    ;
    
    Shader shader = LoadShaderFromMemory(chunkShaderVs, chunkShaderFs);
    
    Image terrainTex = LoadImage("textures/terrain.png"); 
    Texture2D texture = LoadTextureFromImage(terrainTex);
    UnloadImage(terrainTex);

    Image titleTex = LoadImage("textures/TitleBackground.png"); 
    Texture2D titleBackground = LoadTextureFromImage(titleTex);
    UnloadImage(titleTex);
    
    World_ApplyTexture(texture);
    World_ApplyShader(shader);

    //Player Initialization
    Player_Init();
    
    bool exitProgram = false;
    Screens_init(texture, titleBackground, &exitProgram);

    // Game loop
    while (!WindowShouldClose() && !exitProgram) {
        // Update
        if (Game_State == GAME_STATE_GAME) {
            Network_ReadQueue();

            Player_Update();
            World_UpdateChunks();
        }
        
        // Draw
        BeginDrawing();

            if (Game_State == GAME_STATE_GAME) {
                ClearBackground((Color) { 140, 210, 240, 255});

                BeginMode3D(player.camera);
                    World_Draw(player.camera.position);
                    if(player.rayResult.hitBlockID != -1) {
                        Vector3 selectionBoxPos = (Vector3) { floor(player.rayResult.hitPos.x) + 0.5f, floor(player.rayResult.hitPos.y), floor(player.rayResult.hitPos.z) + 0.5f};
                        Block block = Block_GetDefinition(player.rayResult.hitBlockID);
                        Vector3 blockSize = Vector3Subtract(block.maxBB, block.minBB);
                        blockSize = Vector3Scale(blockSize, 1.0f / 16);
                        selectionBoxPos.y += blockSize.y / 2;
                        DrawCube(selectionBoxPos, blockSize.x + 0.01f, blockSize.y + 0.01f, blockSize.z + 0.01f, (Color){255, 255, 255, 40});
                    }
                        
                EndMode3D();
            }
            
            Screen_Make();

        EndDrawing();
    }
    
    Network_threadState = -1;

    UnloadShader(shader);
    UnloadTexture(texture);
    World_Unload();
}

void Game_ChangeState(GameState newState) {
    if (newState == Game_State) return;


    switch (newState)
    {
        case GAME_STATE_TITLE:
            if (Game_State == GAME_STATE_GAME) {
                Network_threadState = -1; //End network thread
                World_Unload();
            }
            break;
        case GAME_STATE_GAME:
            /* code */
            break;
        default:
            break;
    }

    Game_State = newState;
}