#pragma once
#include <glm/glm.hpp>
#include "../../../visitor/BaseVisitable.h"
#include "../../scene/IGameSceneListener.h"
#include <vector>
#include <memory>

class GameEntity;
class Terrain;
class WindManager;

class EnvironmentSystem : public BaseVisitable<>, public IGameSceneListener
{
	std::vector<GameEntity*> mEntities;
	std::vector<GameEntity*> mModificators;
	const Terrain* mTerrain;
	std::unique_ptr<WindManager> mWindManager;
	float mTimer;
	std::vector<glm::vec3> mModificatorsPositions;

public:
	EnvironmentSystem();
	~EnvironmentSystem();

	void Update(float deltaTime);
	
	unsigned int GetNumberGameEntities() const;

	void SetTerrain(const Terrain* terrain);
	
	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

	float GetTimer() const;

private:
	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);
	void RemoveEntityVector(GameEntity* entity, std::vector<GameEntity*>& vector);
	bool HasEnvironmentComponents(const GameEntity* entity) const;

	void UpdateModificatorsVector();

	void OnGameEntityAdded(GameEntity* entity) override;
	void OnGameEntityRemoved(GameEntity* entity) override;
};

