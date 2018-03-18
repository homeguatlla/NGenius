#pragma once
#include "../CharacterControllerEvent.h"

class TurnEvent : public CharacterControllerEvent
{
	float mTurn;

public:
	TurnEvent();
	~TurnEvent();

	TurnEvent* DoClone(const void* data) const;

	float GetTurn() const;
	void SetTurn(float pitch);
};

