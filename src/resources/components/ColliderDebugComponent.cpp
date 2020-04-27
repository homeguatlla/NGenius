#include "stdafx.h"
#include "ColliderDebugComponent.h"
#include "src/resources/systems/renderSystem/RenderSystem.h"
#include "src/resources/renderers/IRenderer.h"
#include "src/resources/renderers/WireframeRenderer.h"
#include "src/resources/renderers/WireframeVerticesRenderer.h"
#include "src/resources/models/ModelsLibrary.h"
#include "src/resources/models/Model.h"
#include "src/resources/materials/MaterialsLibrary.h"
#include "src/resources/materials/IMaterial.h"
#include "src/resources/materials/effects/MaterialEffectFloat3.h"
#include "src/resources/IGameEntity.h"
#include "src/resources/components/PhysicsComponent.h"
#include "src/AABB.h"
#include "src/utils/serializer/XMLSerializer.h"
#include "source/bvh/boundingVolumes/SphereBoundingVolume.h"
#include "source/bvh/boundingVolumes/BoxBoundingVolume.h"
#include "Memory.h"

#include <glm/gtx/transform.hpp>

ColliderDebugComponent::ColliderDebugComponent(IRenderer* renderer) :
mBoundingVolumeRenderer(renderer)
{
	assert(mBoundingVolumeRenderer != nullptr);
}


ColliderDebugComponent::~ColliderDebugComponent()
{
	delete mBoundingVolumeRenderer;
}

ColliderDebugComponent* ColliderDebugComponent::DoClone() const
{
	ColliderDebugComponent* clone = DBG_NEW ColliderDebugComponent(*this);
	clone->SetParent(mParent);
	clone->SetEnabled(this->IsEnabled());
	clone->ResetRenderer();

	return clone;
}

std::string ColliderDebugComponent::GetColliderModelNameAndSize(RenderSystem* renderSystem, glm::vec3& position, glm::vec3& size)
{
	auto physicsComponent = mParent->GetComponent<PhysicsComponent>();
	auto boundingVolume = physicsComponent->GetPhysicsBoundingVolume();
	
	position = boundingVolume->GetPosition();
	auto modelName = ModelsLibrary::CUBE_WIREFRAME_NAME;
	if (typeid(*boundingVolume) == typeid(NPhysics::SphereBoundingVolume))
	{
		modelName = ModelsLibrary::SPHERE_NAME;
		auto sphere = std::dynamic_pointer_cast<NPhysics::SphereBoundingVolume>(boundingVolume);
		size = glm::vec3(sphere->GetRadius());
	}
	else
	{
		auto box = std::dynamic_pointer_cast<NPhysics::BoxBoundingVolume>(boundingVolume);
		size = glm::vec3(box->GetSize());
	}

	return std::string(modelName);
}
void ColliderDebugComponent::CreateBoundingVolumeRenderer(const std::string& modelName, RenderSystem* renderSystem)
{
	if (modelName == ModelsLibrary::SPHERE_NAME)
	{
		mBoundingVolumeRenderer = DBG_NEW WireframeVerticesRenderer(
			renderSystem->GetModel(modelName),
			renderSystem->GetMaterial(MaterialsLibrary::COLLIDER_MATERIAL_NAME));
	}
	else
	{
		mBoundingVolumeRenderer = DBG_NEW WireframeRenderer(
			renderSystem->GetModel(modelName),
			renderSystem->GetMaterial(MaterialsLibrary::COLLIDER_MATERIAL_NAME));
	}
}

void ColliderDebugComponent::Init(GameScene* scene, RenderSystem* renderSystem)
{
	glm::vec3 defaultPosition, defaultVolumeSize;

	if (mBoundingVolumeRenderer == nullptr)
	{
		auto modelName = GetColliderModelNameAndSize(renderSystem, defaultPosition, defaultVolumeSize);
		CreateBoundingVolumeRenderer(modelName, renderSystem);
		
		//change volume color
		auto material = mBoundingVolumeRenderer->GetMaterial();
		material->AddEffect(new MaterialEffectFloat3(glm::vec3(0.0f, 1.0f, 0.0f)));
	}

	mBoundingVolumeRenderer->SetParent(mParent);
	auto parentTransMatrix = mParent->GetTransformation()->GetModelMatrix();
	auto parentInverseTransMatrix = glm::inverse(parentTransMatrix);

	auto colliderScale = defaultVolumeSize / mParent->GetTransformation()->GetScale();
	auto colliderOffset = parentInverseTransMatrix * glm::vec4(defaultPosition, 1);

	Transformation transformation(colliderOffset, glm::vec3(0.0f), colliderScale);
	mBoundingVolumeRenderer->SetTransformation(transformation);

	mBoundingVolumeRenderer->SetLayer(IRenderer::LAYER_DEBUG);
	SetBoundingVolumeVisibility(false);
}

IRenderer* ColliderDebugComponent::GetBoundingVolumeRenderer()
{
	return mBoundingVolumeRenderer;
}

void ColliderDebugComponent::ResetRenderer()
{
	mBoundingVolumeRenderer = nullptr;
}

IComponent* ColliderDebugComponent::Create(std::shared_ptr<IGameEntity> entity)
{
	ColliderDebugComponent* component = DBG_NEW ColliderDebugComponent();
	entity->AddComponent(component);

	return component;
}

bool ColliderDebugComponent::IsBoundingVolumeVisible() const
{
	return mBoundingVolumeRenderer->IsVisible();
}

void ColliderDebugComponent::SetBoundingVolumeVisibility(bool visible)
{
	mBoundingVolumeRenderer->SetVisibility(visible);
}

void ColliderDebugComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void ColliderDebugComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("collider_debug_component"));
}