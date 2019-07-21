#include "stdafx.h"
#include "EnvironmentAffectedComponent.h"
#include "../IGameEntity.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../Memory.h"

EnvironmentAffectedComponent::EnvironmentAffectedComponent() : 
	mIsAffectedByWind(false),
	mWindResistence(0.0f)
{
}


EnvironmentAffectedComponent::~EnvironmentAffectedComponent()
{
}

EnvironmentAffectedComponent* EnvironmentAffectedComponent::DoClone() const
{
	return DBG_NEW EnvironmentAffectedComponent(*this);
}

float EnvironmentAffectedComponent::GetWindResistence() const
{
	return mWindResistence;
}

void EnvironmentAffectedComponent::SetWindResistence(float resistence)
{
	mWindResistence = resistence;
}

void EnvironmentAffectedComponent::SetAffectedByWind(bool isAffected)
{
	mIsAffectedByWind = isAffected;
}

bool EnvironmentAffectedComponent::IsAffectedByWind() const
{
	return mIsAffectedByWind;
}

IComponent* EnvironmentAffectedComponent::Create(IGameEntity* entity)
{
	EnvironmentAffectedComponent* component = DBG_NEW EnvironmentAffectedComponent();
	entity->AddComponent(component);

	return component;
}

void EnvironmentAffectedComponent::DoReadFrom(core::utils::IDeserializer* source)
{
}

void EnvironmentAffectedComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("environment_affected_component"));
	destination->WriteParameter(std::string("is_affected_by_wind"), mIsAffectedByWind);
}
