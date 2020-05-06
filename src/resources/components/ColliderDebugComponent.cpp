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
#include "source/utils/Math.h"
#include "Memory.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>

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

std::string ColliderDebugComponent::GetColliderModelNameAndSize(RenderSystem* renderSystem, glm::vec3& localTranslation, glm::vec3& size, glm::vec3& localRotation)
{
	auto physicsComponent = mParent->GetComponent<PhysicsComponent>();
	auto boundingVolume = physicsComponent->GetPhysicsBoundingVolume();
	
	localTranslation = boundingVolume->GetLocalTranslation();
	localRotation = boundingVolume->GetLocalRotation();

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
	glm::vec3 localTranslation, localRotation, defaultVolumeSize;

	if (mBoundingVolumeRenderer == nullptr)
	{
		auto modelName = GetColliderModelNameAndSize(renderSystem, localTranslation, defaultVolumeSize, localRotation);
		CreateBoundingVolumeRenderer(modelName, renderSystem);
		
		//change volume color
		auto material = mBoundingVolumeRenderer->GetMaterial();
		material->AddEffect(new MaterialEffectFloat3(glm::vec3(0.0f, 1.0f, 0.0f)));
	}

	mBoundingVolumeRenderer->SetParent(mParent);
	Transformation transformation(localTranslation, localRotation, defaultVolumeSize);
	auto parentRotation = glm::rotate(glm::mat4(1.0f), mParent->GetTransformation()->GetRotation().x, glm::vec3(1.0f, 0.0f, 0.0f));
	parentRotation = glm::rotate(parentRotation, mParent->GetTransformation()->GetRotation().y, glm::vec3(0.0f, 1.0f, 0.0f));
	parentRotation = glm::rotate(parentRotation, mParent->GetTransformation()->GetRotation().z, glm::vec3(0.0f, 0.0f, 1.0f));

	auto parentTranslation = glm::translate(glm::mat4(1.0f), mParent->GetTransformation()->GetPosition());

	auto mat = glm::inverse(mParent->GetTransformation()->GetModelMatrix()) * parentTranslation * parentRotation * transformation.GetModelMatrix();
	glm::vec3 finalScale;
	glm::quat finalRotation;
	glm::vec3 finalTranslation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(mat, finalScale, finalRotation, finalTranslation, skew, perspective);

	mBoundingVolumeRenderer->SetTransformation(Transformation(finalTranslation, glm::eulerAngles(finalRotation), finalScale));

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