#include "stdafx.h"
#include "EnergyWall.h"

#include "../renderers/EnergyWallRenderer.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectFloat3.h"
#include "../materials/effects/MaterialEffectFloat.h"
#include "../Memory.h"

EnergyWall::EnergyWall(Transformation* transformation, IMaterial* material, Model* model, float maxLiveTime) : 
GameEntity(transformation),
mMaterial(material),
mModel(model),
mMaxLiveTime(maxLiveTime),
mLiveTime(0.0f),
mContactPoint(0.0f),
mMaterialEffectFloat(nullptr)
{
	assert(model != nullptr);
	assert(material != nullptr);
	material->AddEffect(new MaterialEffectFloat3(glm::vec3(0.0f)));
	mMaterialEffectFloat = DBG_NEW MaterialEffectFloat(mLiveTime);
	material->AddEffect(mMaterialEffectFloat);
	SetRenderer(new EnergyWallRenderer(model, material));
	GetRenderer()->SetLayer(IRenderer::LAYER_PARTICLES);
	GetRenderer()->SetTransparency(true);
}

EnergyWall::~EnergyWall()
{
}

EnergyWall* EnergyWall::DoClone() const
{
	EnergyWall* clone = DBG_NEW EnergyWall(new Transformation(*GetTransformation()), mMaterial, mModel, mLiveTime);

	return clone;

}
void EnergyWall::Update(float elapsedTime)
{
	GameEntity::Update(elapsedTime);

	assert(mLiveTime <= mMaxLiveTime);

	mLiveTime = glm::max(0.0f, mLiveTime - elapsedTime);
	mMaterialEffectFloat->SetValue(mLiveTime);	
}

void EnergyWall::SetLiveTime(float liveTime)
{
	mLiveTime = liveTime;
}

void EnergyWall::SetMaxLiveTime(float maxLiveTime)
{
	mMaxLiveTime = maxLiveTime;
}

void EnergyWall::SetContactPoint(const glm::vec3& contact)
{
	mContactPoint = contact;
	MaterialEffectFloat3* effect = mMaterial->GetEffect<MaterialEffectFloat3>();
	effect->SetValue(mContactPoint);
}

float EnergyWall::GetLiveTime() const
{
	return mLiveTime;
}

bool EnergyWall::IsAlive() const
{
	return mLiveTime > 0.0f;
}

glm::vec3 EnergyWall::GetContactPoint() const
{
	return mContactPoint;
}

float EnergyWall::GetMaxLiveTime() const
{
	return mMaxLiveTime;
}