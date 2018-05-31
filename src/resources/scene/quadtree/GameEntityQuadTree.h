#pragma once
#include "../ISpacePartition.h"
#include "DirectQuadTree.h"
#include "../../../AABB.h"

class GameEntity;

class GameEntityQuadTree : public ISpacePartition
{
	DirectQuadTree<GameEntity>* mQuadTree;
public:
	static const int MAX_QUADTREE_LEVELS;

	GameEntityQuadTree(const AABB& boundingBox);
	virtual ~GameEntityQuadTree();

	void AddGameEntity(GameEntity* entity) override;
	void RemoveGameEntity(GameEntity* entity) override;
	void Query(const AABB& aabb, std::vector<GameEntity*>& result) override;
	void Query(const AABB& aabb, const Frustum& frustum, std::vector<GameEntity*>& result) override;
	unsigned int GetNumEntities() const override;
};

