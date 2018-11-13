#include "stdafx.h"
#include "GameEventsComponent.h"
#include "../GameEvent.h"
#include <assert.h>

GameEventsComponent::GameEventsComponent()
{
}

GameEventsComponent::~GameEventsComponent()
{
}

GameEventsComponent* GameEventsComponent::DoClone() const
{
	return new GameEventsComponent(*this);
}

void GameEventsComponent::OnCharacterControllerEvent(const GameEvent* gameEvent)
{
	assert(gameEvent != nullptr);
	mEvents.push(gameEvent);
}

bool GameEventsComponent::HasEvents() const
{
	return !mEvents.empty();
}

const GameEvent* GameEventsComponent::ConsumeEvent()
{
	const GameEvent* event = mEvents.front();
	mEvents.pop();

	return event;
}