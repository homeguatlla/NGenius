#include "stdafx.h"
#include "EnvironmentModificatorComponent.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../Memory.h"

EnvironmentModificatorComponent::EnvironmentModificatorComponent()
{
}


EnvironmentModificatorComponent::~EnvironmentModificatorComponent()
{
}

IComponent* EnvironmentModificatorComponent::Create()
{
	return DBG_NEW EnvironmentModificatorComponent();
}

EnvironmentModificatorComponent* EnvironmentModificatorComponent::DoClone() const
{
	return DBG_NEW EnvironmentModificatorComponent(*this);
}

void EnvironmentModificatorComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void EnvironmentModificatorComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("environment_modificator_component"));
}