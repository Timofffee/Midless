/**
 * Copyright (c) 2021-2022 Sirvoid
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <raylib.h>
#include "input.h"

KeyboardKey actions[ACTION_END_ENUM];

void Input_Init() {
    actions[ACTION_MOVE_FORWARD] = KEY_W;
    actions[ACTION_MOVE_BACKWARD] = KEY_S;
    actions[ACTION_MOVE_LEFT] = KEY_A;
    actions[ACTION_MOVE_RIGHT] = KEY_D;
    actions[ACTION_JUMP] = KEY_SPACE;
    actions[ACTION_RUN] = KEY_LEFT_SHIFT;
    actions[ACTION_CANCEL] = KEY_ESCAPE;
}

bool Input_IsActionPressed(InputAction action) {
    return IsKeyPressed(actions[action]);
}

bool Input_IsActionDown(InputAction action) {
    return IsKeyDown(actions[action]);
}