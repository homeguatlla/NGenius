#include "stdafx.h"
#include "OverWaterComponent.h"
#include "../GameEntity.h"

#include <glm/glm.hpp>

OverWaterComponent::OverWaterComponent(float waterHeight) :mWaterHeight(waterHeight)
{
}

OverWaterComponent::~OverWaterComponent()
{
}

OverWaterComponent* OverWaterComponent::DoClone() const
{
	return new OverWaterComponent(*this);
}

float OverWaterComponent::GetWaterHeight() const
{
	return mWaterHeight;
}