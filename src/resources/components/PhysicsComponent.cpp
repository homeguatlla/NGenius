#include "stdafx.h"
#include "PhysicsComponent.h"
#include "../IGameEntity.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../../utils/serializer/XMLDeserializer.h"
#include "../Memory.h"
#include "../systems/PhysicsSystem.h"

PhysicsComponent::PhysicsComponent(bool isStatic, const glm::vec3& gravity) :mIsStatic(isStatic), mVelocity(0.0f), mGravity(gravity)
{
}


PhysicsComponent::~PhysicsComponent()
{
}

PhysicsComponent* PhysicsComponent::DoClone() const
{
	return DBG_NEW PhysicsComponent(*this);
}

const glm::vec3 PhysicsComponent::GetVelocity() const
{
	return mVelocity;
}

void PhysicsComponent::SetVelocity(glm::vec3& velocity)
{
	mVelocity = velocity;
}

const glm::vec3 PhysicsComponent::GetGravity() const
{
	return mGravity;
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

void PhysicsComponent::DoReadFrom(core::utils::IDeserializer* source)
{
	source->ReadParameter(std::string("is_static"), &mIsStatic);
	mGravity = PhysicsSystem::GRAVITY_VALUE;
	source->BeginAttribute("gravity");
	source->ReadParameter("X", &mGravity.x);
	source->ReadParameter("Y", &mGravity.y);
	source->ReadParameter("Z", &mGravity.z);
	source->EndAttribute();
}

void PhysicsComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("physics_component"));
	destination->WriteParameter(std::string("is_static"), mIsStatic);
	destination->WriteParameter(std::string("gravity"), mGravity);
}