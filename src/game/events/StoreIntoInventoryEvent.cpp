#include "stdafx.h"
#include "StoreIntoInventoryEvent.h"
#include "../../resources/systems/InputSystem.h"

StoreIntoInventoryEvent::StoreIntoInventoryEvent() : mItem(nullptr)
{
}


StoreIntoInventoryEvent::~StoreIntoInventoryEvent()
{
}

StoreIntoInventoryEvent* StoreIntoInventoryEvent::DoClone(const void* data) const
{
	StoreIntoInventoryEvent* storeInventoryEvent = new StoreIntoInventoryEvent();
	storeInventoryEvent->SetItem(storeInventoryEvent->mItem);

	return storeInventoryEvent;
}

void StoreIntoInventoryEvent::SetItem(InventoryItem* item)
{
	mItem = item;
}

InventoryItem* StoreIntoInventoryEvent::GetItem() const
{
	return mItem;
}