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
	
	//if has renderer, the size is the renderer size
	if (mParent->GetRenderer() != nullptr)
	{
		glm::vec3 min = mParent->GetRenderer()->GetModelAABB().GetVertexMin();
		glm::vec3 max = mParent->GetRenderer()->GetModelAABB().GetVertexMax();
		objectSize = max - min;
	}

	if (typeid(*mBoundingVolume.get()) == typeid(NPhysics::BoxBoundingVolume))
	{
		auto box = std::dynamic_pointer_cast<NPhysics::BoxBoundingVolume>(mBoundingVolume);
		auto transformation = mParent->GetTransformation();
		box->SetPosition(transformation->GetPosition() + mTranslation);
		box->SetSize(objectSize * transformation->GetScale() * mScale);
		box->SetRotation(transformation->GetRotation());
	}
	else if (typeid(*mBoundingVolume.get()) == typeid(NPhysics::SphereBoundingVolume))
	{
		auto sphere = std::dynamic_pointer_cast<NPhysics::SphereBoundingVolume>(mBoundingVolume);
		auto transformation = mParent->GetTransformation();
		auto size = objectSize * transformation->GetScale() * mScale;
		sphere->SetPosition(transformation->GetPosition() + mTranslation);
		sphere->SetRadius(glm::max(size.x, glm::max(size.y, size.z)) * 0.5f );
	}
	
	glm::vec3 position = mParent->GetTransformation()->GetPosition();
	glm::vec3 initialRotation = mParent->GetTransformation()->GetRotation();
	glm::vec3 initialAngularVelocity(0.0f);

	auto rigidBody = std::make_shared<NPhysics::RigidBody>(position + mTranslation, initialAngularVelocity, mInitialVelocity, mType);
	rigidBody->SetRotation(initialRotation);
	rigidBody->SetResitution(mRestitution);

	mObject = rigidBody;

	float volume = mBoundingVolume->GetVolume();
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
