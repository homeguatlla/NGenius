#include "stdafx.h"
#include "Inventory.h"
#include "../../utils/Log.h"

Inventory::Inventory()
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


void Inventory::Store(ItemType type, unsigned int counter)
{
	Item* newItem = nullptr;

	switch (type)
	{
	case Inventory::ITEM_SHOT_GUN:
		newItem = new Item(Inventory::ITEM_SHOT_GUN, counter, false, 1);
		break;
	case Inventory::ITEM_LIVE_CAPSULE:
		newItem = new Item(Inventory::ITEM_LIVE_CAPSULE, counter, true, 10);
		break;
	case Inventory::ITEM_INVALID:
		Log(Log::LOG_WARNING) << "Trying to store an invalid item!\n";
		break;
	default:
		Log(Log::LOG_ERROR) << "Error storing an item into the inventory\n";
		break;
	}

	if (newItem != nullptr)
	{
		mItemsList.push_back(newItem);
	}
}

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
}