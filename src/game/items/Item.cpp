#include "stdafx.h"
#include "Item.h"
#include <glm/glm.hpp>


unsigned int Item::IDCounter = 0;

Item::Item(const std::string& name, ItemType  type, unsigned int counter, bool isAccumulative, unsigned int maxAccumulable) :
	mName(name),
	mType(type),
	mCounter(counter),
	mIsAccumulative(isAccumulative),
	mMaxAccumulable(maxAccumulable),
	mId(++IDCounter)
{
}


Item::~Item()
{
}

const std::string& Item::GetName() const
{
	return mName;
}

Item::ItemType Item::GetType() const
{
	return mType;
}

unsigned int Item::GetCounter() const
{
	return mCounter;
}

bool Item::IsAccumulative() const
{
	return mIsAccumulative;
}

unsigned int Item::GetId() const
{
	return mId;
}

unsigned int Item::Accumulate(unsigned int counter)
{
	unsigned int total = mCounter + counter;

	mCounter = glm::min(total, mMaxAccumulable);

	unsigned int remainder = total - mCounter;

	return remainder;
}

unsigned int Item::Acquire(unsigned int counter)
{
	int total = mCounter - counter;

	mCounter = glm::max(total, 0);

	return total < 0 ? -total : 0;
}
