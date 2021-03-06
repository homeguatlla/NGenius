#pragma once
#include "PhysicsComponent.h"

class RigidbodyPhysicsComponent : public PhysicsComponent
{
public:
	static std::string GetClassName() { return std::string("RigidbodyPhysicsComponent"); }
	static IComponent* Create(std::shared_ptr<IGameEntity> entity);

	void AddForceAtBodyPoint(const glm::vec3& force, const glm::vec3& point);

private:
	// Heredado v�a PhysicsComponent
	IComponent* DoClone() const override;
	void DoCreatePhysicsData() override;
};

