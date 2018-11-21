#pragma once
#include <vector>
#include "../inventory/InventoryItem.h"

class Inventory
{
	std::vector<InventoryItem*> mItemsList;
	unsigned int mMaxItems;
	
public:
	Inventory(unsigned int maxItems);
	~Inventory();

	void Store(InventoryItem* item);
	//unsigned int Retrieve(ItemType type, unsigned int counter);
	InventoryItem* Retrieve(unsigned int id);
	bool HasSpace() const;
	bool ExistItem(InventoryItem::ItemType type) const;
};

