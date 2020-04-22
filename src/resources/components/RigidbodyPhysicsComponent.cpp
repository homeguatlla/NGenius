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

	return newPhysicsComponent;
}

void RigidbodyPhysicsComponent::DoCreatePhysicsData()
{
	/* to be removed after 13/06/2020 (time enough to know if is needed or not after a lot of executions)
	if (mBoundingVolume == nullptr || mBoundingVolume->GetVolume() == 0.0f)
	{
		//Create a bounding volume by default with a radius circumscribe the boundingbox
		//TODO remove this code once we create a BoxBoundingVolume
		const AABB box = mParent->GetRenderer()->GetAABB();
		float largerSide = glm::distance(box.GetVertexMax(), box.GetVertexMin());
		float radiusCircumscribe = largerSide * 0.5f;
		if (mBoundingVolume == nullptr)
		{
			glm::mat4 transformation = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));
			transformation = glm::scale(transformation, glm::vec3(radiusCircumscribe));
			mBoundingVolume = std::make_shared<NPhysics::SphereBoundingVolume>(transformation);
		}
		else
		{
			auto sphereVolume = std::dynamic_pointer_cast<NPhysics::SphereBoundingVolume>(mBoundingVolume);
			sphereVolume->SetRadius(radiusCircumscribe);
		}
	}*/

	assert(mBoundingVolume);
	assert(mBoundingVolume->GetVolume() > 0.0f);

	glm::vec3 min = mParent->GetRenderer()->GetModelAABB().GetVertexMin();
	glm::vec3 max = mParent->GetRenderer()->GetModelAABB().GetVertexMax();
	glm::vec3 objectSize = max - min;

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
