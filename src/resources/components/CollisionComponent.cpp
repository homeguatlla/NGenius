#include "stdafx.h"
#include "CollisionComponent.h"

#include "../GameEntity.h"
#include "../Transformation.h"
#include "../../utils/serializer/XMLSerializer.h"

CollisionComponent::CollisionComponent() :
mIsOnGround(false),
mGroundHeight(0.0f)
{
}


CollisionComponent::~CollisionComponent()
{
}

CollisionComponent* CollisionComponent::DoClone() const
{
	return new CollisionComponent(*this);
}

bool CollisionComponent::IsOnGround() const
{
	return mIsOnGround;
}

void CollisionComponent::SetOnGround(bool isOnGround)
{
	mIsOnGround = isOnGround;
}

void CollisionComponent::SetGroundHeight(float groundHeight)
{
	mGroundHeight = groundHeight;
}

float CollisionComponent::GetGroundHeight() const
{
	return mGroundHeight;
}

void CollisionComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void CollisionComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("collision_component"));
}