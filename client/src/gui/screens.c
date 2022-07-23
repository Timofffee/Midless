/**
 * Copyright (c) 2021-2022 Sirvoid
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS

#include <raylib.h>
#include <raygui.h>
#include "screens.h"

bool Screen_cursorEnabled = false;
bool Screen_showDebug = false;
int screenHeight = 0;
int screenWidth = 0;
Color uiColBg = (Color){ 0 };

void Screens_init() {

    //Set UI colors
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL,    0xfffcfcff); 
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL,      0x00000000); 
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL,      0xffffffff); 
    GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED,   0x010101ff); 
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED,     0xfafafa00); 
    GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED,     0x000000ff); 
    GuiSetStyle(BUTTON, BORDER_COLOR_PRESSED,   0xfcffffff); 
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED,     0x00000000); 
    GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED,     0xffffffff); 

    GuiSetStyle(SLIDER, BORDER_COLOR_NORMAL,    0xfffcfcff); 
    GuiSetStyle(SLIDER, BASE_COLOR_NORMAL,      0x00000000); 
    GuiSetStyle(SLIDER, TEXT_COLOR_NORMAL,      0xffffffff); 
    GuiSetStyle(SLIDER, BORDER_COLOR_FOCUSED,   0xf1f1f1ff); 
    GuiSetStyle(SLIDER, TEXT_COLOR_FOCUSED,     0xf1f1f1ff); 
    GuiSetStyle(SLIDER, BASE_COLOR_PRESSED,     0xfcffffff); 
    GuiSetStyle(SLIDER, BORDER_COLOR_PRESSED,   0xfcffffff); 
    GuiSetStyle(SLIDER, TEXT_COLOR_PRESSED,     0xffffffff); 
    GuiSetStyle(SLIDER, BORDER_WIDTH,           2); 
    
    GuiSetStyle(PROGRESSBAR, BORDER_COLOR_NORMAL,   0xfffdfdff); 
    GuiSetStyle(PROGRESSBAR, BORDER_COLOR_PRESSED,  0xfbf8f8ff); 
    GuiSetStyle(PROGRESSBAR, BASE_COLOR_PRESSED,    0xf8fbfbff); 
    
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL,   0xf9f9f9ff); 
    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL,     0xfbfbfbff); 
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL,     0xfdf9f9ff); 
    GuiSetStyle(TEXTBOX, BASE_COLOR_FOCUSED,    0xc7effeff); 
    GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED,  0x0392c7ff); 
    GuiSetStyle(TEXTBOX, TEXT_COLOR_FOCUSED,    0xf9f9f9ff); 
    GuiSetStyle(TEXTBOX, BORDER_COLOR_PRESSED,  0xf9f9f9ff); 
    GuiSetStyle(TEXTBOX, BASE_COLOR_PRESSED,    0x00000011); 
    GuiSetStyle(TEXTBOX, TEXT_COLOR_PRESSED,    0xf9f9f9ff); 
}