#include "stdafx.h"
#include "EnvironmentModificatorComponent.h"


EnvironmentModificatorComponent::EnvironmentModificatorComponent()
{
}


EnvironmentModificatorComponent::~EnvironmentModificatorComponent()
{
}

EnvironmentModificatorComponent* EnvironmentModificatorComponent::DoClone() const
{
	return new EnvironmentModificatorComponent(*this);
}