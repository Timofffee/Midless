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
#include "gameloop.h"
#include "titleloop.h"

GameState Game_State = GAME_STATE_TITLE;
GameLoop Game_Loops[] = {
    (GameLoop) {TitleLoop_Init, TitleLoop_Deinit, TitleLoop_Update},
    (GameLoop) {GameLoop_Init, GameLoop_Deinit, GameLoop_Update}
};
bool exitProgram = false;

void Game_Init(void) {
    EntityModel_DefineAll();
    Block_BuildDefinition();
    Screens_init();

    Game_Loops[Game_State].init();

    // Game loop
    while (!WindowShouldClose() && !exitProgram) {
        if (!Game_Loops[Game_State].update()) return;
    }
    
    Game_Loops[Game_State].deinit();
}

void Game_ChangeState(GameState newState) {
    if (newState == Game_State) return;

    Game_Loops[Game_State].deinit();
    Game_State = newState;
    Game_Loops[Game_State].init();
}

void Game_Quit() {
    exitProgram = true;
}