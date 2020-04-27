#include "stdafx.h"
#include "LODComponent.h"
#include "../IGameEntity.h"
#include "../camera/ICamera.h"
#include "../systems/renderSystem/RenderSystem.h"
#include "../renderers/IRenderer.h"
#include "../renderers/VerticesRenderer.h"
#include "../models/Model.h"
#include "../materials/IMaterial.h"
#include "../../utils/serializer/XMLSerializer.h"
#include "../../utils/serializer/XMLDeserializer.h"
#include "../Memory.h"

#include <glm/gtx/norm.hpp>
#include <algorithm>

#include <iostream>

const float HYSTERESIS = 5.0f;

LODComponent::LODComponent(const ICamera* camera) :
mCamera(camera),
mLastLODIndex(0)
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
	LODComponent* clone = DBG_NEW LODComponent(mCamera);
	clone->SetParent(mParent);
	clone->SetEnabled(this->IsEnabled());

	for (auto lod : mLODS)
	{
		clone->AddLevelOfDetail(
			DBG_NEW VerticesRenderer(lod->renderer->GetModel(), lod->renderer->GetMaterial()), 
			lod->distance);
	}

	return clone;
}

void LODComponent::Init(GameScene* scene, RenderSystem* renderSystem)
{
	mCamera = renderSystem->GetGameplayCamera();
	for (auto lodData : mLodsToAdd)
	{
		Model* model = renderSystem->GetModel(std::get<0>(lodData));
		IMaterial* material = renderSystem->GetMaterial(std::get<1>(lodData));
		if (model != nullptr && material != nullptr)
		{
			IRenderer* renderer = DBG_NEW VerticesRenderer(model, material);
			AddLevelOfDetail(renderer, std::get<2>(lodData));
		}
	}
	mLodsToAdd.clear();
}

void LODComponent::UpdateInternal(float elapsedTime)
{
	int lod = -1;
	float distance = glm::length2(mParent->GetTransformation()->GetPosition() - mCamera->GetPosition());
	for (unsigned int i = 0; i < mLODS.size(); ++i)
	{
		float offset = mLastLODIndex <= i ? HYSTERESIS : -HYSTERESIS;
		if (distance < mLODS[i]->distance + offset)
		{
			lod = i;
			break;
		}
	}

	if (lod == -1 && mLODS.size() > 0)
	{
		lod = mLODS.size() - 1;
	}
	mLastLODIndex = lod;

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
		LOD* lod = DBG_NEW LOD(renderer, distance);
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

IComponent* LODComponent::Create(std::shared_ptr<IGameEntity> entity)
{
	LODComponent* component = DBG_NEW LODComponent();
	entity->AddComponent(component);

	return component;
}


void LODComponent::DoReadFrom(core::utils::IDeserializer* source)
{
	if (source->HasAttribute("lods"))
	{
		source->BeginAttribute(std::string("lods"));
		unsigned int numConverters = source->ReadNumberOfElements();
		source->BeginAttribute(std::string("lod"));
		do
		{
			auto lodData = ReadLODFrom(source);
			mLodsToAdd.push_back(lodData);
			source->NextAttribute();
			numConverters--;

		} while (numConverters > 0);
		source->EndAttribute();
		source->EndAttribute();
	}
}

LODComponent::LODData LODComponent::ReadLODFrom(core::utils::IDeserializer* source)
{
	std::string modelName;
	std::string material;
	float distance;

	source->ReadParameter("model_name", modelName);
	source->ReadParameter("material", material);
	source->ReadParameter("distance", &distance);

	return std::make_tuple(modelName, material, distance);
}

void LODComponent::DoWriteTo(core::utils::ISerializer* destination)
{
	destination->WriteParameter(std::string("type"), std::string("lod_component"));
	destination->WriteParameter(std::string("camera_name"), mCamera->GetName());
	destination->BeginAttribute(std::string("lods"));
	for (unsigned int i = 0; i < mLODS.size(); ++i)
	{
		destination->BeginAttribute(std::string("lod"));
		destination->WriteParameter(std::string("distance"), mLODS[i]->distance);
		unsigned int modelID = mLODS[i]->renderer->GetModel()->GetID();
		unsigned int materialID = mLODS[i]->renderer->GetMaterial()->GetMaterialID();
		destination->WriteParameter(std::string("modelID"), modelID);
		destination->WriteParameter(std::string("materialID"), materialID);
		destination->EndAttribute();
	}
	destination->EndAttribute();
}