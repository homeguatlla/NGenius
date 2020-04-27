#include "stdafx.h"
#include "LightsSystem.h"
#include "src/resources/IGameEntity.h"
#include "src/resources/entities/Light.h"
#include "src/resources/scene/GameScene.h"

#include <algorithm>

LightsSystem::LightsSystem(GameScene* gameScene) :
mGameScene(gameScene)
{
}


LightsSystem::~LightsSystem()
{
	mLights.clear();
	mNewLightsToAdd.clear();
	mLightsToRemove.clear();
}

void LightsSystem::Update(float elapsedTime)
{
	RemoveLights();
	AddNewLights();
}

void LightsSystem::AddLight(std::shared_ptr<Light> light)
{
	mNewLightsToAdd.push_back(light);
}

void LightsSystem::RemoveLight(std::shared_ptr<Light> light)
{
	mLightsToRemove.push_back(light);
}

void LightsSystem::AddNewLights()
{
	for (auto&& light : mNewLightsToAdd)
	{
		mLights.push_back(light);
		if (light->GetRenderer() != nullptr)
		{
			mGameScene->AddEntity(light);
		}
	}
	mNewLightsToAdd.clear();
}

void LightsSystem::RemoveLights()
{
	for (auto&& light : mLightsToRemove)
	{
		GameLightIterator it = std::find_if(mLights.begin(), mLights.end(), [&](std::shared_ptr<Light> a) { return a == light; });
		mGameScene->RemoveEntity(light);
		mLights.erase(it);
	}
	mLightsToRemove.clear();
}
