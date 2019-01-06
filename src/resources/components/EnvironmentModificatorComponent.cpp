#include "stdafx.h"
#include "EnvironmentModificatorComponent.h"
#include "../../utils/serializer/XMLSerializer.h"

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

void EnvironmentModificatorComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void EnvironmentModificatorComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("environment_modificator_component"));
}