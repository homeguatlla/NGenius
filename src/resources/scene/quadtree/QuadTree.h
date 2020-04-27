#pragma once
#include <glm/glm.hpp>
#include "../../../Frustum.h"
#include "../Memory.h"

#include <vector>
#include <assert.h>
#include <iostream>
#include <algorithm>

template<class T>
class QuadTree
{
	typedef struct Element
	{
		const glm::vec2 regionMin;
		const glm::vec2 regionMax;
		std::shared_ptr<T> data;

		Element(const glm::vec2& min, const glm::vec2& max, std::shared_ptr<T> _data) : regionMin(min), regionMax(max), data(_data) {}
		~Element() {}
	} Element;

	QuadTree* mChildren[4]; //top left, top right, bottom left, bottom right
	QuadTree* mParent;
	unsigned int mMaxLevels;
	glm::vec2 mRegionMin;
	glm::vec2 mRegionMax;
	std::vector<Element*> mElements;

public:
	QuadTree(const glm::vec2& regionMin, const glm::vec2& regionMax, unsigned int maxLevels, QuadTree* parent) :
		mRegionMin(regionMin),
		mRegionMax(regionMax),
		mMaxLevels(maxLevels),
		mParent(parent)
	{
		assert(mMaxLevels > 1);
		for (int i = 0; i < 4; ++i)
		{
			mChildren[i] = nullptr;
		}
	}

	~QuadTree()
	{
		for (int i = 0; i < 4; ++i)
		{
			if (mChildren[i] != nullptr)
			{
				delete mChildren[i];
			}
		}
	}

	unsigned int GetNumElements() const
	{
		return GetNumElements(this);
	}

	void Add(const glm::vec2& regionMin, const glm::vec2& regionMax, std::shared_ptr<T> data)
	{
		if (data != nullptr)
		{
			if (Contains(mRegionMin, mRegionMax, regionMin, regionMax))
			{
				glm::vec2 size = mRegionMax - mRegionMin;
				glm::vec2 size2 = size * 0.5f;
				if (glm::abs(size.x) <= 1.0f && glm::abs(size.y) <= 1.0f)
				{
					//we can not subdivide more
					mElements.push_back(new Element(regionMin, regionMax, data));
				}
				else
				{
					int isLeftPlane = regionMax.x < mRegionMin.x + size2.x;
					int isRightPlane = regionMin.x > mRegionMin.x + size2.x;
					int isTopPlane = regionMax.y > mRegionMin.y + size2.y;
					int isBottomPlane = regionMin.y < mRegionMin.y + size2.y;

					if ((isLeftPlane ^ isRightPlane) && (isTopPlane ^ isBottomPlane))
					{
						glm::vec2 planes(isRightPlane, isTopPlane);
						glm::vec2 newRegionMin = mRegionMin + planes * size2;
						int index = static_cast<int>(planes.x) + static_cast<int>(planes.y) * 2;
						QuadTree* quadTree = mChildren[index];
						if (quadTree == nullptr)
						{
							quadTree = DBG_NEW QuadTree(newRegionMin, newRegionMin + size2, mMaxLevels, mParent);
							mChildren[index] = quadTree;
						}
						quadTree->Add(regionMin, regionMax, data);
					}
					else
					{
						mElements.push_back(new Element(regionMin, regionMax, data));
					}
				}
			}
		}
	}

	void Remove(const glm::vec2& regionMin, const glm::vec2& regionMax, std::shared_ptr<T> data)
	{
		if (data != nullptr)
		{

		}
	}

	void Query(const glm::vec2& regionMin, const glm::vec2& regionMax, std::vector<std::shared_ptr<T>>& result)
	{
		if (Contains(mRegionMin, mRegionMax, regionMin, regionMax))
		{
			for (Element* element : mElements)
			{
				if (Contains(regionMin, regionMax, element->regionMin, element->regionMax))
				{
					result.push_back(element->data);
				}
			}
			for (int i = 0; i < 4; ++i)
			{
				if (mChildren[i] != nullptr)
				{
					mChildren[i]->Query(regionMin, regionMax, result);
				}
			}
		}
	}
	
	void Query(const glm::vec2& regionMin, const glm::vec2& regionMax, const Frustum& frustum, std::vector<std::shared_ptr<T>>& result)
	{
		if (Contains(mRegionMin, mRegionMax, regionMin, regionMax))
		{
			for (Element* element : mElements)
			{
				std::vector<glm::vec2> points;

				points.push_back(element->regionMin);
				points.push_back(glm::vec2(element->regionMax.x, element->regionMin.y));
				points.push_back(element->regionMax);
				points.push_back(glm::vec2(element->regionMin.x, element->regionMax.y));

				if (frustum.Intersect2D(points))
				{
					result.push_back(element->data);
				}
			}
			for (int i = 0; i < 4; ++i)
			{
				if (mChildren[i] != nullptr)
				{
					mChildren[i]->Query(regionMin, regionMax, frustum, result);
				}
			}
		}
	}

	private:

		unsigned int GetNumElements(const QuadTree* node) const
		{
			unsigned int count = 0;

			if (node != nullptr)
			{
				count = node->mElements.size();
				for (int i = 0; i < 4; ++i)
				{
					count += GetNumElements(node->mChildren[i]);
				}
			}

			return count;
		}

		bool Contains(const glm::vec2& region1Min, const glm::vec2& region1Max, const glm::vec2& region2Min, const glm::vec2& region2Max)
		{
			bool isOutside = region2Min.x > region1Max.x || region2Min.y > region1Max.y || region2Max.x < region1Min.x || region2Max.y < region1Min.y;

			return !isOutside;
		}
};



