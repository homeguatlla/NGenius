#pragma once

class GameEntity;

class ISpacePartition
{
public:
	ISpacePartition();
	~ISpacePartition();

	virtual void AddGameEntity(const GameEntity* entity) = 0;
	virtual void RemoveGameEntity(const GameEntity* entity) = 0;
};

