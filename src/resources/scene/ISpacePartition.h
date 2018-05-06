#pragma once

#include <vector>
#include "../../AABB.h"

class GameEntity;

class ISpacePartition
{
public:
	ISpacePartition();
	~ISpacePartition();

	virtual void AddGameEntity(GameEntity* entity) = 0;
	virtual void RemoveGameEntity(GameEntity* entity) = 0;
	virtual void Query(const AABB& aabb, std::vector<GameEntity*>& result) = 0;
	virtual unsigned int GetNumEntities() const = 0;
};

