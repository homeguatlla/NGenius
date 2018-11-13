#pragma once
#include <vector>
#include "../items/Item.h"

class Inventory
{
	std::vector<Item*> mItemsList;
	unsigned int mMaxItems;
	
public:
	Inventory(unsigned int maxItems);
	~Inventory();

	void Store(Item* item);
	//unsigned int Retrieve(ItemType type, unsigned int counter);
	Item* Retrieve(unsigned int id);
	bool HasSpace() const;
	bool ExistItem(Item::ItemType type) const;
};

