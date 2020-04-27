#pragma once
#include "src/resources/scene/ISpacePartition.h"
#include "DirectQuadTree.h"
#include "QuadTree.h"
#include "src/AABB.h"

class IGameEntity;

class GameEntityQuadTree : public ISpacePartition
{
	std::shared_ptr<QuadTree<IGameEntity>> mQuadTree;
public:
	static const int MAX_QUADTREE_LEVELS;

	GameEntityQuadTree(const AABB& boundingBox);
	virtual ~GameEntityQuadTree() = default;

	void AddGameEntity(std::shared_ptr<IGameEntity> entity) override;
	void RemoveGameEntity(std::shared_ptr<IGameEntity> entity) override;
	void Query(const AABB& aabb, std::vector<std::shared_ptr<IGameEntity>>& result) override;
	void Query(const AABB& aabb, const Frustum& frustum, std::vector<std::shared_ptr<IGameEntity>>& result) override;
	unsigned int GetNumEntities() const override;
};

