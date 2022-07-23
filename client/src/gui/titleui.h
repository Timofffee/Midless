/**
 * Copyright (c) 2021-2022 Sirvoid
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef G_TITLEUI_H
#define G_TITLEUI_H

typedef enum TitleScreen {
    TITLE_SCREEN_BASE,
    TITLE_SCREEN_LOGIN,
    TITLE_SCREEN_JOINING,
    TITLE_SCREEN_SETTINGS
} TitleScreen;

void TitleUi_Init(void);
void TitleUi_Deinit(void);
void TitleUi_Switch(TitleScreen screen);
void TitleUi_Draw(void);
void TitleUi_DrawBase(void);
void TitleUi_DrawLogin(void);
void TitleUi_DrawJoining(void);
void TitleUi_DrawSettings(void);

#endif