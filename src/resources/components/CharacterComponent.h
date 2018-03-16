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

	void OnCharacterControllerEvent(const GameEvent* gameEvent);
	bool HasEvents() const;
	const GameEvent* ConsumeEvent();

	CharacterComponent* DoClone() const override;
};

