#include "stdafx.h"
#include "EnergyWallCollisionComponent.h"
#include "../../utils/serializer/XMLSerializer.h"

#include "../GameEntity.h"
#include "../Transformation.h"
#include "../Memory.h"

EnergyWallCollisionComponent::EnergyWallCollisionComponent() :
mIsColliding(false),
mCollisionPoint(glm::vec3(0.0f))
{
}


EnergyWallCollisionComponent::~EnergyWallCollisionComponent()
{
}

EnergyWallCollisionComponent* EnergyWallCollisionComponent::DoClone() const
{
	return DBG_NEW EnergyWallCollisionComponent(*this);
}

bool EnergyWallCollisionComponent::IsColliding() const
{
	return mIsColliding;
}

void EnergyWallCollisionComponent::SetCollision(bool isColliding)
{
	mIsColliding = isColliding;
}

void EnergyWallCollisionComponent::SetCollisionPoint(glm::vec3& point)
{
	mCollisionPoint = point;
}

glm::vec3 EnergyWallCollisionComponent::GetCollisionPoint() const
{
	return mCollisionPoint;
}

IComponent* EnergyWallCollisionComponent::Create()
{
	return DBG_NEW EnergyWallCollisionComponent();
}

void EnergyWallCollisionComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void EnergyWallCollisionComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("energy_wall_collision_component"));
}