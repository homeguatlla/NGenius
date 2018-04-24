#pragma once
#include "../ISpacePartition.h"
#include "QuadTree.h"
#include "../../../AABB.h"

class GameEntity;

class GameEntityQuadTree : public ISpacePartition
{
	QuadTree<const GameEntity*>* mQuadTree;
public:
	GameEntityQuadTree(const AABB& boundingBox);
	virtual ~GameEntityQuadTree();

	void AddGameEntity(const GameEntity* entity);
	void RemoveGameEntity(const GameEntity* entity);
};

