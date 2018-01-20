#include "stdafx.h"
#include "EnergyWall.h"

#include "../../renderer/IRenderer.h"

EnergyWall::EnergyWall(Transformation* transformation, IRenderer* renderer, float maxLiveTime) : 
GameEntity(transformation, renderer),
mMaxLiveTime(maxLiveTime),
mLiveTime(0.0f),
mContactPoint(0.0f)
{

}


EnergyWall::~EnergyWall()
{
}

EnergyWall* EnergyWall::DoClone() const
{
	IRenderer* cloneRenderer = GetRenderer()->Clone();
	EnergyWall* clone = new EnergyWall(new Transformation(*GetTransformation()), cloneRenderer, mLiveTime);

	return clone;

}
void EnergyWall::Update(float elapsedTime)
{
	GameEntity::Update(elapsedTime);

	assert(mLiveTime <= mMaxLiveTime);

	mLiveTime = glm::max(0.0f, mLiveTime - elapsedTime);
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