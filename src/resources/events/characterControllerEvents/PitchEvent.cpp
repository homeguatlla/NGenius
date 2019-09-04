#include "stdafx.h"
#include "PitchEvent.h"
#include "../../systems/InputSystem.h"
#include "../Memory.h"

PitchEvent::PitchEvent() : mPitch(0.0f)
{
}


PitchEvent::~PitchEvent()
{
}

std::shared_ptr<GameEvent> PitchEvent::DoClone(const void* data) const
{
	const InputSystem::MouseData* mouseData = reinterpret_cast<const InputSystem::MouseData*>(data);

	std::shared_ptr<PitchEvent> pitchEvent = std::make_shared<PitchEvent>();
	pitchEvent->SetPitch(static_cast<float>(mouseData->mCursorY));

	return pitchEvent;
}

std::shared_ptr<GameEvent> PitchEvent::Create()
{
	return std::make_shared<PitchEvent>();
}

void PitchEvent::SetPitch(float pitch)
{
	mPitch = pitch;
}

float PitchEvent::GetPitch() const
{
	return mPitch;
}