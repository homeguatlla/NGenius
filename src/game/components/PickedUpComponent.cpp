#include "stdafx.h"
#include "PickedUpComponent.h"


PickedUpComponent::PickedUpComponent()
{
}


PickedUpComponent::~PickedUpComponent()
{
}

IComponent * PickedUpComponent::DoClone() const
{
	return new PickedUpComponent(*this);
}
