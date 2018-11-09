#pragma once
#include "../../resources/events/CharacterControllerEvent.h"

class StoreIntoInventoryEvent : public CharacterControllerEvent
{
	unsigned int mItemId;

public:
	StoreIntoInventoryEvent();
	~StoreIntoInventoryEvent();

	StoreIntoInventoryEvent* DoClone(const void* data) const override;
	
	unsigned int GetItemId() const;
	void SetItemId(unsigned int itemId);
};

