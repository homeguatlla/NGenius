#include "stdafx.h"
#include "Water.h"
#include "../renderers/IndicesRenderer.h"
#include "../models/Mesh.h"
#include "../models/Model.h"

#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectWater.h"

#include <glm/gtc/matrix_transform.hpp>

Water::Water(Transformation* transformation, IMaterial* material, float wide, float length, float speed) :
GameEntity(transformation),
mWide(wide),
mLength(length),
mWaterSpeed(speed),
mCurrentWaterSpeed(speed)
{
	Create();
	SetRenderer(new IndicesRenderer(mModel, material));

	GetRenderer()->SetLayer(IRenderer::LAYER_WATER);
	if (material->HasEffect<MaterialEffectWater>())
	{
		mWaterEffect = material->GetEffect<MaterialEffectWater>();
	}
}

Water::~Water()
{
	delete mModel;
}

void Water::Update(float elapsedTime)
{
	mCurrentWaterSpeed += mWaterSpeed * elapsedTime;
	mCurrentWaterSpeed = glm::mod(mCurrentWaterSpeed, 1.0f);
	mWaterEffect->SetSpeed(mCurrentWaterSpeed);
}

void Water::Create()
{
	std::vector<glm::vec3> vertexs;
	std::vector<glm::vec2> uv;
	std::vector<unsigned int> indices;

	vertexs.push_back(glm::vec3(-mWide, mLength, 0.0f));
	vertexs.push_back(glm::vec3(mWide, mLength, 0.0f));
	vertexs.push_back(glm::vec3(mWide, -mLength, 0.0f));
	vertexs.push_back(glm::vec3(-mWide, -mLength, 0.0f));

	uv.push_back(glm::vec2(0.0f, 1.0f));
	uv.push_back(glm::vec2(1.0f, 1.0f));
	uv.push_back(glm::vec2(1.0f, 0.0f));
	uv.push_back(glm::vec2(0.0f, 0.0f));

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);

	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(1);

	mModel = new Model(new Mesh(vertexs, uv, indices));
}
