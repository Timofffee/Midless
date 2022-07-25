/**
 * Copyright (c) 2021-2022 Sirvoid
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <raylib.h>
#include "gui/titleui.h"

#define START_MUSIC_DELAY 5.0f
#define RESTART_MUSIC_DELAY 30.0f

Music music;
float delayStartMusic = START_MUSIC_DELAY;

void TitleLoop_Init(void) {
    TitleUi_Init();

    music = LoadMusicStream("music/midless.mp3");
    music.looping = false;
}

bool TitleLoop_Update(void) {
    // Background Music
    if (IsMusicStreamPlaying(music)) {
        UpdateMusicStream(music);
    } else {
        if (delayStartMusic > 0) {
            delayStartMusic -= GetFrameTime();
        } else {
            PlayMusicStream(music);
            delayStartMusic = RESTART_MUSIC_DELAY;
        }
    }

    // Draw
    BeginDrawing();
        TitleUi_Draw();
    EndDrawing();

    return true;
}

void TitleLoop_Deinit(void) {
    TitleUi_Deinit();

    UnloadMusicStream(music);
}
