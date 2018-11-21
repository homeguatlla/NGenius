#include "stdafx.h"
#include "Particle.h"
#include "../renderers/ParticleRenderer.h"

#include <iostream>

Particle::Particle(Transformation* transformation, Model* model, IMaterial* material, float liveTime) :
GameEntity(transformation),
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
mColorDestination(1.0f),
mCanDeleteWhenRemovingFromScene(false)
{
	ParticleRenderer* renderer = new ParticleRenderer(model, material);
	renderer->SetTransparency(true);
	renderer->SetLayer(IRenderer::LAYER_PARTICLES);
	SetRenderer(renderer);
}


Particle::~Particle()
{
}

Particle* Particle::DoClone() const
{
	Particle* clone = new Particle(new Transformation(*GetTransformation()), mModel, mMaterial, mLiveTime);

	return clone;
}

bool Particle::CanDeleteWhenRemovingFromScene() const
{
	return mCanDeleteWhenRemovingFromScene;
}

void Particle::SetDeleteWhenRemovingFromScene()
{
	mCanDeleteWhenRemovingFromScene = true;
}

bool Particle::IsAlive() const
{
	return mLiveTime > 0.0f;
}

void Particle::Update(float elapsedTime)
{
	GameEntity::Update(elapsedTime);
	
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