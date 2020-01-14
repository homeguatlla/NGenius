#include "stdafx.h"
#include "RigidBodyPhysicsComponent.h"
#include "../Memory.h"
#include "../IGameEntity.h"
#include "../Transformation.h"
#include "../../../../NPhysics/source/rigidbody/RigidBody.h"
#include "../../../../NPhysics/NPhysicsEngine.h"

IComponent* RigidbodyPhysicsComponent::DoClone() const
{
	RigidbodyPhysicsComponent* newPhysicsComponent = DBG_NEW RigidbodyPhysicsComponent(*this);

	return newPhysicsComponent;
}

void RigidbodyPhysicsComponent::DoCreatePhysicsData(const AABB& box, float mass)
{
	glm::vec3 position = mParent->GetTransformation()->GetPosition();
	glm::vec3 initialRotation = mParent->GetTransformation()->GetRotation();
	auto rigidBody = std::make_shared<NPhysics::RigidBody>(position, initialRotation, mInitialVelocity);
	rigidBody->SetInertiaTensorMatrix(NPhysics::NPhysicsEngine::GetInertiaTensorMatrix(mass, box.GetSize()));

	mObject = rigidBody;
}

IComponent* RigidbodyPhysicsComponent::Create(IGameEntity* entity)
{
	PhysicsComponent* component = DBG_NEW RigidbodyPhysicsComponent();
	entity->AddComponent(component);

	return component;
}

void RigidbodyPhysicsComponent::AddForceAtBodyPoint(const glm::vec3& force, const glm::vec3& point)
{
	auto rigidbody = std::static_pointer_cast<NPhysics::RigidBody>(mObject);
	rigidbody->AddForceAtBodyPoint(force, point);
}
