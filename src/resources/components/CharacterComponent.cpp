#include "stdafx.h"
#include "CharacterComponent.h"
#include "../GameEvent.h"
#include "../IGameEntity.h"
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

void CharacterComponent::OnCharacterControllerEvent(std::shared_ptr<const GameEvent> gameEvent)
{
	assert(gameEvent != nullptr);
	mEvents.push(gameEvent);
}

bool CharacterComponent::HasEvents() const
{
	return !mEvents.empty();
}

std::shared_ptr<const GameEvent> CharacterComponent::ConsumeEvent()
{
	std::shared_ptr<const GameEvent> event = mEvents.front();
	mEvents.pop();

	return event;
}

IComponent* CharacterComponent::Create(IGameEntity* entity)
{
	CharacterComponent* component = DBG_NEW CharacterComponent();
	entity->AddComponent(component);

	return component;
}

void CharacterComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void CharacterComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("character_component"));
}