#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "../../../../NPhysics/source/particle/Particle.h"

class IGameEntity;

class PhysicsComponent : public IComponent
{
	bool mIsStatic;
	glm::vec3 mInitialVelocity;
	float mMass;
	std::shared_ptr<NPhysics::Particle> mParticle;

public:
	PhysicsComponent(bool isStatic, float mass, const glm::vec3& initialVelocity);
	virtual ~PhysicsComponent() = default;

	void Init(GameScene* scene, RenderSystem* renderSystem) override;
	
	std::shared_ptr<NPhysics::Particle> GetParticle() const { return mParticle; }

	bool IsStatic() const;

	static std::string GetClassName() { return std::string("PhysicsComponent"); }
	static IComponent* Create(IGameEntity* entity);

	//[[deprecated("Replaced by NPhysicsEngine, this both methods must be removed. Only physics engine will Set velocity")]]
	const glm::vec3 GetVelocity() const { return mParticle->GetVelocity(); }
	void SetVelocity(glm::vec3& velocity);

private:
	PhysicsComponent() = default;
	void DoReadFrom(core::utils::IDeserializer* source) override;
	void DoWriteTo(core::utils::ISerializer* destination) override;
	PhysicsComponent* DoClone() const override;
	void CreatePhysicsData();
};

