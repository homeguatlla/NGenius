#include "stdafx.h"
#include "RotationComponent.h"
#include "../GameEntity.h"
#include "../Transformation.h"
#include "../../utils/serializer/XMLSerializer.h"

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

void RotationComponent::UpdateInternal(float elapsedTime)
{
	mCurrentAngle += mSpeed * elapsedTime;
	glm::vec3 rotation = mRotation * mCurrentAngle;
	Transformation* transformation = mParent->GetTransformation();
	transformation->SetRotation(rotation);
}

void RotationComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void RotationComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("rotation_component"));
	destination->WriteParameter(std::string("rotation"), mRotation);
	destination->WriteParameter(std::string("speed"), mSpeed);
}