#include "stdafx.h"
#include "CharacterComponent.h"
#include "src/resources/GameEvent.h"
#include "src/resources/IGameEntity.h"
#include "src/utils/serializer/XMLSerializer.h"
#include "Memory.h"

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

void CharacterComponent::OnCharacterControllerEvent(std::shared_ptr<GameEvent> gameEvent)
{
	assert(gameEvent != nullptr);
	mEvents.push(gameEvent);
}

bool CharacterComponent::HasEvents() const
{
	return !mEvents.empty();
}

std::shared_ptr<GameEvent> CharacterComponent::ConsumeEvent()
{
	std::shared_ptr<GameEvent> event = mEvents.front();
	mEvents.pop();

	return event;
}

void CharacterComponent::ClearEvents()
{
	//The way to clear a queue is to swap by a new empty queue
	std::swap(mEvents, std::queue<std::shared_ptr<GameEvent>>());
}

IComponent* CharacterComponent::Create(std::shared_ptr<IGameEntity> entity)
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