#include "stdafx.h"
#include "PickedUpComponent.h"


PickedUpComponent::PickedUpComponent(Item::ItemType type, unsigned int value) :
	mType(type),
	mValue(value)
{
}


PickedUpComponent::~PickedUpComponent()
{
}

Item::ItemType PickedUpComponent::GetType() const
{
	return mType;
}

unsigned int PickedUpComponent::GetValue() const
{
	return mValue;
}

IComponent * PickedUpComponent::DoClone() const
{
	return new PickedUpComponent(*this);
}
