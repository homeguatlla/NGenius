#include "stdafx.h"
#include "RotationComponent.h"
#include "../IGameEntity.h"
#include "../Transformation.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../../utils/serializer/XMLDeserializer.h"
#include "../Memory.h"

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
	return DBG_NEW RotationComponent(*this);
}

void RotationComponent::UpdateInternal(float elapsedTime)
{
	mCurrentAngle += mSpeed * elapsedTime;
	glm::vec3 rotation = mRotation * mCurrentAngle;
	Transformation* transformation = mParent->GetTransformation();
	transformation->SetRotation(rotation);
}

IComponent* RotationComponent::Create(IGameEntity* entity)
{
	RotationComponent* component = DBG_NEW RotationComponent();
	entity->AddComponent(component);

	return component;
}

void RotationComponent::DoReadFrom(core::utils::IDeserializer* source)
{
	source->ReadParameter(std::string("rotation_speed"), &mSpeed);
	source->BeginAttribute("axis");
	source->ReadParameter("X", &mRotation.x);
	source->ReadParameter("Y", &mRotation.y);
	source->ReadParameter("Z", &mRotation.z);
	source->EndAttribute();
}

void RotationComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("rotation_component"));
	destination->WriteParameter(std::string("rotation"), mRotation);
	destination->WriteParameter(std::string("speed"), mSpeed);
}