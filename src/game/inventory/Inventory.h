#pragma once
#include <vector>

class Inventory
{
	enum ItemType
	{
		ITEM_SHOT_GUN,
		ITEM_LIVE_CAPSULE,
		ITEM_INVALID
	};

	typedef struct Item
	{
		ItemType mType;
		unsigned int mCounter;
		bool mCanAccumulate;
		unsigned int mMaxAccumulable;

		Item(ItemType type, unsigned int counter, bool canAccumulate, unsigned int maxAccumulable)
		{
			mType = type;
			mCounter = counter;
			mCanAccumulate = canAccumulate;
			mMaxAccumulable = maxAccumulable;
		}
	} Item;

	std::vector<Item*> mItemsList;
	
public:
	Inventory();
	~Inventory();

	void Store(ItemType type, unsigned int counter);
	unsigned int Retrieve(ItemType type, unsigned int counter);
};

