#include "stdafx.h"
#include "SpacePartitionComponent.h"
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

IComponent* SpacePartitionComponent::Create()
{
	return new SpacePartitionComponent();
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