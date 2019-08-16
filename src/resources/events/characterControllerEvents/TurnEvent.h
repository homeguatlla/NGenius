#pragma once
#include "../CharacterControllerEvent.h"
#include <string>

class TurnEvent : public CharacterControllerEvent
{
	float mTurn;

public:
	TurnEvent();
	~TurnEvent();

	std::shared_ptr<GameEvent> DoClone(const void* data) const;
	static std::string GetClassName() { return std::string("TurnEvent"); }
	static std::shared_ptr<GameEvent> Create();

	float GetTurn() const;
	void SetTurn(float turn);
};

