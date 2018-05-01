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

	void AddGameEntity(GameEntity* entity) override;
	void RemoveGameEntity(GameEntity* entity) override;
	std::vector<const GameEntity*> Query(const AABB& aabb) override;
};

