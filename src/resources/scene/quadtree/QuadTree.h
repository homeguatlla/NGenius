#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <assert.h>

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
		mConversionFactor = glm::pow(2, mMaxLevels - 1);

		mRoot = new QuadTreeNode(glm::uvec2(0), maxLevels - 1, nullptr);
	}
	~QuadTree() 
	{
	}

	void Add(const glm::vec2& regionMin, const glm::vec2& regionMax, T data)
	{
		glm::uvec2 locationCode1 = ((regionMin - mRegionMin) / mRegionSize) * mConversionFactor;
		glm::uvec2 locationCode2 = ((regionMax - mRegionMin) / mRegionSize) * mConversionFactor;

		unsigned int xDiff = locationCode1.x ^ locationCode2.x;
		unsigned int yDiff = locationCode1.y ^ locationCode2.y;

		QuadTreeNode* node = mRoot;
		for(int level = mMaxLevels; level >= 0; level--)
		{
			unsigned int levelMask = 1 << level;
			unsigned int childIndex = ((xDiff & levelMask) >> level) + ((yDiff & levelMask) >> level);
			if (node->mChildren[childIndex] == nullptr)
			{
				node->AddChildren(childIndex, new QuadTreeNode(glm::uvec2(0), level, node));
			}
			node = node->mChildren[childIndex];
		}
		//node->mData.push_back(data);
	}
	template<class T>
	void Remove(T data)
	{

	}
	/*
	std::vector<T> Query(const glm::vec2& regionMin, const glm::vec2& regionMax)
	{
		std::vector<T> datas;
		return datas;
	}*/
};



