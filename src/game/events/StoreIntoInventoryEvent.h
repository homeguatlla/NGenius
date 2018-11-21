#pragma once
#include "../../resources/events/CharacterControllerEvent.h"

class InventoryItem;

class StoreIntoInventoryEvent : public CharacterControllerEvent
{
	InventoryItem* mItem;

public:
	StoreIntoInventoryEvent();
	~StoreIntoInventoryEvent();

	StoreIntoInventoryEvent* DoClone(const void* data) const override;
	
	InventoryItem* GetItem() const;
	void SetItem(InventoryItem* item);
};

