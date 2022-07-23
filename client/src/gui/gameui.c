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
#include <raygui.h>
#include <raymath.h>
#include "rlgl.h"
#include "player.h"
#include "world.h"
#include "screens.h"
#include "block/block.h"
#include "networking/networkhandler.h"
#include "chat.h"
#include "game.h"
#include "gameui.h"

GameScreen GameScreen_Current = GAME_SCREEN_HUD;
Texture2D textureMap;

void GameUi_Init(void) {
    Image terrainTex = LoadImage("textures/terrain.png"); 
    textureMap = LoadTextureFromImage(terrainTex);
    UnloadImage(terrainTex);

    GameScreen_Current = GAME_SCREEN_HUD;
}

void GameUi_Deinit(void) {
    UnloadTexture(textureMap);
}

void GameUi_Switch(GameScreen screen) {
    GameScreen_Current = screen;

    if (GameScreen_Current == GAME_SCREEN_HUD) {
        DisableCursor();
    } else {
        EnableCursor();
    }
}

void GameUi_Draw(void) {
    screenHeight = GetScreenHeight();
    screenWidth = GetScreenWidth();
    
    uiColBg = (Color){ 0, 0, 0, 80 };
    
    switch (GameScreen_Current)
    {
        case GAME_SCREEN_HUD:
            GameUi_DrawHUD();
            break;
        case GAME_SCREEN_PAUSE:
            GameUi_DrawPause();
            break;
        case GAME_SCREEN_SETTINGS:
            GameUi_DrawSettings();
            break;
        default:
            break;
    }
}

void GameUi_DrawHUD(void) {

    //Draw debug infos
    const char* coordText = TextFormat("X: %i Y: %i Z: %i", (int)player.position.x, (int)player.position.y, (int)player.position.z);
    const char* camUp = TextFormat("X: %f Y: %f Z: %f", player.camera.target.x, player.camera.target.y, player.camera.target.z);
    const char* debugText;

    if(Screen_showDebug) {
        if(Network_connectedToServer) {
            debugText = TextFormat("%2i FPS %2i PING", GetFPS(), Network_ping);
        } else {
            debugText = TextFormat("%2i FPS", GetFPS());
        }
    
        int backgroundWidth = fmax(MeasureText(coordText, 20), MeasureText(debugText, 20));

        DrawRectangle(13, 15, backgroundWidth + 6, 39, uiColBg);
        DrawText(debugText, 16, 16, 20, WHITE);
        DrawText(coordText, 16, 36, 20, WHITE);
        DrawText(camUp, 16, 56, 20, WHITE);
    }

    //Draw crosshair
    DrawRectangle(screenWidth / 2 - 8, screenHeight / 2 - 2, 16, 4, uiColBg);
    DrawRectangle(screenWidth / 2 - 2, screenHeight / 2 + 2,  4, 6, uiColBg);
    DrawRectangle(screenWidth / 2 - 2, screenHeight / 2 - 8,  4, 6, uiColBg);

    //Draw Block Selected
    Block blockDef = Block_GetDefinition(player.blockSelected);
    int texI = blockDef.textures[4];
    int texX = texI % 16 * 16;
    int texY = texI / 16 * 16;

    Rectangle texRec = (Rectangle) {
        texX + 16 - blockDef.maxBB.x, 
        texY + 16 - blockDef.maxBB.y, 
        (blockDef.maxBB.x - blockDef.minBB.x), 
        (blockDef.maxBB.y - blockDef.minBB.y)
    };

    Rectangle destRec = (Rectangle) { 
        screenWidth - 80 + (blockDef.minBB.x * 4), 
        16 + ((16 - blockDef.maxBB.y) * 4), 
        (blockDef.maxBB.x - blockDef.minBB.x) * 4, 
        (blockDef.maxBB.y - blockDef.minBB.y) * 4
    };

    DrawTexturePro(textureMap, texRec, destRec, (Vector2) {0, 0}, 0, WHITE);

    //Draw Chat
    Chat_Draw((Vector2){16, screenHeight - 52}, uiColBg);
}

void GameUi_DrawPause(void) {
    DrawRectangle(0, 0, screenWidth, screenHeight, uiColBg);

    int offsetY = screenHeight / 2 - 75;
    int offsetX = screenWidth / 2 - 100;

    int index = 0;

    //Continue Button
    if(GuiButton((Rectangle) {offsetX , offsetY + (index++ * 35), 200, 30 }, "Continue")) {
        GameUi_Switch(GAME_SCREEN_HUD);
        DisableCursor();
        Screen_cursorEnabled = false;
    }

    //Options Button
    if(GuiButton((Rectangle) {offsetX, offsetY + (index++ * 35), 200, 30 }, "Settings")) {
        GameUi_Switch(GAME_SCREEN_SETTINGS);
    }

    //Main Menu Button
    if(GuiButton((Rectangle) {offsetX, offsetY + (index++ * 35), 200, 30 }, "Main Menu")) {
        Game_ChangeState(GAME_STATE_GAME);
        Screen_cursorEnabled = false;
    }

    index++; // Separator

    //Quit Button
    if(GuiButton((Rectangle) {offsetX, offsetY + (index++ * 35), 200, 30 }, "Quit")) {
        Game_Quit();
    }
}

void GameUi_DrawSettings(void) {
    DrawRectangle(0, 0, screenWidth, screenHeight, uiColBg);

    int offsetY = screenHeight / 2 - 30;
    int offsetX = screenWidth / 2 - 100;

    const char* drawDistanceTxt = TextFormat("Draw Distance: %i", world.drawDistance);

    //Draw distance Button
    int newDrawDistance = GuiSlider((Rectangle) {offsetX, offsetY - 15, 200, 30 }, "", "", world.drawDistance, 2, 8);
    Vector2 sizeText = MeasureTextEx(GetFontDefault(), drawDistanceTxt, 10.0f, 1);
    DrawTextEx(GetFontDefault(), drawDistanceTxt, (Vector2){offsetX + 100 - sizeText.x / 2 + 1, offsetY - sizeText.y / 2 + 1}, 10.0f, 1, BLACK);
    DrawTextEx(GetFontDefault(), drawDistanceTxt, (Vector2){offsetX + 100 - sizeText.x / 2, offsetY - sizeText.y / 2}, 10.0f, 1, WHITE);

    if (newDrawDistance != world.drawDistance) {
        if(newDrawDistance > world.drawDistance) {
            world.drawDistance = newDrawDistance;
            World_LoadChunks(false);
        } else {
            world.drawDistance = newDrawDistance;
            World_Reload();
        }
    }

    //Draw Debug Button
    const char* debugStateTxt = "OFF";
    if(Screen_showDebug) debugStateTxt = "ON";
    const char* showDebugTxt = TextFormat("Show Debug: %s", debugStateTxt);
    if(GuiButton((Rectangle) {offsetX, offsetY + 20, 200, 30 }, showDebugTxt)) {
        Screen_showDebug = !Screen_showDebug;
    }

    //Back Button
    if(GuiButton((Rectangle) {offsetX, offsetY + 55, 200, 30 }, "Back")) {
        GameUi_Switch(GAME_SCREEN_PAUSE);
    }
}