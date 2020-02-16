#include "stdafx.h"
#include "RigidBodyPhysicsComponent.h"
#include "../Memory.h"
#include "../IGameEntity.h"
#include "../Transformation.h"
#include "../renderers/IRenderer.h"
#include "../../AABB.h"
#include "../../../../NPhysics/source/rigidbody/RigidBody.h"
#include "../../../../NPhysics/NPhysicsEngine.h"
#include "../../../../NPhysics/source/bvh/boundingVolumes/SphereBoundingVolume.h"

IComponent* RigidbodyPhysicsComponent::DoClone() const
{
	RigidbodyPhysicsComponent* newPhysicsComponent = DBG_NEW RigidbodyPhysicsComponent(*this);

	return newPhysicsComponent;
}

void RigidbodyPhysicsComponent::DoCreatePhysicsData()
{
	if (mBoundingVolume == nullptr || mBoundingVolume->GetVolume() == 0.0f)
	{
		//Create a bounding volume by default with a radius circumscribe the boundingbox
		//TODO remove this code once we create a BoxBoundingVolume
		const AABB box = mParent->GetRenderer()->GetAABB();
		float largerSide = glm::distance(box.GetVertexMax(), box.GetVertexMin());
		float radiusCircumscribe = largerSide * 0.866f;
		if (mBoundingVolume == nullptr)
		{
			mBoundingVolume = std::make_shared<NPhysics::SphereBoundingVolume>(glm::vec3(0.0f), radiusCircumscribe);
		}
		else
		{
			auto sphereVolume = std::dynamic_pointer_cast<NPhysics::SphereBoundingVolume>(mBoundingVolume);
			sphereVolume->SetRadius(radiusCircumscribe);
		}
	}

	assert(mBoundingVolume);

	float volume = mBoundingVolume->GetVolume();
	float mass = mDensity * volume;

	glm::vec3 position = mParent->GetTransformation()->GetPosition();
	glm::vec3 initialRotation = mParent->GetTransformation()->GetRotation();
	glm::vec3 initialAngularVelocity(0.0f);
	auto rigidBody = std::make_shared<NPhysics::RigidBody>(position, initialAngularVelocity, mInitialVelocity);
	rigidBody->SetRotation(initialRotation);
	rigidBody->SetInertiaTensorMatrix(mBoundingVolume->GetInertiaTensorMatrix(mass));

	mObject = rigidBody;
	mObject->SetMass(mass);
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
