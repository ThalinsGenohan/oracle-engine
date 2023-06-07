//
// Created by Thalins on 1/2/23.
//

#include "Input.h"

namespace oracle
{
static bool justPressedKeys[MAX_KEYS];
static bool heldKeys[MAX_KEYS];
static bool justReleasedKeys[MAX_KEYS];

bool WasKeyJustPressed(const u32 key)
{
	return justPressedKeys[key];
}

void SetKeyJustPressed(const u32 key)
{
	justPressedKeys[key] = true;
}

bool IsKeyHeld(const u32 key)
{
	return heldKeys[key];
}

bool WasKeyJustReleased(const u32 key)
{
	return justReleasedKeys[key];
}

void SetKeyJustReleased(const u32 key)
{
	heldKeys[key]         = false;
	justReleasedKeys[key] = true;
}

bool GetKeyState(const u32 key)
{
	return justPressedKeys[key] || heldKeys[key];
}

void UpdateKeys()
{
	for (u32 i = 0; i < MAX_KEYS; i++)
	{
		justReleasedKeys[i] = false;
		heldKeys[i]         = heldKeys[i] || justPressedKeys[i];
		justPressedKeys[i]  = false;
	}
}
}
