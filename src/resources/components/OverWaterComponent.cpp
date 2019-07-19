#include "stdafx.h"
#include "OverWaterComponent.h"
#include "../GameEntity.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../Memory.h"
#include <glm/glm.hpp>

OverWaterComponent::OverWaterComponent(float waterHeight) :mWaterHeight(waterHeight)
{
}

OverWaterComponent::~OverWaterComponent()
{
}

OverWaterComponent* OverWaterComponent::DoClone() const
{
	return DBG_NEW OverWaterComponent(*this);
}

float OverWaterComponent::GetWaterHeight() const
{
	return mWaterHeight;
}

IComponent* OverWaterComponent::Create()
{
	return DBG_NEW OverWaterComponent();
}

void OverWaterComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void OverWaterComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("over_water_component"));
	destination->WriteParameter(std::string("water_height"), mWaterHeight);
}