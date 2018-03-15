#include "stdafx.h"
#include "CharacterComponent.h"
#include "../GameEvent.h"

CharacterComponent::CharacterComponent()
{
}

CharacterComponent::~CharacterComponent()
{
}

CharacterComponent* CharacterComponent::DoClone() const
{
	return new CharacterComponent(*this);
}

void CharacterComponent::OnGameEvent(const GameEvent* gameEvent)
{
	mEvents.push(gameEvent);
}

bool CharacterComponent::HasGameEvents() const
{
	return !mEvents.empty();
}

const GameEvent* CharacterComponent::ConsumeGameEvent()
{
	const GameEvent* event = mEvents.front();
	mEvents.pop();

	return event;
}