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
#include "rlgl.h"
#include "player.h"
#include "world.h"
#include "screens.h"
#include "block/block.h"
#include "networking/networkhandler.h"
#include "chat.h"

#define GLSL_VERSION 330

int main(void) {
    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Midless");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(0);
    SetTraceLogLevel(LOG_WARNING);
    SetTargetFPS(60); 

    Image midlessLogo = LoadImage("textures/midless.png"); 
    SetWindowIcon(midlessLogo);

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
    Image titleTex = LoadImage("textures/TitleBackground.png"); 
    Texture2D texture = LoadTextureFromImage(terrainTex);
    Texture2D titleBackground = LoadTextureFromImage(titleTex);
    UnloadImage(terrainTex);
    UnloadImage(titleTex);
    
    World_ApplyTexture(texture);
    World_ApplyShader(shader);

    //Player Initialization
    Player_Init();
    
    bool exitProgram = false;
    Screens_init(texture, titleBackground, &exitProgram);

    // Game loop
    while (!WindowShouldClose() && !exitProgram) {
        
        Network_ReadQueue();

        screenHeight = GetScreenHeight();
        screenWidth = GetScreenWidth();
        
        // Update
        Player_Update();
        World_UpdateChunks();
        
        Vector3 selectionBoxPos = (Vector3) { floor(player.rayResult.hitPos.x) + 0.5f, floor(player.rayResult.hitPos.y) + 0.5f, floor(player.rayResult.hitPos.z) + 0.5f};
        
        // Draw
        BeginDrawing();

            ClearBackground((Color) { 140, 210, 240, 255});

            BeginMode3D(player.camera);
                World_Draw(player.camera.position);
                if(player.rayResult.hitBlockID != -1) 
                    DrawCube(selectionBoxPos, 1.01f, 1.01f, 1.01f, (Color){255, 255, 255, 40});
            EndMode3D();
            
            Screen_Make();

        EndDrawing();
    }
    
    Network_threadState = -1;

    UnloadShader(shader);
    UnloadTexture(texture);
    UnloadTexture(titleBackground);
    World_Unload();

    CloseWindow();

    return 0;
}