#include "stdafx.h"
#include "RotationComponent.h"
#include "../GameEntity.h"
#include "../Transformation.h"

RotationComponent::RotationComponent(const glm::vec3& rotation, float speed) :
mRotation(rotation),
mSpeed(speed)
{
}


RotationComponent::~RotationComponent()
{
}
RotationComponent* RotationComponent::DoClone() const
{
	return new RotationComponent(*this);
}

void RotationComponent::Update(float elapsedTime)
{
	mCurrentAngle += mSpeed * elapsedTime;
	glm::vec3 rotation = mRotation * mCurrentAngle;
	Transformation* transformation = mParent->GetTransformation();
	transformation->SetRotation(rotation);
}
