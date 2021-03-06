#include "stdafx.h"
#include "OverWaterComponent.h"
#include "../IGameEntity.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../Memory.h"
#include "../systems/renderSystem/RenderSystem.h"
#include <glm/glm.hpp>

OverWaterComponent::OverWaterComponent(float waterHeight) :mWaterHeight(waterHeight)
{
}

OverWaterComponent::~OverWaterComponent()
{
}

void OverWaterComponent::Init(GameScene* scene, RenderSystem* renderSystem)
{
	mWaterHeight = renderSystem->GetWaterHeight();
}

OverWaterComponent* OverWaterComponent::DoClone() const
{
	return DBG_NEW OverWaterComponent(*this);
}

float OverWaterComponent::GetWaterHeight() const
{
	return mWaterHeight;
}

IComponent* OverWaterComponent::Create(std::shared_ptr<IGameEntity> entity)
{
	OverWaterComponent* component = DBG_NEW OverWaterComponent();
	entity->AddComponent(component);

	return component;
}

void OverWaterComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void OverWaterComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("over_water_component"));
	destination->WriteParameter(std::string("water_height"), mWaterHeight);
}