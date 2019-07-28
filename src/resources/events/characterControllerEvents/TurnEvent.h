#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class TurnEvent : public CharacterControllerEvent
{
	float mTurn;

public:
	TurnEvent();
	~TurnEvent();

	TurnEvent* DoClone(const void* data) const;
	static std::string GetClassName() { return std::string("TurnEvent"); }
	static GameEvent* Create();

	float GetTurn() const;
	void SetTurn(float turn);
};

