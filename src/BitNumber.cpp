#include "stdafx.h"
#include "BitNumber.h"
#include <assert.h>

BitNumber::BitNumber()
{
}


BitNumber::~BitNumber()
{
}

void BitNumber::SetDistance(uint64_t  id)
{
	mBits &= ~MASK_DISTANCE;
	uint64_t  number = id << 0 & MASK_DISTANCE;
	mBits |= number;
}

void BitNumber::SetModel(uint64_t  id)
{
	mBits &= ~MASK_MODEL;
	uint64_t  number = id << 32 & MASK_MODEL;
	mBits |= number;

	mBitsWithoutDistance = mBits;
}

void BitNumber::SetTexture(uint64_t  id)
{
	mBits &= ~MASK_TEXTURE;
	uint64_t  number = id << 40 & MASK_TEXTURE;
	mBits |= number;

	mBitsWithoutDistance = mBits;
}

void BitNumber::SetMaterial(uint64_t  id)
{
	mBits &= ~MASK_MATERIAL;
	uint64_t  number = id << 48 & MASK_MATERIAL;
	mBits |= number;

	mBitsWithoutDistance = mBits;
}

void BitNumber::SetLayer(uint64_t  id)
{
	mBits &= ~MASK_LAYER;
	uint64_t  number = id << 57 & MASK_LAYER;
	mBits |= number;

	mBitsWithoutDistance = mBits;
}

void BitNumber::SetTransparency(bool transparent)
{
	uint64_t  id = transparent ? 0 : 1;
	mBits &= ~MASK_TRANSPARENCY;
	uint64_t  number = id << 56 & MASK_TRANSPARENCY;
	mBits |= number;

	mBitsWithoutDistance = mBits;
}

uint64_t  BitNumber::GetValue() const
{
	return mBits.to_ullong();
}

uint64_t  BitNumber::GetValueWithoutDistance() const
{
	/*std::bitset<64> mBitsCopy(mBits);
	mBitsCopy &= ~MASK_DISTANCE;

	OPTIMIZATION
	it's faster to make a copy of mBits into mBitsWithoutDistance to mantain updated instead of doing the copy here
	*/
	return mBitsWithoutDistance.to_ullong();
}


std::string BitNumber::GetString() const
{
	return mBits.to_string();
}

void BitNumber::Test()
{
	BitNumber number;
	
	assert(number.GetString() == "0000000000000000000000000000000000000000000000000000000000000000");
	number.SetMaterial(1);
	assert(number.GetString() == "0000000000000001000000000000000000000000000000000000000000000000");
	number.SetMaterial(32);
	assert(number.GetString() == "0000000000100000000000000000000000000000000000000000000000000000");
	number.SetModel(4);
	assert(number.GetString() == "0000000000100000000000000000010000000000000000000000000000000000");
	number.SetMaterial(8);
	assert(number.GetString() == "0000000000001000000000000000010000000000000000000000000000000000");
	number.SetTexture(64);
	assert(number.GetString() == "0000000000001000010000000000010000000000000000000000000000000000");
	number.SetLayer(1);
	assert(number.GetString() == "0000001000001000010000000000010000000000000000000000000000000000");
	number.SetLayer(8);
	assert(number.GetString() == "0001000000001000010000000000010000000000000000000000000000000000");
	number.SetTransparency(false);
	assert(number.GetString() == "0001000100001000010000000000010000000000000000000000000000000000");
	number.SetTransparency(true);
	assert(number.GetString() == "0001000000001000010000000000010000000000000000000000000000000000");
	number.SetDistance(200);
	assert(number.GetString() == "0001000000001000010000000000010000000000000000000000000011001000");
	number.SetDistance(3200);
	assert(number.GetString() == "0001000000001000010000000000010000000000000000000000110010000000");
	number.SetDistance(65535);
	assert(number.GetString() == "0001000000001000010000000000010000000000000000001111111111111111");
}