#pragma once
#include "../../resources/scene/IGameSceneListener.h"
#include <vector>

class GameEntity;

class DamageSystem : public IGameSceneListener
{
	std::vector<GameEntity*> mEntities;
	typedef std::vector<GameEntity*>::iterator GameEntitiesIterator;
	bool mIsInitialized;

public:

	DamageSystem();
	~DamageSystem();

	void Start();

	void Update(float elapsedTime);

private:
	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);

	bool HasDamageComponents(const GameEntity* entity) const;

	void OnGameEntityAdded(GameEntity* entity) override;
	void OnGameEntityRemoved(GameEntity* entity) override;
};

