#include "stdafx.h"
#include "GravityComponent.h"
#include "../IGameEntity.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../../utils/serializer/XMLDeserializer.h"
#include "../Memory.h"
#include "../../../../NPhysics/source/particle/forceGenerators/ParticleGravity.h"

GravityComponent::GravityComponent(const glm::vec3& gravity) : mGravity(gravity)
{
}

GravityComponent* GravityComponent::DoClone() const
{
	return DBG_NEW GravityComponent(*this);
}

IComponent* GravityComponent::Create(std::shared_ptr<IGameEntity> entity)
{
	GravityComponent* component = DBG_NEW GravityComponent();
	entity->AddComponent(component);

	return component;
}

void GravityComponent::DoReadFrom(core::utils::IDeserializer* source)
{
	mGravity = glm::vec3(0.0f);
	source->BeginAttribute("gravity");
	source->ReadParameter("X", &mGravity.x);
	source->ReadParameter("Y", &mGravity.y);
	source->ReadParameter("Z", &mGravity.z);
	source->EndAttribute();
}

void GravityComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("gravity"), mGravity);
}