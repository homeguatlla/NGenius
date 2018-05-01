#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <assert.h>
#include <iostream>

template<class T>
class QuadTree
{
	typedef struct QuadTreeNode
	{
		glm::uvec2 mLocationCode;
		unsigned int mLevel;
		QuadTreeNode* mParent;
		QuadTreeNode* mChildren[4];
		std::vector<T> mData;
		QuadTreeNode(const glm::uvec2& _locationCode, unsigned int _level, QuadTreeNode* _parent) :
			mLocationCode(_locationCode), mLevel(_level), mParent(_parent)
		{
			for (int i = 0; i < 4; ++i)
			{
				mChildren[i] = nullptr;
			}
		}
		void AddChildren(unsigned int index, QuadTreeNode* child) 
		{
			assert(index < 4);
			mChildren[index] = child; 
		}
	}  QuadTreeNode;

	QuadTreeNode* mRoot;
	unsigned int mMaxLevels;
	glm::vec2 mRegionMin;
	glm::vec2 mRegionSize;
	float mConversionFactor;

public:
	QuadTree(const glm::vec2& regionMin, const glm::vec2& regionMax, unsigned int maxLevels) :
		mRoot(nullptr), 
		mMaxLevels(maxLevels) 
	{
		assert(mMaxLevels > 1);

		mRegionMin = regionMin;
		mRegionSize = regionMax - regionMin;
		mConversionFactor = static_cast<float>(glm::pow(2, mMaxLevels));

		mRoot = new QuadTreeNode(glm::uvec2(0), maxLevels - 1, nullptr);
	}
	~QuadTree() 
	{
	}

	void Add(const glm::vec2& regionMin, const glm::vec2& regionMax, T data)
	{
		glm::vec2 location1 = ((regionMin - mRegionMin) / mRegionSize);
		glm::vec2 location2 = ((regionMax - mRegionMin) / mRegionSize);

		glm::uvec2 locationCode1 = location1 * mConversionFactor;
		glm::uvec2 locationCode2 = location2 * mConversionFactor;

		unsigned int xDiff = locationCode1.x ^ locationCode2.x;
		unsigned int yDiff = locationCode1.y ^ locationCode2.y;

		unsigned int minLevel = FindMinLevel(xDiff, yDiff);
		std::cout << minLevel << "\n";

		/*
		QuadTreeNode* node = mRoot;
		for(int level = mMaxLevels - 2; level >= 0; level--)
		{
			unsigned int levelMask = 1 << level;
			unsigned int childIndex = ((xDiff & levelMask) >> level) + ((yDiff & levelMask) >> level);
			if (node->mChildren[childIndex] == nullptr)
			{
				node->AddChildren(childIndex, new QuadTreeNode(glm::uvec2(xDiff, yDiff), level, node));
			}
			node = node->mChildren[childIndex];
			std::cout << childIndex << ", ";
		}
		node->mData.push_back(data);

		std::cout << "\n";*/
	}
	template<class T>
	void Remove(T data)
	{

	}
	
	unsigned int FindMinLevel(unsigned int xDiff, unsigned int yDiff)
	{
		unsigned int level = mMaxLevels - 1;
		unsigned int minLevel = level;

		while (!(xDiff & (1 << level)) && level) level--;
		while (!(yDiff & (1 << minLevel)) && (minLevel > level)) minLevel--;

		return minLevel + 1;
	}

	std::vector<T>& Query(const glm::vec2& regionMin, const glm::vec2& regionMax)
	{
		glm::uvec2 locationCode1 = ((regionMin - mRegionMin) / mRegionSize) * mConversionFactor;
		glm::uvec2 locationCode2 = ((regionMax - mRegionMin) / mRegionSize) * mConversionFactor;

		unsigned int xDiff = locationCode1.x ^ locationCode2.x;
		unsigned int yDiff = locationCode1.y ^ locationCode2.y;

		unsigned int level = mMaxLevels - 1;
		unsigned int minLevel = level;

		while (!(xDiff & (1 << level)) && level) level--;
		while (!(yDiff & (1 << minLevel)) && (minLevel > level)) minLevel--;
		//minLevel++;

		//PHASE 2
		QuadTreeNode* node = mRoot;

		unsigned int nextLevel = mMaxLevels - 1;
		level = minLevel;
		unsigned int n = nextLevel - level + 1;
		while (n--)
		{
			unsigned int levelMask = 1 << nextLevel;
			unsigned int childIndex = ((locationCode1.x & levelMask) >> nextLevel) + ((locationCode1.y & levelMask) >> nextLevel);
			nextLevel--;
			QuadTreeNode* newNode = node->mChildren[childIndex];
			if (newNode == nullptr)
			{
				break;
			}
			else
			{
				node = newNode;
			}
		}
		assert(node != nullptr);

		return node->mData;
	}
};



