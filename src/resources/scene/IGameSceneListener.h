#pragma once

class IGameEntity;

class IGameSceneListener
{
public:
	IGameSceneListener();
	~IGameSceneListener();

	virtual void OnGameEntityAdded(IGameEntity* entity) = 0;
	virtual void OnGameEntityRemoved(IGameEntity* entity) = 0;
};

