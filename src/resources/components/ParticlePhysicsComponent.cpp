#include "stdafx.h"
#include "ParticlePhysicsComponent.h"
#include "src/resources/IGameEntity.h"
#include "src/resources/Transformation.h"
#include "src/resources/renderers/IRenderer.h"
#include "Memory.h"
#include "src/resources/systems/PhysicsSystem.h"
#include "source/bvh/boundingVolumes/SphereBoundingVolume.h"
#include "source/PhysicsObject.h"


ParticlePhysicsComponent::ParticlePhysicsComponent(float density, const glm::vec3& initialVelocity) : 
	PhysicsComponent(NPhysics::PhysicsType::kDynamic, density, initialVelocity)
{
}

ParticlePhysicsComponent* ParticlePhysicsComponent::DoClone() const
{
	ParticlePhysicsComponent* newPhysicsComponent = DBG_NEW ParticlePhysicsComponent(*this);

	return newPhysicsComponent;
}

IComponent* ParticlePhysicsComponent::Create(IGameEntity* entity)
{
	PhysicsComponent* component = DBG_NEW ParticlePhysicsComponent();
	entity->AddComponent(component);

	return component;
}

void ParticlePhysicsComponent::DoCreatePhysicsData()
{
	if (mBoundingVolume == nullptr)
	{
		//Create a bounding volume by default
		mBoundingVolume = std::make_shared<NPhysics::SphereBoundingVolume>(glm::vec3(0.0f), 1.0f);
	}

	assert(mBoundingVolume);

	float volume = mBoundingVolume->GetVolume();
	float mass = mDensity * volume;

	glm::vec3 position = mParent->GetTransformation()->GetPosition();
	mObject = std::make_shared<NPhysics::Particle>(position, mInitialVelocity);
	mObject->SetMass(mass);
}