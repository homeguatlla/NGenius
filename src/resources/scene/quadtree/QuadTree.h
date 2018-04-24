#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <assert.h>

template<class T>
class QuadTree
{
	typedef struct QuadTreeNode
	{
		glm::vec2 mLocationMinCode;
		glm::vec2 mLocationMaxCode;
		unsigned int mLevel;
		QuadTreeNode* mParent;
		QuadTreeNode* mChildren[4];
		std::vector<T*> mData;
		QuadTreeNode(const glm::vec2& _locationMin, const glm::vec2& _locationMax, unsigned int _level, QuadTreeNode* _parent) :
			mLocationMinCode(_locationMin), mLocationMaxCode(_locationMax), mLevel(_level), mParent(_parent) {}
		void AddChildren(unsigned int index, QuadTreeNode* child) 
		{
			assert(index > 3);
			mChildren[index] = child; 
		}
	}  QuadTreeNode;

	QuadTreeNode* mRoot;
	unsigned int mMaxLevels;

public:
	QuadTree(glm::vec2& regionMin, glm::vec2& regionMax, unsigned int maxLevels) :
		mRoot(new QuadTreeNode(regionMin, regionMax, maxLevels-1, nullptr)), mMaxLevels(maxLevels) {}
	~QuadTree() 
	{

	}

	void Add(glm::vec2& regionMin, glm::vec2& regionMax, T data)
	{
		
	}

	void Remove(T data)
	{

	}
};



