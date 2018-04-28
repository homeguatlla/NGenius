#pragma once

class GameEntity;

class IGameSceneListener
{
public:
	IGameSceneListener();
	~IGameSceneListener();

	virtual void OnGameEntityAdded(GameEntity* entity) = 0;
	virtual void OnGameEntityRemoved(GameEntity* entity) = 0;
};

