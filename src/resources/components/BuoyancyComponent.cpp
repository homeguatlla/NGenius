#include "stdafx.h"
#include "BuoyancyComponent.h"
#include "src/resources/IGameEntity.h"
#include "src/resources/systems/renderSystem/RenderSystem.h"
#include "src/resources/renderers/IRenderer.h"
#include "src/utils/serializer/XMLSerializer.h"
#include "src/utils/serializer/XMLDeserializer.h"
#include "src/AABB.h"
#include "Memory.h"
#include "src/resources/components/RigidbodyPhysicsComponent.h"
#include "source/particle/forceGenerators/ParticleBuoyancy.h"

BuoyancyComponent::BuoyancyComponent(float liquidDensity) : 
	mLiquidDensity(liquidDensity)
{
}

void BuoyancyComponent::Init(GameScene* scene, RenderSystem* renderSystem)
{
	//TODO Water is enabled???
	if (mParent->HasComponent<PhysicsComponent>())
	{
		auto component = mParent->GetComponent<PhysicsComponent>();
		if (component != nullptr && typeid(*component) == typeid(RigidbodyPhysicsComponent))
		{
			auto rigidBody = component->GetPhysicsObject();
			auto boundingVolume = component->GetPhysicsBoundingVolume();

			//this is the center of the object in object coordinates
			mCenter = glm::vec3(0.0f);
			mVolume = boundingVolume->GetVolume();
			mMaxDepth = boundingVolume->GetSize().y * 0.5f;
			mWaterHeight = renderSystem->GetWaterHeight();
		}
	}
}

BuoyancyComponent* BuoyancyComponent::DoClone() const
{
	return DBG_NEW BuoyancyComponent(*this);
}

IComponent* BuoyancyComponent::Create(std::shared_ptr<IGameEntity> entity)
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