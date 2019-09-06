#include "stdafx.h"
#include "PhysicsComponent.h"
#include "../IGameEntity.h"
#include "../Transformation.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../../utils/serializer/XMLDeserializer.h"
#include "../Memory.h"
#include "../systems/PhysicsSystem.h"


PhysicsComponent::PhysicsComponent(bool isStatic, float mass, const glm::vec3& initialVelocity) :mIsStatic(isStatic), mMass(mass), mInitialVelocity(initialVelocity)
{
}

void PhysicsComponent::Init(GameScene* scene, RenderSystem* renderSystem)
{
	CreatePhysicsData();
}

PhysicsComponent* PhysicsComponent::DoClone() const
{
	return DBG_NEW PhysicsComponent(*this);
}

bool PhysicsComponent::IsStatic() const
{
	return mIsStatic;
}

IComponent* PhysicsComponent::Create(IGameEntity* entity)
{
	PhysicsComponent* component = DBG_NEW PhysicsComponent();
	entity->AddComponent(component);

	return component;
}

void PhysicsComponent::SetVelocity(glm::vec3& velocity)
{
	mParticle->SetInitialVelocity(velocity);
}

void PhysicsComponent::CreatePhysicsData()
{
	glm::vec3 position = mParent->GetTransformation()->GetPosition();
	mParticle = std::make_shared<NPhysics::Particle>(position, mInitialVelocity);
	mParticle->SetMass(mMass);
}

void PhysicsComponent::DoReadFrom(core::utils::IDeserializer* source)
{
	source->ReadParameter(std::string("is_static"), &mIsStatic);
	source->ReadParameter(std::string("mass"), &mMass);
	mInitialVelocity = glm::vec3(0.0f);
	source->BeginAttribute("initialVelocity");
	source->ReadParameter("X", &mInitialVelocity.x);
	source->ReadParameter("Y", &mInitialVelocity.y);
	source->ReadParameter("Z", &mInitialVelocity.z);
	source->EndAttribute();
}

void PhysicsComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("physics_component"));
	destination->WriteParameter(std::string("is_static"), mIsStatic);
	destination->WriteParameter(std::string("initialVelocity"), mInitialVelocity);
}