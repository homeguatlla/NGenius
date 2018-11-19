#pragma once
#include <glm/glm.hpp>
#include "../../visitor/BaseVisitable.h"
#include "../scene/IGameSceneListener.h"
#include <vector>

class GameEntity;
class Terrain;

class PhysicsSystem : public BaseVisitable<>, public IGameSceneListener
{
	std::vector<GameEntity*> mEntities;
	const Terrain* mTerrain;
	float mEnergyWallRadius;
	glm::vec3 mEnergyWallPosition;
	glm::vec3 mGravity;

public:
	static const glm::vec3 GRAVITY_VALUE;

	PhysicsSystem();
	~PhysicsSystem();

	void Update(float deltaTime);
	
	unsigned int GetNumberGameEntities() const;

	//TODO eliminar este método cuando no haga falta
	void SetTerrain(const Terrain* terrain);
	void SetEnergyWall(const glm::vec3& position, float radius);

	float GetHeight(glm::vec2 point) const;

	const glm::vec3& GetGravity() const;
	void SetGravity(const glm::vec3& gravity);

	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

private:
	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);
	bool HasPhysicsComponents(const GameEntity* entity) const;

	void OnGameEntityAdded(GameEntity* entity) override;
	void OnGameEntityRemoved(GameEntity* entity) override;

	void ApplyMRU(float deltaTime, GameEntity* entity);
	bool ApplyCollisions(GameEntity* entity, float *groundHeight);
	bool ApplyEnergyWallCollision(GameEntity *entity, glm::vec3& collisionPoint);
	void CheckCollisions(GameEntity* entity);
	bool IsInsideTerrain(GameEntity *entity);

	void CheckCollisionTerrain(GameEntity* entity);
	void CheckCollisionEnergyWall(GameEntity* entity);
};

