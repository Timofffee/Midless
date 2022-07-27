/**
 * Copyright (c) 2021-2022 Sirvoid
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <raylib.h>
#include <raymath.h>
#include "player.h"
#include "world.h"
#include "block/block.h"
#include "networking/networkhandler.h"
#include "game.h"
#include "gui/gameui.h"

Texture2D texture;
Shader shader;

void GameLoop_Init(void) {
    DisableCursor();

    GameUi_Init();

    // World Initialization
    World_Init();

    char *chunkShaderVs = 
        #include "chunk/chunk_shader.vs"
    ;
    
    char *chunkShaderFs = 
        #include "chunk/chunk_shader.fs"
    ;

    shader = LoadShaderFromMemory(chunkShaderVs, chunkShaderFs);
    World_ApplyShader(shader);

    Image terrainTex = LoadImage("textures/terrain.png"); 
    texture = LoadTextureFromImage(terrainTex);
    UnloadImage(terrainTex);
    World_ApplyTexture(texture);

    //Player Initialization
    Player_Init();
    
    World_LoadMap();
}

bool GameLoop_Update(void) {
    // Update
    Network_ReadQueue();

    Player_Update();
    World_UpdateChunks();
    
    // Draw
    BeginDrawing();

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

        GameUi_Draw();

    EndDrawing();

    return true;
}

void GameLoop_Deinit(void) {
    GameUi_Deinit();

    Network_threadState = -1;
    World_Unload();

    UnloadTexture(texture);
    UnloadShader(shader);
}
