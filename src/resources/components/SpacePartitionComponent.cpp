#include "stdafx.h"
#include "SpacePartitionComponent.h"


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
