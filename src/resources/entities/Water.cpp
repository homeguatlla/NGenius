#include "stdafx.h"
#include "Water.h"
#include "../systems/renderSystem/RenderSystem.h"
#include "../renderers/IndicesRenderer.h"
#include "../models/Mesh.h"
#include "../models/Model.h"

#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectWater.h"

#include "../../utils/serializer/IDeserializer.h"
#include "../../utils/serializer/XMLDeserializer.h"
#include "../../utils/Log.h"

#include <glm/gtc/matrix_transform.hpp>

Water::Water(Transformation* transformation, IMaterial* material, float wide, float length, float speed) :
GameEntity(transformation),
mWide(wide),
mLength(length),
mWaterSpeed(speed),
mCurrentWaterSpeed(speed)
{
	Create(material);
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

GameEntity* Water::CreateGameEntity()
{
	return new Water();
}

void Water::Build(RenderSystem* renderSystem)
{
	IMaterial* material = renderSystem->GetMaterial(mMaterialName);
	Create(material);
}

void Water::ReadFrom(core::utils::IDeserializer* source)
{
	GameEntity::ReadFrom(source);

	source->ReadParameter("material", mMaterialName);
	source->ReadParameter("wide", &mWide);
	source->ReadParameter("length", &mLength);
}

void Water::CreateModel()
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

void Water::Create(IMaterial* material)
{
	CreateModel();
	
	SetRenderer(new IndicesRenderer(mModel, material));

	GetRenderer()->SetLayer(IRenderer::LAYER_WATER);
	if (material->HasEffect<MaterialEffectWater>())
	{
		mWaterEffect = material->GetEffect<MaterialEffectWater>();
		mWaterSpeed = mWaterEffect->GetSpeed();
	}
}

