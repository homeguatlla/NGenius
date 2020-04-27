#pragma once
#include <glm/glm.hpp>
#include "src/visitor/BaseVisitable.h"
#include "src/resources/scene/IGameSceneListener.h"
#include "NPhysicsEngine.h"
#include "source/particle/Particle.h"
#include <vector>

class IGameEntity;
class Terrain;
namespace NPhysics
{
	class IBoundingVolume;
}

class PhysicsSystem : public BaseVisitable<>, public IGameSceneListener
{
	std::vector<std::shared_ptr<IGameEntity>> mEntities;
	std::shared_ptr<Terrain> mTerrain;
	float mEnergyWallRadius;
	glm::vec3 mEnergyWallPosition;

	NPhysics::NPhysicsEngine mEngine;
	bool mCanUpdate;

public:
	static const glm::vec3 GRAVITY_VALUE;

	PhysicsSystem();
	~PhysicsSystem();
	void Release();

	void Update(float deltaTime);
	void Reload();

	unsigned int GetNumberGameEntities() const;

	void EnableUpdate(bool enable);

	//TODO eliminar este método cuando no haga falta
	void SetTerrain(const std::shared_ptr<Terrain> terrain);
	void SetEnergyWall(const glm::vec3& position, float radius);

	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

private:
	void AddEntity(std::shared_ptr<IGameEntity> entity);
	void RemoveEntity(std::shared_ptr<IGameEntity> entity);
	bool HasPhysicsComponents(const std::shared_ptr<IGameEntity> entity) const;

	void AddGenerators(std::shared_ptr<NPhysics::Particle>& particle, std::shared_ptr<IGameEntity> entity);
	void AddGenerators(std::shared_ptr<NPhysics::RigidBody>& rigidBody, std::shared_ptr<NPhysics::IBoundingVolume>& volume, std::shared_ptr<IGameEntity> entity);

	void OnGameEntityAdded(std::shared_ptr<IGameEntity> entity) override;
	void OnGameEntityRemoved(std::shared_ptr<IGameEntity> entity) override;

	bool ApplyCollisions(std::shared_ptr<IGameEntity> entity, float *groundHeight);
	bool ApplyEnergyWallCollision(std::shared_ptr<IGameEntity> entity, glm::vec3& collisionPoint);
	void CheckCollisions(std::shared_ptr<IGameEntity> entity);
	bool IsInsideTerrain(std::shared_ptr<IGameEntity> entity);

	void CheckCollisionTerrain(std::shared_ptr<IGameEntity> entity);
	void CheckCollisionEnergyWall(std::shared_ptr<IGameEntity> entity);

	void UpdatePhysicsObjectsData(std::shared_ptr<IGameEntity> entity);
	void UpdateEntitiesData(std::shared_ptr<IGameEntity> entity);
};

