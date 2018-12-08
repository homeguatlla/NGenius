#pragma once
#include "../CharacterControllerEvent.h"

class InventoryItem;

class DieEvent : public CharacterControllerEvent
{

public:
	DieEvent();
	~DieEvent();

	DieEvent* DoClone(const void* data) const override;
};

