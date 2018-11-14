#pragma once
#include "../../resources/events/CharacterControllerEvent.h"

class Item;

class NextPreviousInventoryItemEvent : public CharacterControllerEvent
{
	bool mIsNext;

public:
	NextPreviousInventoryItemEvent();
	~NextPreviousInventoryItemEvent();

	NextPreviousInventoryItemEvent* DoClone(const void* data) const override;
	
	bool IsNext() const;
	void SetIsNext(bool isNext);
};

