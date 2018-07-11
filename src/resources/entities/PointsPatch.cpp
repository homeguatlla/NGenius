#include "stdafx.h"
#include "PointsPatch.h"
#include "../renderers/PointsRenderer.h"
#include "../models/Model.h"
#include "../models/Mesh.h"
#include "../entities/Terrain.h"

PointsPatch::PointsPatch(Transformation* transformation, IMaterial* material, const Terrain* terrain, float waterHeight, float wide, float length, float density) : 
	GameEntity(transformation),
	mTerrain(terrain),
	mWaterHeight(waterHeight),
	mWide(wide),
	mLength(length),
	mDensity(density)
{
	Create();
	SetRenderer(new PointsRenderer(mModel, material));
	GetRenderer()->SetCullingEnabled(false);
	GetRenderer()->SetTransparency(true);
}


PointsPatch::~PointsPatch()
{
}


void PointsPatch::Create()
{
	std::vector<glm::vec3> vertexs;
	std::vector<glm::vec2> uv;
	std::vector<unsigned int> indices;

	int numPoints = static_cast<int>((mWide * mLength) * mDensity);
	for (int point = 0; point < numPoints; ++point)
	{
		float x = -mWide * 0.5f + (rand() % 1000) * (mWide / 1000.0f);
		float z = -mLength * 0.5f + (rand() % 1000) * (mLength / 1000.0f);
		float y = mTerrain->GetHeight(glm::vec2(x, z));
		if (y > mWaterHeight - 0.1f)
		{
			vertexs.push_back(glm::vec3(x, y, z));
		}
	}

	mModel = new Model(new Mesh(vertexs, uv, indices));
}