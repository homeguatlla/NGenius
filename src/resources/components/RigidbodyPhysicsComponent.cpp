#include "stdafx.h"
#include "RigidBodyPhysicsComponent.h"
#include "Memory.h"
#include "src/resources/IGameEntity.h"
#include "src/resources/Transformation.h"
#include "src/resources/renderers/IRenderer.h"
#include "src/AABB.h"
#include "source/rigidbody/RigidBody.h"
#include "NPhysicsEngine.h"
#include "source/bvh/boundingVolumes/SphereBoundingVolume.h"
#include "source/bvh/boundingVolumes/BoxBoundingVolume.h"
#include "source/utils/Math.h"
#include <glm/gtx/transform.hpp>

IComponent* RigidbodyPhysicsComponent::DoClone() const
{
	RigidbodyPhysicsComponent* newPhysicsComponent = DBG_NEW RigidbodyPhysicsComponent(*this);
	newPhysicsComponent->SetBoundingVolume(GetPhysicsBoundingVolume()->Clone());
	newPhysicsComponent->SetPhysicsObject(GetPhysicsObject()->Clone());

	return newPhysicsComponent;
}

void RigidbodyPhysicsComponent::DoCreatePhysicsData()
{
	assert(mBoundingVolume);
	assert(mBoundingVolume->GetVolume() > 0.0f);

	glm::vec3 objectSize = mSize;
	auto transformation = mParent->GetTransformation();
	
	//if has renderer, the size is the renderer size
	if (mParent->GetRenderer() != nullptr)
	{
		glm::vec3 min = mParent->GetRenderer()->GetModelAABB().GetVertexMin();
		glm::vec3 max = mParent->GetRenderer()->GetModelAABB().GetVertexMax();
		objectSize = (max - min) * transformation->GetScale();
	}
	
	if (typeid(*mBoundingVolume.get()) == typeid(NPhysics::BoxBoundingVolume))
	{
		auto box = std::dynamic_pointer_cast<NPhysics::BoxBoundingVolume>(mBoundingVolume);
		box->SetLocalTransformation(mLocalTranslation, mLocalScale, mLocalRotation);
		box->SetSize(objectSize);
	}
	else if (typeid(*mBoundingVolume.get()) == typeid(NPhysics::SphereBoundingVolume))
	{
		auto sphere = std::dynamic_pointer_cast<NPhysics::SphereBoundingVolume>(mBoundingVolume);
		sphere->SetLocalTransformation(mLocalTranslation, mLocalScale, mLocalRotation);
		sphere->SetRadius(glm::max(objectSize.x, glm::max(objectSize.y, objectSize.z)) * 0.5f );
	}
	
	glm::vec3 initialAngularVelocity(0.0f);
	mBoundingVolume->SetParentTransformation(transformation->GetPosition(), transformation->GetScale(), transformation->GetRotation());
	auto rigidBody = std::make_shared<NPhysics::RigidBody>(mBoundingVolume->GetPosition(), initialAngularVelocity, mInitialVelocity, mType);
	rigidBody->SetRotation(mLocalRotation);
	rigidBody->SetResitution(mRestitution);

	mObject = rigidBody;

	float volume = mBoundingVolume->GetVolume();
	assert(volume > 0.0f);
	float mass = mDensity * volume;

	//Although is a static object and its mass is infinite the inertia tensor matrix needs to be calculated 
	//with the value of its real mass (not infinite)
	rigidBody->SetInertiaTensorMatrix(mBoundingVolume->GetInertiaTensorMatrix(mass, false));

	if (mType == NPhysics::PhysicsType::kStatic)
	{
		mObject->SetInfiniteMass();
	}
	else
	{
		mObject->SetMass(mass);
	}
}

IComponent* RigidbodyPhysicsComponent::Create(std::shared_ptr<IGameEntity> entity)
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
