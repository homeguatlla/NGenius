#pragma once
#include <glm/glm.hpp>
#include "../../visitor/BaseVisitable.h"
#include "../scene/IGameSceneListener.h"
#include <vector>

class GameEntity;
class Terrain;

class EnvironmentSystem : public BaseVisitable<>, public IGameSceneListener
{
	std::vector<GameEntity*> mEntities;
	const Terrain* mTerrain;
	float mSpeed;
	float mAccumulatedTime;

public:
	EnvironmentSystem();
	~EnvironmentSystem();

	void Update(float deltaTime);
	
	unsigned int GetNumberGameEntities() const;

	void SetTerrain(const Terrain* terrain);
	
	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

private:
	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);
	bool HasEnvironmentComponents(const GameEntity* entity) const;

	void OnGameEntityAdded(GameEntity* entity) override;
	void OnGameEntityRemoved(GameEntity* entity) override;
};

