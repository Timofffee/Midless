/**
 * Copyright (c) 2021-2022 Sirvoid
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <raylib.h>
#include "gui/titleui.h"

void TitleLoop_Init(void) {
    TitleUi_Init();
}

bool TitleLoop_Update(void) {
    // Draw
    BeginDrawing();
        TitleUi_Draw();
    EndDrawing();

    return true;
}

void TitleLoop_Deinit(void) {
    TitleUi_Deinit();
}
