/**
 * Copyright (c) 2021-2022 Sirvoid
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <raylib.h>
#include "game.h"

#define GLSL_VERSION 330

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int main(void) {
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Midless");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetExitKey(0);
    SetTraceLogLevel(LOG_WARNING);
    SetTargetFPS(60); 

    Image midlessLogo = LoadImage("textures/midless.png"); 
    SetWindowIcon(midlessLogo);

    Game_Init();

    CloseWindow();

    return 0;
}