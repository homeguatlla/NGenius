#pragma once
#include "../../resources/events/CharacterControllerEvent.h"

class Item;

class StoreIntoInventoryEvent : public CharacterControllerEvent
{
	Item* mItem;

public:
	StoreIntoInventoryEvent();
	~StoreIntoInventoryEvent();

	StoreIntoInventoryEvent* DoClone(const void* data) const override;
	
	Item* GetItem() const;
	void SetItem(Item* item);
};

