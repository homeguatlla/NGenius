#include "stdafx.h"
#include "Battery.h"
#include "../components/PickupGameEntityComponent.h"

Battery::Battery(InventoryItem::ItemType type, unsigned int power, GameEntity& entity) :
	mType(type),
	mPower(power),
	GameEntity(entity)
{
	AddComponent(new PickupGameEntityComponent(type, power));
}

Battery::~Battery()
{
}

InventoryItem::ItemType Battery::GetType() const
{
	return mType;
}

unsigned int Battery::GetPower() const
{
	return mPower;
}