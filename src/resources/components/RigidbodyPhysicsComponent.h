#pragma once
#include "PhysicsComponent.h"

class RigidbodyPhysicsComponent : public PhysicsComponent
{
public:
	static std::string GetClassName() { return std::string("RigidbodyPhysicsComponent"); }
	static IComponent* Create(IGameEntity* entity);

	void AddForceAtBodyPoint(const glm::vec3& force, const glm::vec3& point);

private:
	// Heredado vía PhysicsComponent
	IComponent* DoClone() const override;
	void DoCreatePhysicsData(const NPhysics::IBoundingVolume& volume, float mass) override;
};

