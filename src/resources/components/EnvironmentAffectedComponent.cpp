#include "stdafx.h"
#include "EnvironmentAffectedComponent.h"


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
	return new EnvironmentAffectedComponent(*this);
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
