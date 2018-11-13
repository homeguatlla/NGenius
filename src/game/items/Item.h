#pragma once

#include <string>

class Item
{
public:
	enum ItemType
	{
		ITEM_SHOT_GUN,
		ITEM_LIVE_CAPSULE,
		ITEM_INVALID
	};
private:
	static unsigned int IDCounter;

	unsigned int mId;
	std::string mName;
	ItemType mType;
	unsigned int mCounter;
	bool mIsAccumulative;
	unsigned int mMaxAccumulable;

public:
	Item(const std::string& name, ItemType type, unsigned int counter, bool isAccumulative, unsigned int maxAccumulable);
	~Item();

	const std::string& GetName() const;
	Item::ItemType GetType() const;
	unsigned int GetCounter() const;
	bool IsAccumulative() const;
	unsigned int Accumulate(unsigned int counter);
	unsigned int Acquire(unsigned int counter);
	unsigned int GetId() const;
};

