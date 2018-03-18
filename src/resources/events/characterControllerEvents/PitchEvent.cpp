#include "stdafx.h"
#include "PitchEvent.h"
#include "../../systems/InputSystem.h"

PitchEvent::PitchEvent() : mPitch(0.0f)
{
}


PitchEvent::~PitchEvent()
{
}

PitchEvent* PitchEvent::DoClone(const void* data) const
{
	const InputSystem::MouseData* mouseData = reinterpret_cast<const InputSystem::MouseData*>(data);

	PitchEvent* pitchEvent = new PitchEvent();
	pitchEvent->SetPitch(mouseData->mCursorY);

	return pitchEvent;
}

void PitchEvent::SetPitch(float pitch)
{
	mPitch = pitch;
}

float PitchEvent::GetPitch() const
{
	return mPitch;
}