#pragma once
#include "IComponent.h"
#include <queue>


class GameEvent;

class GameEventsComponent : public IComponent
{
	std::queue<const GameEvent*> mEvents;

public:
	explicit GameEventsComponent();
	virtual ~GameEventsComponent();

	void OnCharacterControllerEvent(const GameEvent* gameEvent);
	bool HasEvents() const;
	const GameEvent* ConsumeEvent();

	GameEventsComponent* DoClone() const override;
};

