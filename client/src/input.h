/**
 * Copyright (c) 2021-2022 Sirvoid
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#ifndef G_INPUT_H
#define G_INPUT_H

#include "raylib.h"

typedef enum InputAction {
    ACTION_MOVE_FORWARD,
    ACTION_MOVE_BACKWARD,
    ACTION_MOVE_LEFT,
    ACTION_MOVE_RIGHT,
    ACTION_JUMP,
    ACTION_RUN,
    ACTION_CANCEL,
    ACTION_END_ENUM
} InputAction;

void Input_Init(void);
bool Input_IsActionPressed(InputAction action);
bool Input_IsActionDown(InputAction action);

#endif