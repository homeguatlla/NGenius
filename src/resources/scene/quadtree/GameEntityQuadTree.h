#pragma once
#include "../ISpacePartition.h"
#include "DirectQuadTree.h"
#include "QuadTree.h"
#include "../../../AABB.h"

class IGameEntity;

class GameEntityQuadTree : public ISpacePartition
{
	QuadTree<IGameEntity>* mQuadTree;
public:
	static const int MAX_QUADTREE_LEVELS;

	GameEntityQuadTree(const AABB& boundingBox);
	virtual ~GameEntityQuadTree();

	void AddGameEntity(IGameEntity* entity) override;
	void RemoveGameEntity(IGameEntity* entity) override;
	void Query(const AABB& aabb, std::vector<IGameEntity*>& result) override;
	void Query(const AABB& aabb, const Frustum& frustum, std::vector<IGameEntity*>& result) override;
	unsigned int GetNumEntities() const override;
};

