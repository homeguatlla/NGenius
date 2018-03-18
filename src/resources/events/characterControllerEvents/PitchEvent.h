#pragma once
#include "../CharacterControllerEvent.h"

class PitchEvent : public CharacterControllerEvent
{
	float mPitch;

public:
	PitchEvent();
	~PitchEvent();

	PitchEvent* DoClone(const void* data) const override;
	
	float GetPitch() const;
	void SetPitch(float pitch);
};

