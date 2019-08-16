#include "stdafx.h"
#include "DebugComponent.h"
#include "../systems/renderSystem/RenderSystem.h"
#include "../renderers/IRenderer.h"
#include "../renderers/WireframeRenderer.h"
#include "../models/ModelsLibrary.h"
#include "../materials/MaterialsLibrary.h"
#include "../../AABB.h"
#include "../IGameEntity.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../../Memory.h"

DebugComponent::DebugComponent(IRenderer* renderer) :
mBoundingBoxRenderer(renderer)
{
	assert(mBoundingBoxRenderer != nullptr);
}


DebugComponent::~DebugComponent()
{
	delete mBoundingBoxRenderer;
}

DebugComponent* DebugComponent::DoClone() const
{
	return DBG_NEW DebugComponent(*this);
}

void DebugComponent::Init(GameScene* scene, RenderSystem* renderSystem)
{
	if (mBoundingBoxRenderer == nullptr)
	{
		mBoundingBoxRenderer = DBG_NEW WireframeRenderer(
			renderSystem->GetModel(ModelsLibrary::CUBE_NAME), 
			renderSystem->GetMaterial(MaterialsLibrary::WIREFRAME_MATERIAL_NAME));
	}

	//boundingboxrenderer is of dimensions 1x1x1 and centered in origin in order the 
	//next transformation works
	mBoundingBoxRenderer->SetParent(mParent);
	glm::vec3 min = mParent->GetRenderer()->GetModelAABB().GetVertexMin();
	glm::vec3 max = mParent->GetRenderer()->GetModelAABB().GetVertexMax();

	glm::vec3 scale = max - min;

	Transformation transformation(min + scale * 0.5f, glm::vec3(0.0f), scale);
	mBoundingBoxRenderer->SetTransformation(transformation);
	mBoundingBoxRenderer->SetLayer(IRenderer::LAYER_DEBUG);
	SetBoundingBoxVisibility(false);
}

IRenderer* DebugComponent::GetBoundingBoxRenderer()
{
	return mBoundingBoxRenderer;
}

IComponent* DebugComponent::Create(IGameEntity* entity)
{
	DebugComponent* component = DBG_NEW DebugComponent();
	entity->AddComponent(component);

	return component;
}

bool DebugComponent::IsBoundingBoxVisible() const
{
	return mBoundingBoxRenderer->IsVisible();
}

void DebugComponent::SetBoundingBoxVisibility(bool visible)
{
	mBoundingBoxRenderer->SetVisibility(visible);
}

void DebugComponent::DoReadFrom(core::utils::IDeserializer* source)
{

}

void DebugComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("debug_component"));
}