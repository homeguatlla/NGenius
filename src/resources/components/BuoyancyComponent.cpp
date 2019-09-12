#include "stdafx.h"
#include "BuoyancyComponent.h"
#include "../IGameEntity.h"
#include "../systems/renderSystem/RenderSystem.h"
#include "../renderers/IRenderer.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../../utils/serializer/XMLDeserializer.h"
#include "../../AABB.h"
#include "../Memory.h"
#include "../../../../NPhysics/source/particle/forceGenerators/ParticleBuoyancy.h"

BuoyancyComponent::BuoyancyComponent(float liquidDensity) : 
	mLiquidDensity(liquidDensity)
{
}

void BuoyancyComponent::Init(GameScene* scene, RenderSystem* renderSystem)
{
	const AABB aabb = mParent->GetRenderer()->GetAABB();
	float volume = aabb.GetVolume();
	//glm::vec3 size = aabb.GetSize();

	//because the object could be rotated and scaled
	glm::vec3 max = aabb.GetVertexMax();
	glm::vec3 min = aabb.GetVertexMin();
	glm::mat3 matrix = glm::mat3(mParent->GetTransformation()->GetModelMatrix());
	max = matrix * max;
	min = matrix * min;

	float sizeY = max.y > min.y ? max.y - min.y : min.y - max.y;
	float maxDepth = sizeY * 0.5f;
	float waterHeight = renderSystem->GetWaterHeight();
	

	//TODO is water enabled??
	mGenerator = std::make_shared<NPhysics::ParticleBuoyancy>(maxDepth, volume, waterHeight, mLiquidDensity);
}

BuoyancyComponent* BuoyancyComponent::DoClone() const
{
	return DBG_NEW BuoyancyComponent(*this);
}

IComponent* BuoyancyComponent::Create(IGameEntity* entity)
{
	BuoyancyComponent* component = DBG_NEW BuoyancyComponent();
	entity->AddComponent(component);

	return component;
}

void BuoyancyComponent::DoReadFrom(core::utils::IDeserializer* source)
{
	source->ReadParameter(std::string("liquid_density"), &mLiquidDensity);
}

void BuoyancyComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("liquid_density"), mLiquidDensity);
}