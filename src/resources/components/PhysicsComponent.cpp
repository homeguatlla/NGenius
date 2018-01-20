#include "stdafx.h"
#include "PhysicsComponent.h"


PhysicsComponent::PhysicsComponent(bool isStatic, const glm::vec3& gravity) :mIsStatic(isStatic), mVelocity(0.0f), mGravity(gravity)
{
}


PhysicsComponent::~PhysicsComponent()
{
}

PhysicsComponent* PhysicsComponent::DoClone() const
{
	return new PhysicsComponent(*this);
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
