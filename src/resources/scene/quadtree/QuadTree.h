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
		std::vector<T*> mData;
		QuadTreeNode(const glm::uvec2& _locationCode, unsigned int _level, QuadTreeNode* _parent) :
			mLocationCode(_locationCode), mLevel(_level), mParent(_parent)
		{
			for (int i = 0; i < 4; ++i)
			{
				mChildren[i] = nullptr;
			}
		}
		~QuadTreeNode()
		{
			for (int i = 0; i < 4; ++i)
			{
				if (mChildren[i] != nullptr)
				{
					delete mChildren[i];
				}
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
		delete mRoot;
	}

	void Add(const glm::vec2& regionMin, const glm::vec2& regionMax, T* data)
	{
		glm::uvec2 locationCode1 = CalculateLocationCode(regionMin);
		glm::uvec2 locationCode2 = CalculateLocationCode(regionMax);

		unsigned int xDiff = locationCode1.x ^ locationCode2.x;
		unsigned int yDiff = locationCode1.y ^ locationCode2.y;

		unsigned int minLevel = FindMinLevel(xDiff, yDiff);
		//std::cout << "diff(" << xDiff << " , " << yDiff << ")" << " level:" << minLevel << " ";
		//std::cout << " code: " << locationCode1.x << ", " << locationCode1.y << "\n";

		QuadTreeNode* node = TraverseToLevelFromRootCreatingNewNodes(mMaxLevels - 1, locationCode1, minLevel);
		if (node != nullptr)
		{
			node->mData.push_back(data);
		}
	}

	void Remove(const glm::vec2& regionMin, const glm::vec2& regionMax, T* data)
	{
		glm::uvec2 locationCode1 = CalculateLocationCode(regionMin);
		glm::uvec2 locationCode2 = CalculateLocationCode(regionMax);

		unsigned int xDiff = locationCode1.x ^ locationCode2.x;
		unsigned int yDiff = locationCode1.y ^ locationCode2.y;

		unsigned int minLevel = FindMinLevel(xDiff, yDiff);

		QuadTreeNode* node = TraverseToLevel(mRoot, mMaxLevels - 1, locationCode1, minLevel);

		if (node != nullptr)
		{
			std::vector<T*>::iterator it = std::find(node->mData.begin(), node->mData.end(), data);
			if (it != node->mData.end())
			{
				node->mData.erase(it);
			}
			else
			{
				assert(false);
			}
		}
	}

	unsigned int GetNumElements() const
	{
		return GetNumElements(mRoot);
	}

	std::vector<T*>& Query(const glm::vec2& regionMin, const glm::vec2& regionMax)
	{
		glm::uvec2 locationCode1 = CalculateLocationCode(regionMin);
		glm::uvec2 locationCode2 = CalculateLocationCode(regionMax);

		unsigned int xDiff = locationCode1.x ^ locationCode2.x;
		unsigned int yDiff = locationCode1.y ^ locationCode2.y;

		unsigned int minLevel = FindMinLevel(xDiff, yDiff);

		QuadTreeNode* node = TraverseToLevel(mRoot, mMaxLevels-1, locationCode1, minLevel);

		return node->mData;
	}

	private:
		glm::uvec2 CalculateLocationCode(const glm::vec2& region)
		{
			glm::vec2 location = ((region - mRegionMin) / mRegionSize);
			glm::uvec2 locationCode = location * mConversionFactor;

			return locationCode;
		}

		unsigned int FindMinLevel(unsigned int xDiff, unsigned int yDiff)
		{
			unsigned int level = mMaxLevels - 1;
			unsigned int minLevel = level;

			while (!(xDiff & (1 << level)) && level) level--;
			while (!(yDiff & (1 << minLevel)) && (minLevel > level)) minLevel--;

			return minLevel + 1;
		}
		
		QuadTreeNode*  TraverseToLevelFromRootCreatingNewNodes(unsigned int nextLevel, const glm::uvec2& locationCode, unsigned int level)
		{
			QuadTreeNode* finalNode = mRoot;
			unsigned int n = nextLevel - level + 1;

			//std::cout << "	indexs: \n";
			while (n--)
			{
				unsigned int childBitMask = 1 << nextLevel;
				unsigned int valueX = (locationCode.x & childBitMask) >> nextLevel;
				unsigned int valueY = (locationCode.y & childBitMask) >> (--nextLevel);
				unsigned int childIndex = valueX + valueY;
				//std::cout << "	(" << valueX << "," << valueY << ") index: " << childIndex << "\n";
				
				QuadTreeNode* nextNode = finalNode->mChildren[childIndex];
				if (nextNode == nullptr)
				{
					nextNode = new QuadTreeNode(locationCode, n, finalNode);
					finalNode->mChildren[childIndex] = nextNode;
				}

				finalNode = nextNode;
			}
			return finalNode;
		}

		QuadTreeNode* TraverseToLevel(QuadTreeNode* node, unsigned int nextLevel, const glm::uvec2& locationCode, unsigned int level)
		{
			QuadTreeNode* finalNode = node;

			unsigned int n = nextLevel - level + 1;
			while (n--)
			{
				unsigned int childBitMask = 1 << nextLevel;
				unsigned int valueX = (locationCode.x & childBitMask) >> nextLevel;
				unsigned int valueY = (locationCode.y & childBitMask) >> (--nextLevel);
				unsigned int childIndex = valueX + valueY;
				finalNode = finalNode->mChildren[childIndex];
			}

			return finalNode;
		}

		unsigned int GetNumElements(QuadTreeNode* node) const
		{
			unsigned int count = 0;

			if (node != nullptr)
			{
				count = node->mData.size();
				for (int i = 0; i < 4; ++i)
				{
					count += GetNumElements(node->mChildren[i]);
				}
			}

			return count;
		}
};



