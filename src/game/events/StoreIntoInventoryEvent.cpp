#include "stdafx.h"
#include "StoreIntoInventoryEvent.h"
#include "../../resources/systems/InputSystem.h"

StoreIntoInventoryEvent::StoreIntoInventoryEvent() : mItemId(0)
{
}


StoreIntoInventoryEvent::~StoreIntoInventoryEvent()
{
}

StoreIntoInventoryEvent* StoreIntoInventoryEvent::DoClone(const void* data) const
{
	StoreIntoInventoryEvent* storeInventoryEvent = new StoreIntoInventoryEvent();
	storeInventoryEvent->SetItemId(storeInventoryEvent->mItemId);

	return storeInventoryEvent;
}

void StoreIntoInventoryEvent::SetItemId(unsigned int itemId)
{
	mItemId = itemId;
}

unsigned int StoreIntoInventoryEvent::GetItemId() const
{
	return mItemId;
}