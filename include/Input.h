//
// Created by Thalins on 1/2/23.
//

#ifndef ORACLES_INPUT_H
#define ORACLES_INPUT_H

#include "types.h"

namespace oracle
{
constexpr auto MAX_KEYS = 1024;

bool WasKeyJustPressed(u32 key);
void SetKeyJustPressed(u32 key);

bool IsKeyHeld(u32 key);

bool WasKeyJustReleased(u32 key);
void SetKeyJustReleased(u32 key);

bool GetKeyState(u32 key);

void UpdateKeys();
}

#endif // ORACLES_INPUT_H
