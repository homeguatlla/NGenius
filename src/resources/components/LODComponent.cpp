#include "stdafx.h"
#include "LODComponent.h"
#include "../GameEntity.h"
#include "../camera/ICamera.h"
#include "../renderers/IRenderer.h"
#include <glm/gtx/norm.hpp>
#include <algorithm>

LODComponent::LODComponent(const ICamera* camera) :
mCamera(camera)
{
	mLODS.reserve(MAX_LODS);
}


LODComponent::~LODComponent()
{
	for (unsigned int i = 0; i < mLODS.size(); ++i)
	{
		delete mLODS[i];
	}
	mLODS.clear();
}

IComponent* LODComponent::DoClone() const
{
	return new LODComponent(*this);
}

void LODComponent::UpdateInternal(float elapsedTime)
{
	int lod = -1;
	float distance = glm::length2(mParent->GetTransformation()->GetPosition() - mCamera->GetPosition());
	for (unsigned int i = 0; i < mLODS.size(); ++i)
	{
		if (distance < mLODS[i]->distance)
		{
			lod = i;
			break;
		}
	}

	if (lod == -1 && mLODS.size() > 0)
	{
		lod = mLODS.size() - 1;
	}

	mParent->SetRenderer(mLODS[lod]->renderer);
}



void LODComponent::AddLevelOfDetail(IRenderer* renderer, float distance)
{
	//before start adding levels must add the component to the GameEntity
	assert(mParent != nullptr);

	unsigned int totalLODS = mLODS.size();
	
	assert(totalLODS < MAX_LODS);

	if (totalLODS < MAX_LODS)
	{
		LOD* lod = new LOD(renderer, distance);
		renderer->SetParent(mParent);
		mLODS.push_back(lod);
	}
	std::sort(mLODS.begin(), mLODS.end(), [](const LOD* a, const LOD* b) -> bool
	{
		return a->distance < b->distance;
	});
	if (mParent->GetRenderer() == nullptr && mLODS.size() > 0)
	{
		//assign a renderer by default the closer. It will be needed for the CollisionComponent if is the case. Should always be one assigned.
		mParent->SetRenderer(mLODS[0]->renderer);
	}
}
