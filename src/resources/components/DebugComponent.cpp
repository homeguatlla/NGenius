#include "stdafx.h"
#include "DebugComponent.h"

DebugComponent::DebugComponent() :
mIsBoundingBoxVisible(false)
{
}


DebugComponent::~DebugComponent()
{
}

DebugComponent* DebugComponent::DoClone() const
{
	return new DebugComponent(*this);
}

bool DebugComponent::IsBoundingBoxVisible() const
{
	return mIsBoundingBoxVisible;
}

void DebugComponent::SetBoundingBoxVisibility(bool visible)
{
	mIsBoundingBoxVisible = visible;
}