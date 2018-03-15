#pragma once
#include "IComponent.h"

#include <queue>


class GameEvent;

class CharacterComponent : public IComponent
{
	std::queue<const GameEvent*> mEvents;

public:
	explicit CharacterComponent();
	virtual ~CharacterComponent();

	void OnGameEvent(const GameEvent* gameEvent);
	bool HasGameEvents() const;
	const GameEvent* ConsumeGameEvent();

	CharacterComponent* DoClone() const override;
};

