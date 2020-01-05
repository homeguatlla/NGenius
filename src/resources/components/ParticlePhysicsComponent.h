#pragma once
#include "IComponent.h"
#include <glm/glm.hpp>
#include <string>
#include <memory>
#include "PhysicsComponent.h"

class IGameEntity;

namespace NPhysics
{
	class Particle;
	class PhysicsObject;
}

class ParticlePhysicsComponent : public PhysicsComponent
{
public:
	ParticlePhysicsComponent(bool isStatic, float mDensity, const glm::vec3& initialVelocity);
	virtual ~ParticlePhysicsComponent() = default;

	static std::string GetClassName() { return std::string("ParticlePhysicsComponent"); }
	static IComponent* Create(IGameEntity* entity);

private:
	ParticlePhysicsComponent() = default;
	ParticlePhysicsComponent* DoClone() const override;
	void DoCreatePhysicsData(const AABB& box, float mass) override;
};
