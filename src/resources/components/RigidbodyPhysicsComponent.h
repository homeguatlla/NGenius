#pragma once
#include "PhysicsComponent.h"

class RigidbodyPhysicsComponent : public PhysicsComponent
{
public:
	static std::string GetClassName() { return std::string("RigidBodyPhysicsComponent"); }
	static IComponent* Create(IGameEntity* entity);

private:
	// Heredado vía PhysicsComponent
	IComponent* DoClone() const override;
	void DoCreatePhysicsData() override;
};

