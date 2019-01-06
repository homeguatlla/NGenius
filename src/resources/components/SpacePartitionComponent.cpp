#include "stdafx.h"
#include "SpacePartitionComponent.h"
#include "../../utils/serializer/XMLSerializer.h"

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

SpacePartitionComponent* SpacePartitionComponent::DoClone() const
{
	return new SpacePartitionComponent(*this);
}

void SpacePartitionComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void SpacePartitionComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("space_partition_component"));
}