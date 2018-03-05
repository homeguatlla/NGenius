#pragma once

#include <bitset>
#include <stdint.h>

class BitNumber
{
	//0000000		0			00000000  00000000  00000000 00000000 00000000
	//layer		transparency      shader   texture	 model		distance		
	
	const long long MASK_LAYER = 0xFE00000000000000;
	const long long MASK_TRANSPARENCY = 0x0100000000000000;
	const long long MASK_MATERIAL = 0x00FF000000000000;
	const long long MASK_TEXTURE = 0x0000FF0000000000;
	const long long MASK_MODEL = 0x000000FF00000000;
	const long long MASK_DISTANCE = 0x000000000000FFFF;

	std::bitset<64> mBits;
	std::bitset<64> mBitsWithoutDistance;


public:
	BitNumber();
	~BitNumber();

	void SetDistance(uint64_t  id);
	void SetModel(uint64_t  id);
	void SetTexture(uint64_t  id);
	void SetMaterial(uint64_t  id);
	void SetLayer(uint64_t  id);
	//false opaque, true transparent and then will be rendered opaque first
	void SetTransparency(bool transparent);

	uint64_t  GetValue() const;
	uint64_t  GetValueWithoutDistance() const;
	std::string GetString() const;

	bool operator < (const BitNumber& number) const
	{
		return GetValue() < number.GetValue();
	}

	void Test();
};

