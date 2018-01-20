#include "stdafx.h"
#include "Particle.h"
#include "../../renderer/IRenderer.h"

#include <iostream>

Particle::Particle(Transformation* transformation, IRenderer* renderer, float liveTime) :
GameEntity(transformation, renderer),
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
}


Particle::~Particle()
{
}

Particle* Particle::DoClone() const
{
	IRenderer* cloneRenderer = GetRenderer()->Clone();
	Particle* clone = new Particle(new Transformation(*GetTransformation()), cloneRenderer, mLiveTime);

	return clone;
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
	float scale = Lerp(mScale.x, mScale.y, t);
	transformation->SetScale(glm::vec3(scale));

	//COLOR
	mColor = Lerp(mColorOrigin, mColorDestination, t);
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

void Particle::SetColorGradientValues(glm::vec4 origin, glm::vec4 destination)
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

float Particle::Lerp(float p0, float p1, float t)
{
	return p0 + (p1 - p0) * t;
}
glm::vec4 Particle::Lerp(glm::vec4& p0, glm::vec4& p1, float t)
{
	return p0 + (p1 - p0) * t;
}