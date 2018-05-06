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
	virtual std::vector<GameEntity*>& Query(const AABB& aabb) = 0;
	virtual unsigned int GetNumEntities() const = 0;
};

