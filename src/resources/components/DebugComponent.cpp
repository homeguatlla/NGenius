#include "stdafx.h"
#include "DebugComponent.h"
#include "../renderers/IRenderer.h"
#include "../../AABB.h"
#include "../GameEntity.h"

DebugComponent::DebugComponent(IRenderer* renderer) :
mBoundingBoxRenderer(renderer)
{
	assert(mBoundingBoxRenderer != nullptr);
}


DebugComponent::~DebugComponent()
{
}

DebugComponent* DebugComponent::DoClone() const
{
	return new DebugComponent(*this);
}

void DebugComponent::Init()
{
	//boundingboxrenderer is of dimensions 1x1x1 and centered in origin in order the 
	//next transformation works
	mBoundingBoxRenderer->SetParent(mParent);
	glm::vec3 min = mParent->GetRenderer()->GetAABB().GetVertexMin();
	glm::vec3 max = mParent->GetRenderer()->GetAABB().GetVertexMax();

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

bool DebugComponent::IsBoundingBoxVisible() const
{
	return mBoundingBoxRenderer->IsVisible();
}

void DebugComponent::SetBoundingBoxVisibility(bool visible)
{
	mBoundingBoxRenderer->SetVisibility(visible);
}