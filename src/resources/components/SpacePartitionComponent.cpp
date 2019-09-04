#include "stdafx.h"
#include "SpacePartitionComponent.h"
#include "../IGameEntity.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../Memory.h"

SpacePartitionComponent::SpacePartitionComponent() : mIsVisible(false)
{
}


SpacePartitionComponent::~SpacePartitionComponent()
{
}

bool SpacePartitionComponent::IsVisible() const
{
	return mIsVisible;
}

void SpacePartitionComponent::SetVisibility(bool visibility)
{
	mIsVisible = visibility;
}

IComponent* SpacePartitionComponent::Create(IGameEntity* entity)
{
	SpacePartitionComponent* component = new SpacePartitionComponent();
	entity->AddComponent(component);

	return component;
}

SpacePartitionComponent* SpacePartitionComponent::DoClone() const
{
	return DBG_NEW SpacePartitionComponent(*this);
}

void SpacePartitionComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void SpacePartitionComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("space_partition_component"));
}