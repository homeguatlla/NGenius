#include "stdafx.h"
#include "CharacterControllerEvent.h"

#include <typeinfo>
#include <assert.h>

CharacterControllerEvent::CharacterControllerEvent() : mIsPressed {false}
{
}

void CharacterControllerEvent::SetPressed(bool pressed)
{
	mIsPressed = pressed;
}

bool CharacterControllerEvent::IsPressed() const
{
	return mIsPressed;
}