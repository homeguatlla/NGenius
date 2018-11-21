#include "stdafx.h"
#include "InventoryItem.h"
#include <glm/glm.hpp>


unsigned int InventoryItem::IDCounter = 0;

InventoryItem::InventoryItem(const std::string& name, ItemType  type, unsigned int counter, bool isAccumulative, unsigned int maxAccumulable) :
	mName(name),
	mType(type),
	mCounter(counter),
	mIsAccumulative(isAccumulative),
	mMaxAccumulable(maxAccumulable),
	mId(++IDCounter)
{
}


InventoryItem::~InventoryItem()
{
}

const std::string& InventoryItem::GetName() const
{
	return mName;
}

InventoryItem::ItemType InventoryItem::GetType() const
{
	return mType;
}

unsigned int InventoryItem::GetCounter() const
{
	return mCounter;
}

bool InventoryItem::IsAccumulative() const
{
	return mIsAccumulative;
}

unsigned int InventoryItem::GetId() const
{
	return mId;
}

unsigned int InventoryItem::Accumulate(unsigned int counter)
{
	unsigned int total = mCounter + counter;

	mCounter = glm::min(total, mMaxAccumulable);

	unsigned int remainder = total - mCounter;

	return remainder;
}

unsigned int InventoryItem::Acquire(unsigned int counter)
{
	int total = mCounter - counter;

	mCounter = glm::max(total, 0);

	return total < 0 ? -total : 0;
}
