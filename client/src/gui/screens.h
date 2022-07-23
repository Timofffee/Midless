/**
 * Copyright (c) 2021-2022 Sirvoid
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef G_SCREEN_H
#define G_SCREEN_H

extern bool Screen_cursorEnabled;
extern bool Screen_showDebug;
extern int screenHeight;
extern int screenWidth;
extern Color uiColBg;

void Screens_init();

#endif