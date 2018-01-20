#pragma once
#include <glm/glm.hpp>
#include <vector>

class GameEntity;
class Terrain;

class PhysicsSystem
{
	std::vector<GameEntity*> mEntities;
	const Terrain* mTerrain;
	float mEnergyWallRadius;

public:
	static const glm::vec3 GRAVITY_VALUE;

	PhysicsSystem();
	~PhysicsSystem();

	void Update(float deltaTime);
	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);
	bool HasPhysicsComponents(GameEntity* entity) const;

	//TODO eliminar este método cuando no haga falta
	void SetTerrain(const Terrain* terrain);
	void SetEnergyWallRadius(float radius);

private:
	void ApplyMRU(float deltaTime, GameEntity* entity);
	bool ApplyCollisions(GameEntity* entity, float *groundHeight);
	bool ApplyEnergyWallCollision(GameEntity *entity, glm::vec3& collisionPoint);
	void CheckCollisions(GameEntity* entity);
	bool IsInsideTerrain(GameEntity *entity);

	void CheckCollisionTerrain(GameEntity* entity);
	void CheckCollisionEnergyWall(GameEntity* entity);
};

