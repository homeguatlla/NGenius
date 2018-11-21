#pragma once
#include "IComponent.h"
#include <list>

class GameEvent;

class GameEventsComponent : public IComponent
{
	std::list<GameEvent*> mEvents;
	std::list<GameEvent*>::iterator mEventsIterator;
	std::list<GameEvent*>::iterator mPreviousEventsIterator;

public:
	explicit GameEventsComponent();
	virtual ~GameEventsComponent();

	void OnCharacterControllerEvent(GameEvent* gameEvent);

	void StartIterate();
	bool HasEvents() const;
	void ConsumeEvent();
	GameEvent* GetEvent();

	GameEventsComponent* DoClone() const override;
};

