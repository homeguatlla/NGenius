#include "stdafx.h"
#include "PickupGameEntityComponent.h"


PickupGameEntityComponent::PickupGameEntityComponent(InventoryItem::ItemType type, unsigned int value) :
	mType(type),
	mValue(value)
{
}


PickupGameEntityComponent::~PickupGameEntityComponent()
{
}

InventoryItem::ItemType PickupGameEntityComponent::GetType() const
{
	return mType;
}

unsigned int PickupGameEntityComponent::GetValue() const
{
	return mValue;
}

IComponent * PickupGameEntityComponent::DoClone() const
{
	return new PickupGameEntityComponent(*this);
}
