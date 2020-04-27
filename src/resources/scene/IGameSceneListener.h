#pragma once
#include <memory>

class IGameEntity;

class IGameSceneListener
{
public:
	IGameSceneListener();
	~IGameSceneListener();

	virtual void OnGameEntityAdded(std::shared_ptr<IGameEntity> entity) = 0;
	virtual void OnGameEntityRemoved(std::shared_ptr<IGameEntity> entity) = 0;
};

