#include "stdafx.h"
#include "CharacterComponent.h"
#include "../GameEvent.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../Memory.h"

#include <assert.h>

CharacterComponent::CharacterComponent()
{
}

CharacterComponent::~CharacterComponent()
{
}

CharacterComponent* CharacterComponent::DoClone() const
{
	return DBG_NEW CharacterComponent(*this);
}

void CharacterComponent::OnCharacterControllerEvent(const GameEvent* gameEvent)
{
	assert(gameEvent != nullptr);
	mEvents.push(gameEvent);
}

bool CharacterComponent::HasEvents() const
{
	return !mEvents.empty();
}

const GameEvent* CharacterComponent::ConsumeEvent()
{
	const GameEvent* event = mEvents.front();
	mEvents.pop();

	return event;
}

IComponent* CharacterComponent::Create()
{
	return DBG_NEW CharacterComponent();
}

void CharacterComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void CharacterComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("character_component"));
}