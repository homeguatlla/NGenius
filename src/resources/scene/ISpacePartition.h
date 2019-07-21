#pragma once

#include <vector>
#include "../../AABB.h"
#include "../../Frustum.h"

class IGameEntity;

class ISpacePartition
{
public:
	ISpacePartition();
	~ISpacePartition();

	virtual void AddGameEntity(IGameEntity* entity) = 0;
	virtual void RemoveGameEntity(IGameEntity* entity) = 0;
	virtual void Query(const AABB& aabb, std::vector<IGameEntity*>& result) = 0;
	virtual void Query(const AABB& aabb, const Frustum& frustum, std::vector<IGameEntity*>& result) = 0;
	virtual unsigned int GetNumEntities() const = 0;
};

