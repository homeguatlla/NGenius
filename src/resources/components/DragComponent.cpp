#include "stdafx.h"
#include "DragComponent.h"
#include "../IGameEntity.h"
#include "../systems/renderSystem/RenderSystem.h"
#include "../renderers/IRenderer.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../../utils/serializer/XMLDeserializer.h"
#include "../../AABB.h"
#include "../Memory.h"
#include "../../../../NPhysics/source/particle/forceGenerators/ParticleDrag.h"

DragComponent::DragComponent(float k1, float k2) : mK1(k1), mK2(k2)
{
}

DragComponent* DragComponent::DoClone() const
{
	return DBG_NEW DragComponent(*this);
}

IComponent* DragComponent::Create(std::shared_ptr<IGameEntity> entity)
{
	DragComponent* component = DBG_NEW DragComponent();
	entity->AddComponent(component);

	return component;
}

void DragComponent::DoReadFrom(core::utils::IDeserializer* source)
{
	source->ReadParameter(std::string("k1"), &mK1);
	source->ReadParameter(std::string("k2"), &mK2);
}

void DragComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("k1"), mK1);
	destination->WriteParameter(std::string("k2"), mK2);
}