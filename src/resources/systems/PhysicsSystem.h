#pragma once
#include <glm/glm.hpp>
#include "../../visitor/BaseVisitable.h"
#include "../scene/IGameSceneListener.h"
#include <vector>

class IGameEntity;
class Terrain;

class PhysicsSystem : public BaseVisitable<>, public IGameSceneListener
{
	std::vector<IGameEntity*> mEntities;
	const Terrain* mTerrain;
	float mEnergyWallRadius;
	glm::vec3 mEnergyWallPosition;

public:
	static const glm::vec3 GRAVITY_VALUE;

	PhysicsSystem();
	~PhysicsSystem();

	void Update(float deltaTime);
	
	unsigned int GetNumberGameEntities() const;

	//TODO eliminar este método cuando no haga falta
	void SetTerrain(const Terrain* terrain);
	void SetEnergyWall(const glm::vec3& position, float radius);

	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

private:
	void AddEntity(IGameEntity* entity);
	void RemoveEntity(IGameEntity* entity);
	bool HasPhysicsComponents(const IGameEntity* entity) const;

	void OnGameEntityAdded(IGameEntity* entity) override;
	void OnGameEntityRemoved(IGameEntity* entity) override;

	void ApplyMRU(float deltaTime, IGameEntity* entity);
	bool ApplyCollisions(IGameEntity* entity, float *groundHeight);
	bool ApplyEnergyWallCollision(IGameEntity*entity, glm::vec3& collisionPoint);
	void CheckCollisions(IGameEntity* entity);
	bool IsInsideTerrain(IGameEntity*entity);

	void CheckCollisionTerrain(IGameEntity* entity);
	void CheckCollisionEnergyWall(IGameEntity* entity);
};

