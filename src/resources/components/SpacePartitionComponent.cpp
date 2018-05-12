#include "stdafx.h"
#include "SpacePartitionComponent.h"


SpacePartitionComponent::SpacePartitionComponent()
{
}


SpacePartitionComponent::~SpacePartitionComponent()
{
}

SpacePartitionComponent* SpacePartitionComponent::DoClone() const
{
	return new SpacePartitionComponent(*this);
}
