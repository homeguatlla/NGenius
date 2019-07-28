#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class PitchEvent : public CharacterControllerEvent
{
	float mPitch;

public:
	PitchEvent();
	~PitchEvent();

	PitchEvent* DoClone(const void* data) const override;
	static std::string GetClassName() { return std::string("PitchEvent"); }
	static GameEvent* Create();

	float GetPitch() const;
	void SetPitch(float pitch);
};

