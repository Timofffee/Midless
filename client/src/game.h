/**
 * Copyright (c) 2021-2022 Sirvoid
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef G_GAME_H
#define G_GAME_H

typedef enum GameState {
    GAME_STATE_TITLE,
    GAME_STATE_GAME
} GameState;

extern GameState Game_State;

void Game_Init(void);

void Game_ChangeState(GameState newState);

#endif