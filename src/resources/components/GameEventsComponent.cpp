#include "stdafx.h"
#include "GameEventsComponent.h"
#include "../GameEvent.h"
#include <assert.h>

GameEventsComponent::GameEventsComponent() : 
	mEventsIterator(mEvents.begin())
{
}

GameEventsComponent::~GameEventsComponent()
{
	
}

GameEventsComponent* GameEventsComponent::DoClone() const
{
	return new GameEventsComponent(*this);
}

void GameEventsComponent::OnCharacterControllerEvent(GameEvent* gameEvent)
{
	assert(gameEvent != nullptr);
	
	mEvents.push_back(gameEvent);
	StartIterate();
}

void GameEventsComponent::StartIterate()
{
	mEventsIterator = mEvents.begin();
	mPreviousEventsIterator = mEvents.begin();
}

bool GameEventsComponent::HasEvents() const
{
	return mEventsIterator != mEvents.end();
}

void GameEventsComponent::ConsumeEvent()
{
	if (mPreviousEventsIterator != mEvents.end())
	{
		GameEvent* event = *mPreviousEventsIterator;
		mEvents.erase(mPreviousEventsIterator);
		delete event;
	} 
}

GameEvent* GameEventsComponent::GetEvent()
{
	GameEvent* event = *mEventsIterator;
	
	if (HasEvents())
	{
		mPreviousEventsIterator = mEventsIterator;
		mEventsIterator++;
	}

	return event;
}