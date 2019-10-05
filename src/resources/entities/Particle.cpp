#include "stdafx.h"
#include "Particle.h"
#include "../renderers/ParticleRenderer.h"
#include "../Memory.h"

#include <iostream>

Particle::Particle(Transformation* transformation, Model* model, IMaterial* material, float liveTime) :
	BaseGameEntity(transformation),
mModel(model),
mMaterial(material),
mMaxLiveTime(liveTime),
mLiveTime(liveTime),
mScale(1.0f),
mRotationSpeed(0.0f),
mRotation(0.0f),
mDistanceToTheCamera(0.0f),
mColor(1.0f),
mColorOrigin(1.0f),
mColorDestination(1.0f)
{
	CreateRenderer(model, material);
}

void Particle::CreateRenderer(Model* model, IMaterial* material)
{
	ParticleRenderer* renderer = DBG_NEW ParticleRenderer(model, material);
	renderer->SetTransparency(true);
	renderer->SetLayer(IRenderer::LAYER_PARTICLES);
	SetRenderer(renderer);
}

void Particle::ReadFrom(core::utils::IDeserializer* source)
{
	BaseGameEntity::ReadFrom(source);
	source->ReadParameter("max_live_time", &mMaxLiveTime);
}

IGameEntity* Particle::DoCreate()
{
	return DBG_NEW Particle();
}

Particle* Particle::DoClone() const
{
	Particle* clone = DBG_NEW Particle(new Transformation(*GetTransformation()), mModel, mMaterial, mMaxLiveTime);
	
	return clone;
}

void Particle::Build(NGenius* engine)
{
	if (!mMaterialName.empty())
	{
		mMaterial = engine->GetMaterial(mMaterialName);
		mModel = engine->GetModel(mModelName);
		CreateRenderer(mModel, mMaterial);
	}
}

bool Particle::IsAlive() const
{
	return mLiveTime > 0.0f;
}

void Particle::Update(float elapsedTime)
{
	BaseGameEntity::Update(elapsedTime);
	
	assert(mLiveTime <= mMaxLiveTime);

	mLiveTime = glm::max(0.0f, mLiveTime - elapsedTime);

	float t = ((mMaxLiveTime - mLiveTime) / mMaxLiveTime);
	t = glm::max(0.0f, glm::min(1.0f, t));

	//TRANSFORMATIONS
	Transformation* transformation = GetTransformation();
	mRotation += mRotationSpeed * elapsedTime;
	transformation->SetRotation(glm::vec3(mRotation));
	float scale = glm::mix(mScale.x, mScale.y, t);
	transformation->SetScale(glm::vec3(scale));

	//COLOR
	mColor = glm::mix(mColorOrigin, mColorDestination, t);
}

void Particle::SetScaleFactor(glm::vec2& scale)
{
	mScale = scale;
}

void Particle::SetRotationSpeed(float rotationSpeed)
{
	mRotation = static_cast<float>(rand() % 360);
	mRotationSpeed = rotationSpeed;
}

void Particle::SetLiveTime(float liveTime)
{
	mLiveTime = liveTime;
}

float Particle::GetLiveTime() const
{
	return mLiveTime;
}

void Particle::DoInit(GameScene* scene, RenderSystem* renderSystem)
{
}

void Particle::SetColorGradientValues(const glm::vec4& origin, const glm::vec4& destination)
{
	mColorOrigin = origin;
	mColorDestination = destination;
}

const glm::vec4& Particle::GetColor() const
{
	return mColor;
}

float Particle::GetMaxLiveTime() const
{
	return mMaxLiveTime;
}