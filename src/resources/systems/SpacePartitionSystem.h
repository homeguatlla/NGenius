#pragma once

#include "../scene/GameScene.h"
#include "../scene/IGameSceneListener.h"
#include <vector>

class GameEntity;
class GameEntityQuadTree;

class SpacePartitionSystem : public IGameSceneListener
{
	std::vector<GameEntity*> mNewEntitiesToAdd;
	std::vector<GameEntity*> mEntitiesToRemove;
	GameEntityQuadTree* mQuadTree;

public:
	SpacePartitionSystem();
	~SpacePartitionSystem();

	void Start(const GameScene& gameScene);
	void Update(float elapsedTime);
	
private:
	bool HasSpacePartitionComponents(const GameEntity* entity);
	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);
	
	void OnGameEntityAdded(GameEntity* entity) override;
	void OnGameEntityRemoved(GameEntity* entity) override;
	
	void RemoveEntities();
	void AddNewEntities();
};

