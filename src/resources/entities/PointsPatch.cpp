#include "stdafx.h"
#include "PointsPatch.h"
#include "../renderers/PointsRenderer.h"
#include "../models/Model.h"
#include "../models/Mesh.h"
#include "../entities/Terrain.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectFloat.h"

#include <iostream>

PointsPatch::PointsPatch(Transformation* transformation, IMaterial* material, const Terrain* terrain, float heightMin, float heightMax, float wide, float length, float density) : 
	GameEntity(transformation),
	mTerrain(terrain),
	mHeightMin(heightMin),
	mHeightMax(heightMax),
	mWide(wide),
	mLength(length),
	mDensity(density)
{
	assert(material != nullptr);

	Create();

	if (mModel != nullptr)
	{
		SetRenderer(new PointsRenderer(mModel, material));
		GetRenderer()->SetCullingEnabled(false);
		GetRenderer()->SetTransparency(true);
	}
}


PointsPatch::~PointsPatch()
{
}

void PointsPatch::Create()
{
	std::vector<glm::vec3> vertexs;
	std::vector<glm::vec2> uv;
	std::vector<unsigned int> indices;

	unsigned int numPoints = static_cast<int>((mWide * mLength) * mDensity);
	for (unsigned int point = 0; point < numPoints; ++point)
	{
		float x = -mWide * 0.5f + (rand() % 1000) * (mWide / 1000.0f);
		float z = -mLength * 0.5f +(rand() % 1000) * (mLength / 1000.0f);
		float y = mTerrain->GetHeight(glm::vec2(x, z));
		if (y >= mHeightMin && y <= mHeightMax)
		{
			vertexs.push_back(glm::vec3(x, y, z));
			float rotationY = rand() % 360;
			float scale = 0.4f + (rand() % 10) * (0.4f / 10.0f);
			uv.push_back(glm::vec2(glm::radians(rotationY), scale * 0.1));
		}
	}
	if (vertexs.size() > 0)
	{
		mModel = new Model(new Mesh(vertexs, uv, indices));
	}
}

