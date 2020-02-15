#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "../../../../NPhysics/source/PhysicsObject.h"
#include "../../../../NPhysics/source/bvh/boundingVolumes/IBoundingVolume.h"
#include "../../AABB.h"

class IGameEntity;

class PhysicsComponent : public IComponent
{
protected:
	bool mIsStatic;
	glm::vec3 mInitialVelocity;
	float mDensity;
	std::shared_ptr<NPhysics::PhysicsObject> mObject;
	std::shared_ptr<NPhysics::IBoundingVolume> mBoundingVolume;

public:
	PhysicsComponent(bool isStatic, float mDensity, const glm::vec3& initialVelocity);
	virtual ~PhysicsComponent() = default;

	void Init(GameScene* scene, RenderSystem* renderSystem) override;
	bool IsStatic() const;

	static std::string GetClassName() { return std::string("PhysicsComponent"); }
	
	//[[deprecated("Replaced by NPhysicsEngine, this both methods must be removed. Only physics engine will Set velocity")]]
	const glm::vec3 GetVelocity() const;
	void SetInitialVelocity(const glm::vec3& velocity);
	void SetVelocity(const glm::vec3& velocity);
	std::shared_ptr<NPhysics::PhysicsObject> GetPhysicsObject() const;
	std::shared_ptr<NPhysics::IBoundingVolume> GetPhysicsBouningVolume() const;

protected:
	PhysicsComponent() = default;

private:
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	static IComponent* DoCreate(IGameEntity* entity) { return nullptr; }
	virtual void DoCreatePhysicsData(const NPhysics::IBoundingVolume& volume, float mass) = 0;
	void ReadBoundingVolumeFrom(core::utils::IDeserializer* source);
};

