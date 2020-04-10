#pragma once
#include <glm/glm.hpp>
#include "../../visitor/BaseVisitable.h"
#include "../scene/IGameSceneListener.h"
#include "../../../../NPhysics/NPhysicsEngine.h"
#include "../../../../NPhysics/source/particle/Particle.h"
#include <vector>

class IGameEntity;
class Terrain;
namespace NPhysics
{
	class IBoundingVolume;
}

class PhysicsSystem : public BaseVisitable<>, public IGameSceneListener
{
	std::vector<IGameEntity*> mEntities;
	const Terrain* mTerrain;
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

	//TODO eliminar este m�todo cuando no haga falta
	void SetTerrain(const Terrain* terrain);
	void SetEnergyWall(const glm::vec3& position, float radius);

	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

private:
	void AddEntity(IGameEntity* entity);
	void RemoveEntity(IGameEntity* entity);
	bool HasPhysicsComponents(const IGameEntity* entity) const;

	void AddGenerators(std::shared_ptr<NPhysics::Particle>& particle, IGameEntity* entity);
	void AddGenerators(std::shared_ptr<NPhysics::RigidBody>& rigidBody, std::shared_ptr<NPhysics::IBoundingVolume>& volume, IGameEntity* entity);

	void OnGameEntityAdded(IGameEntity* entity) override;
	void OnGameEntityRemoved(IGameEntity* entity) override;

	bool ApplyCollisions(IGameEntity* entity, float *groundHeight);
	bool ApplyEnergyWallCollision(IGameEntity*entity, glm::vec3& collisionPoint);
	void CheckCollisions(IGameEntity* entity);
	bool IsInsideTerrain(IGameEntity*entity);

	void CheckCollisionTerrain(IGameEntity* entity);
	void CheckCollisionEnergyWall(IGameEntity* entity);

	void UpdatePhysicsObjectsData(IGameEntity* entity);
	void UpdateEntitiesData(IGameEntity* entity);
};

