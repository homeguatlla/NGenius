#include "stdafx.h"
#include "WindManager.h"

const float WIND_SPEED = 1.0f;

WindManager::WindManager(int gridSize) :
	mGridSize(gridSize),
	mSpeed(WIND_SPEED),
	mAccumulatedTime(0.0)
{
	mWindMap.resize(mGridSize);
	for (int c = 0; c < mGridSize; ++c)
	{
		mWindMap[c].resize(mGridSize);
	}
	CreateAttractors(1);
}


WindManager::~WindManager()
{
}


void WindManager::Update(float deltaTime)
{
	mAccumulatedTime += deltaTime;

	/*for (int c = 0; c < mGridSize; ++c)
	{
		for (int r = 0; r < mGridSize; ++r)
		{
			mWindMap[c][r] = glm::vec3(-c, -r, 0.0f);
			
			for (int a = 0; a < mAttractors.size(); ++a)
			{
				float distance = glm::distance(glm::vec3(c, r, 0.0f), mAttractors[a]->position);
				mWindMap[c][r] += (mAttractors[a]->position * mAttractors[a]->force / distance);
			}
		}
	}*/
}

void WindManager::CreateAttractors(int numAttractors)
{
	for (int i = 0; i < numAttractors; ++i)
	{
		WindAttractor* attractor = new WindAttractor();
		attractor->position = glm::vec3(rand() % mGridSize, rand() % mGridSize, 0.0f);
		attractor->force = 1.0f;
		mAttractors.push_back(attractor);
	}
}

void WindManager::CreateRepellers(int numRepellers)
{
	for (int i = 0; i < mAttractors.size(); ++i)
	{
		delete mAttractors[i];
	}
	mAttractors.clear();
}

void WindManager::ReleaseAttractors()
{

}

void WindManager::ReleaseRepellers()
{
	for (int i = 0; i < mAttractors.size(); ++i)
	{
		delete mRepellers[i];
	}
	mRepellers.clear();
}
