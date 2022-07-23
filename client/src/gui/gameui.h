/**
 * Copyright (c) 2021-2022 Sirvoid
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef G_GAMEUI_H
#define G_GAMEUI_H

typedef enum GameScreen {
    GAME_SCREEN_HUD,
    GAME_SCREEN_PAUSE,
    GAME_SCREEN_SETTINGS
} GameScreen;

void GameUi_Init(void);
void GameUi_Deinit(void);
void GameUi_Switch(GameScreen screen);
void GameUi_Draw(void);
void GameUi_DrawHUD(void);
void GameUi_DrawPause(void);
void GameUi_DrawSettings(void);

#endif