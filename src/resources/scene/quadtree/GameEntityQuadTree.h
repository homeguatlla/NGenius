#pragma once
#include "../ISpacePartition.h"
#include "QuadTree.h"
#include "../../../AABB.h"

class GameEntity;

class GameEntityQuadTree : public ISpacePartition
{
	QuadTree<GameEntity*>* mQuadTree;
public:
	GameEntityQuadTree(const AABB& boundingBox);
	virtual ~GameEntityQuadTree();

	void AddGameEntity(GameEntity* entity) override;
	void RemoveGameEntity(GameEntity* entity) override;
	std::vector<GameEntity*>& Query(const AABB& aabb) override;
};

