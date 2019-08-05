#include "stdafx.h"
#include "PointsPatch.h"
#include "../renderers/PointsRenderer.h"
#include "../models/Model.h"
#include "../models/Mesh.h"
#include "../entities/Terrain.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectFloat.h"
#include "../Memory.h"

#include <iostream>

PointsPatch::PointsPatch(Transformation* transformation, IMaterial* material, const Terrain* terrain, float heightMin, float heightMax, float wide, float length, float density) : 
	BaseGameEntity(transformation),
	mTerrain(terrain),
	mHeightMin(heightMin),
	mHeightMax(heightMax),
	mWide(wide),
	mLength(length),
	mDensity(density)
{
	assert(material != nullptr);

	CreatePointsPatch(material);
}


PointsPatch::~PointsPatch()
{
}

IGameEntity* PointsPatch::DoCreate()
{
	return DBG_NEW PointsPatch();
}

void PointsPatch::Build(NGenius* engine)
{
	if (!mMaterialName.empty())
	{
		IMaterial* material = engine->GetMaterial(mMaterialName);

		mTerrain = static_cast<Terrain*>(engine->GetGameEntity("terrain"));
		assert(mTerrain != nullptr);

		CreatePointsPatch(material);
	}
}

void PointsPatch::CreatePointsPatch(IMaterial* material)
{
	CreateModel();
	if (mModel != nullptr)
	{
		SetRenderer(new PointsRenderer(mModel, material));
		GetRenderer()->SetCullingEnabled(false);
		GetRenderer()->SetTransparency(true);
	}
}

void PointsPatch::CreateModel()
{
	std::vector<glm::vec3> vertexs;
	std::vector<glm::vec2> uv;
	std::vector<unsigned int> indices;

	unsigned int numPoints = static_cast<int>((mWide * mLength) * mDensity);
	for (unsigned int i = 0; i < numPoints; ++i)
	{
		glm::vec3 point(0.0f);

		point.x = -mWide * 0.5f + (rand() % 1000) * (mWide / 1000.0f);
		point.z = -mLength * 0.5f + (rand() % 1000) * (mLength / 1000.0f);
		point = GetTransformation()->GetModelMatrix() * glm::vec4(point, 1.0f);

		point.y = mTerrain->GetHeight(glm::vec2(point.x, point.z));
		if (point.y >= mHeightMin && point.y <= mHeightMax)
		{
			vertexs.push_back(point);
			float rotationY = static_cast<float>(rand() % 360);
			float scale = 1.2f + (rand() % 10) * (0.4f / 10.0f);
			uv.push_back(glm::vec2(glm::radians(rotationY), scale * 0.2));
		}
	}

	if (vertexs.size() > 0)
	{
		mModel = DBG_NEW Model(new Mesh(vertexs, uv, indices));
	}
}

void PointsPatch::ReadFrom(core::utils::IDeserializer* source)
{
	BaseGameEntity::ReadFrom(source);
	source->ReadParameter("min_height", &mHeightMin);
	source->ReadParameter("max_height", &mHeightMax);
	source->ReadParameter("wide", &mWide);
	source->ReadParameter("lenght", &mLength);
	source->ReadParameter("density", &mDensity);
}

