#include "stdafx.h"
#include "Inventory.h"
#include "../../utils/Log.h"

#include <algorithm>


Inventory::Inventory(unsigned int maxItems) :
	mMaxItems(maxItems)
{
}


Inventory::~Inventory()
{
	for (InventoryItem* item : mItemsList)
	{
		delete item;
	}
	mItemsList.clear();
}

bool Inventory::HasSpace() const
{
	return mItemsList.size() < mMaxItems;
}

bool Inventory::ExistItem(InventoryItem::ItemType type) const
{
	for (InventoryItem* item : mItemsList)
	{
		if (item->GetType() == type)
		{
			return true;
		}
	}

	return false;
}


void Inventory::Store(InventoryItem* item)
{
	if (HasSpace())
	{
		mItemsList.push_back(item);
	}
}

InventoryItem* Inventory::Retrieve(unsigned int id)
{
	InventoryItem* item = nullptr;

	std::vector<InventoryItem*>::iterator it = std::find_if(
		mItemsList.begin(), 
		mItemsList.end(), 
		[&](InventoryItem* item) { return item->GetId() == id; });

	if (it != mItemsList.end())
	{
		item = *it;
		it = mItemsList.erase(it);
	}

	return item;
}
/*
unsigned int Inventory::Retrieve(ItemType type, unsigned int counter)
{
	unsigned int itemsPending = counter;

	for (InventoryItem* item : mItemsList)
	{
		if (item->mType == type)
		{
			if (item->mCounter >= itemsPending)
			{
				item->mCounter -= itemsPending;
				itemsPending = 0;				
			}
			else if (item->mCounter < itemsPending)
			{
				itemsPending -= item->mCounter;
				item->mCounter = 0;
			}

			if (itemsPending == 0)
			{
				break;
			}
		}
	}

	//remove items where mCounter = 0;
	std::vector<InventoryItem*>::iterator it;
	for (it = mItemsList.begin(); it != mItemsList.end();)
	{
		if ((*it)->mCounter == 0)
		{
			it = mItemsList.erase(it);
		}
		else
		{
			++it;
		}
	}

	//if itemsPending = 0 successfuly all items have been retrieved
	//if itemsPending > 0 not all items have been retrieved, missing itemsPending items.
	return itemsPending;
}*/