#include "stdafx.h"
#include "RigidBodyPhysicsComponent.h"
#include "../Memory.h"
#include "../IGameEntity.h"
#include "../Transformation.h"
#include "../../../../NPhysics/source/rigidbody/RigidBody.h"

IComponent* RigidbodyPhysicsComponent::DoClone() const
{
	RigidbodyPhysicsComponent* newPhysicsComponent = DBG_NEW RigidbodyPhysicsComponent(*this);

	return newPhysicsComponent;
}

void RigidbodyPhysicsComponent::DoCreatePhysicsData()
{
	glm::vec3 position = mParent->GetTransformation()->GetPosition();
	mObject = std::make_shared<NPhysics::RigidBody>(position, mInitialVelocity);
}

IComponent* RigidbodyPhysicsComponent::Create(IGameEntity* entity)
{
	PhysicsComponent* component = DBG_NEW RigidbodyPhysicsComponent();
	entity->AddComponent(component);

	return component;
}
