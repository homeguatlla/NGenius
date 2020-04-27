#pragma once

#include <vector>
#include <memory>

#include "src/AABB.h"
#include "src/Frustum.h"

class IGameEntity;

class ISpacePartition
{
public:
	ISpacePartition();
	~ISpacePartition();

	virtual void AddGameEntity(std::shared_ptr<IGameEntity> entity) = 0;
	virtual void RemoveGameEntity(std::shared_ptr<IGameEntity> entity) = 0;
	virtual void Query(const AABB& aabb, std::vector<std::shared_ptr<IGameEntity>>& result) = 0;
	virtual void Query(const AABB& aabb, const Frustum& frustum, std::vector<std::shared_ptr<IGameEntity>>& result) = 0;
	virtual unsigned int GetNumEntities() const = 0;
};

