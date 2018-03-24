#include "stdafx.h"
#include "DebugComponent.h"
#include "../renderers/IRenderer.h"

DebugComponent::DebugComponent(IRenderer* renderer) :
mBoundingBoxRenderer(renderer),
mIsBoundingBoxVisible(false)
{
	assert(mBoundingBoxRenderer != nullptr);
	UpdateBoundingBox();
}


DebugComponent::~DebugComponent()
{
}

DebugComponent* DebugComponent::DoClone() const
{
	return new DebugComponent(*this);
}

IRenderer* DebugComponent::GetBoundingBoxRenderer()
{
	return mBoundingBoxRenderer;
}

bool DebugComponent::IsBoundingBoxVisible() const
{
	return mIsBoundingBoxVisible;
}

void DebugComponent::SetBoundingBoxVisibility(bool visible)
{
	mIsBoundingBoxVisible = visible;
	UpdateBoundingBox();
}

void DebugComponent::UpdateBoundingBox()
{
	//TODO esto es un poco feo, cuando se crea el componente no tiene el padre asignado todavía
	if (mBoundingBoxRenderer->GetParent() == nullptr)
	{
		mBoundingBoxRenderer->SetParent(mParent);
	}
	mBoundingBoxRenderer->SetVisibility(mIsBoundingBoxVisible);
}