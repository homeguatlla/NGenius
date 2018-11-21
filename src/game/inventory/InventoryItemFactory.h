#pragma once

#include "InventoryItem.h"

class ItemFactory
{
public:
	ItemFactory();
	~ItemFactory();

	InventoryItem* Create(InventoryItem::ItemType type);
};

