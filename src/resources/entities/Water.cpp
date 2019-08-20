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
#include "../Memory.h"

#include <glm/gtc/matrix_transform.hpp>

Water::Water(Transformation* transformation, IMaterial* material, float wide, float length, float speed) :
	BaseGameEntity(transformation),
mWide(wide),
mLength(length),
mWaterSpeed(speed),
mCurrentWaterSpeed(speed)
{
	CreateWater(material);
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

void Water::Build(NGenius* engine)
{
	IMaterial* material = engine->GetMaterial(mMaterialName);
	CreateWater(material);

	//Setting the y of the water entity into the water position the renderer says.
	//Remember that can only exist ONE water entity at moment
	glm::vec3 position = GetTransformation()->GetPosition();
	position.y = engine->GetWaterHeight();
	GetTransformation()->SetPosition(position);
}

void Water::ReadFrom(core::utils::IDeserializer* source)
{
	BaseGameEntity::ReadFrom(source);

	source->ReadParameter("material", mMaterialName);
	source->ReadParameter("wide", &mWide);
	source->ReadParameter("length", &mLength);
}

IGameEntity* Water::DoCreate()
{
	return DBG_NEW Water();
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

	mModel = DBG_NEW Model(new Mesh(vertexs, uv, indices));
}

void Water::DoInit(GameScene* scene, RenderSystem* renderSystem)
{
}

void Water::CreateWater(IMaterial* material)
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

