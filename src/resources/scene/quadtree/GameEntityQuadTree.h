#pragma once
#include "../ISpacePartition.h"
#include "QuadTree.h"
#include "../../../AABB.h"

class GameEntity;

class GameEntityQuadTree : public ISpacePartition
{
	QuadTree<GameEntity>* mQuadTree;
public:
	static const int MAX_QUADTREE_LEVELS;

	GameEntityQuadTree(const AABB& boundingBox);
	virtual ~GameEntityQuadTree();

	void AddGameEntity(GameEntity* entity) override;
	void RemoveGameEntity(GameEntity* entity) override;
	void Query(const AABB& aabb, std::vector<GameEntity*>& result) override;
	unsigned int GetNumEntities() const override;
};

