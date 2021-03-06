#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "source/PhysicsObject.h"
#include "source/bvh/boundingVolumes/IBoundingVolume.h"
#include "source/PhysicsObject.h"
#include "../../AABB.h"

class IGameEntity;

class PhysicsComponent : public IComponent
{
protected:
	//parameters read from xml
	NPhysics::PhysicsType mType;
	glm::vec3 mInitialVelocity;
	glm::vec3 mLocalTranslation;
	glm::vec3 mLocalScale;
	glm::vec3 mLocalRotation;
	float mDensity;
	float mRestitution;
	glm::vec3 mSize;

	//real parameters
	std::shared_ptr<NPhysics::PhysicsObject> mObject;
	std::shared_ptr<NPhysics::IBoundingVolume> mBoundingVolume;

public:
	PhysicsComponent(NPhysics::PhysicsType type, float mDensity, const glm::vec3& initialVelocity);
	virtual ~PhysicsComponent() = default;

	void Init(GameScene* scene, RenderSystem* renderSystem) override;
	NPhysics::PhysicsType GetType() const;

	static std::string GetClassName() { return std::string("PhysicsComponent"); }
	
	//[[deprecated("Replaced by NPhysicsEngine, this both methods must be removed. Only physics engine will Set velocity")]]
	const glm::vec3 GetVelocity() const;
	//This translation, is an offset related the parent position
	//const glm::vec3 GetTranslation() const { return mTranslation; }
	void SetInitialVelocity(const glm::vec3& velocity);
	void SetVelocity(const glm::vec3& velocity);
	std::shared_ptr<NPhysics::PhysicsObject> GetPhysicsObject() const;
	std::shared_ptr<NPhysics::IBoundingVolume> GetPhysicsBoundingVolume() const;

protected:
	PhysicsComponent();
	void SetBoundingVolume(std::shared_ptr<NPhysics::IBoundingVolume> volume) { mBoundingVolume = volume; }
	void SetPhysicsObject(std::shared_ptr<NPhysics::PhysicsObject> object) { mObject = object; }

private:
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	static IComponent* DoCreate(std::shared_ptr<IGameEntity> entity) { return nullptr; }
	virtual void DoCreatePhysicsData() = 0;
	void ReadBoundingVolumeFrom(core::utils::IDeserializer* source);
};

