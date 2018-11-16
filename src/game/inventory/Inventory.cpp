#include "stdafx.h"
#include "Inventory.h"
#include "../../utils/Log.h"

Inventory::Inventory(unsigned int maxItems) :
	mMaxItems(maxItems)
{
}


Inventory::~Inventory()
{
	for (Item* item : mItemsList)
	{
		delete item;
	}
	mItemsList.clear();
}

bool Inventory::HasSpace() const
{
	return mItemsList.size() < mMaxItems;
}

bool Inventory::ExistItem(Item::ItemType type) const
{
	for (Item* item : mItemsList)
	{
		if (item->GetType() == type)
		{
			return true;
		}
	}

	return false;
}


void Inventory::Store(Item* item)
{
	if (HasSpace())
	{
		mItemsList.push_back(item);
	}
}

Item* Inventory::Retrieve(unsigned int id)
{
	Item* item = nullptr;

	std::vector<Item*>::iterator it;
	for (it = mItemsList.begin(); it != mItemsList.end();)
	{
		if ((*it)->GetId() == id)
		{
			item = *it;
			it = mItemsList.erase(it);
			break;
		}
		else
		{
			++it;
		}
	}

	return item;
}
/*
unsigned int Inventory::Retrieve(ItemType type, unsigned int counter)
{
	unsigned int itemsPending = counter;

	for (Item* item : mItemsList)
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
	std::vector<Item*>::iterator it;
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