#pragma once

#include "Item.h"

class ItemFactory
{
public:
	ItemFactory();
	~ItemFactory();

	Item* Create(Item::ItemType type);
};

