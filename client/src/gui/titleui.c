/**
 * Copyright (c) 2021-2022 Sirvoid
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <pthread.h>
#include <math.h>
#include <raylib.h>
#include <raygui.h>
#include "screens.h"
#include "chat.h"
#include "../player.h"
#include "../world.h"
#include "../block/block.h"
#include "../networking/networkhandler.h"
#include "../networking/client.h"
#include "../worldgenerator.h"
#include "../game.h"
#include "titleui.h"

char name_input[16] = "Player";
char ip_input[16] = "127.0.0.1";
char port_input[5] = "25565";

bool login_editMode = false;
bool ip_editMode = false;
bool port_editMode = false;

TitleScreen TitleScreen_Current = TITLE_SCREEN_BASE;
Texture2D titleBackground;

void TitleUi_Init(void) {
    EnableCursor();

    Image titleTex = LoadImage("textures/TitleBackground.png"); 
    titleBackground = LoadTextureFromImage(titleTex);
    UnloadImage(titleTex);

    TitleScreen_Current = TITLE_SCREEN_BASE;
}

void TitleUi_Deinit(void) {
    UnloadTexture(titleBackground);
}

void TitleUi_Switch(TitleScreen screen) {
    TitleScreen_Current = screen;
}

void TitleUi_Draw(void) {
    screenHeight = GetScreenHeight();
    screenWidth = GetScreenWidth();
    
    uiColBg = (Color){ 0, 0, 0, 80 };
    
    switch (TitleScreen_Current)
    {
        case TITLE_SCREEN_JOINING:
            TitleUi_DrawJoining();
            break;
        case TITLE_SCREEN_BASE:
            TitleUi_DrawBase();
            break;
        case TITLE_SCREEN_LOGIN:
            TitleUi_DrawLogin();
            break;
        case TITLE_SCREEN_SETTINGS:
            TitleUi_DrawSettings();
            break;
        default:
            break;
    }
}

void TitleUi_DrawBase(void) {
    // Draw background
    float ratio = screenWidth / (float)screenHeight;
    float defaultRatio = 16.0f / 9;
    Rectangle titleBackgroundRect = (Rectangle){0, 0, titleBackground.width, titleBackground.height};
    if (ratio > defaultRatio) {
        titleBackgroundRect.height *= defaultRatio / ratio;
        titleBackgroundRect.y += (titleBackground.height - titleBackgroundRect.height) / 2;
    } else {
        titleBackgroundRect.width *= ratio / defaultRatio;
        titleBackgroundRect.x += (titleBackground.width - titleBackgroundRect.width) / 2;
    }
    DrawTexturePro(titleBackground, titleBackgroundRect, (Rectangle){0, 0, screenWidth, screenHeight}, (Vector2){ 0 }, 0, WHITE);

    // Draw Logo
    const char *title = "MIDLESS";
    int offsetY = screenHeight / 2;
    int offsetX = screenWidth / 2;

    DrawText(title, offsetX - (MeasureText(title, 80) / 2) + 4, offsetY - 150 + 4, 80, BLACK);
    DrawText(title, offsetX - (MeasureText(title, 80) / 2), offsetY - 150, 80, WHITE);

    //Singleplayer Button
    if(GuiButton((Rectangle) { offsetX - 100, offsetY - 10, 200, 30 }, "Singleplayer")) {
        Game_ChangeState(GAME_STATE_GAME);
    }

    //Login button
    if(GuiButton((Rectangle) { offsetX - 100, offsetY + 25, 200, 30 }, "Multiplayer")) {
        TitleUi_Switch(TITLE_SCREEN_LOGIN);
    }

    if(GuiButton((Rectangle) { offsetX - 100, offsetY + 60, 200, 30 }, "Settings")) {
        TitleUi_Switch(TITLE_SCREEN_SETTINGS);
    }
}

void TitleUi_DrawLogin(void) {
    int offsetY = screenHeight / 2;
    int offsetX = screenWidth / 2;

    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);

    //Name Input
    DrawRectangleRec((Rectangle) { offsetX - 100, offsetY - 65, 200, 30 }, (Color){0, 0, 0, 80});
    if(GuiTextBox((Rectangle) { offsetX - 100, offsetY - 65, 200, 30 }, name_input, 16, login_editMode)) {
        login_editMode = !login_editMode;
    }

    //IP Input
    DrawRectangleRec((Rectangle) { offsetX - 100, offsetY - 30, 136, 30 }, (Color){0, 0, 0, 80});
    if(GuiTextBox((Rectangle) { offsetX - 100, offsetY - 30, 136, 30 }, ip_input, 16, ip_editMode)) {
        ip_editMode = !ip_editMode;
    }

    //Port Input
    DrawRectangleRec((Rectangle) { offsetX + 40, offsetY - 30, 60, 30 }, (Color){0, 0, 0, 80});
    if(GuiTextBox((Rectangle) { offsetX + 40, offsetY - 30, 60, 30 }, port_input, 5, port_editMode)) {
        port_editMode = !port_editMode;
    }

    //Login button
    if(GuiButton((Rectangle) { offsetX - 100, offsetY + 5, 200, 30 }, "Login")) {
        TitleUi_Switch(TITLE_SCREEN_JOINING);
        Network_threadState = 0;
        Network_name = name_input;
        Network_ip = ip_input;
        Network_port = TextToInteger(port_input);

        //Start Client on a new thread
        pthread_t clientThread_id;
        pthread_create(&clientThread_id, NULL, Client_Init, (void*)&Network_threadState);
    }
    
    //Back Button
    if(GuiButton((Rectangle) { offsetX - 100, offsetY + 50, 200, 30 }, "Back")) {
        TitleUi_Switch(TITLE_SCREEN_BASE);
    }
}

void TitleUi_DrawJoining(void) {
    DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
    DrawText("Joining Server...", screenWidth / 2 - 80, screenHeight / 2 - 30, 20, WHITE);
}

void TitleUi_DrawSettings(void) {
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
        world.drawDistance = newDrawDistance;
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
        TitleUi_Switch(TITLE_SCREEN_BASE);
    }
}

