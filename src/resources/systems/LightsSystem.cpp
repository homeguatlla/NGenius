#include "stdafx.h"
#include "LightsSystem.h"
#include "../GameEntity.h"
#include "../entities/Light.h"
#include "EntitiesSystem.h"

#include <algorithm>

LightsSystem::LightsSystem(EntitiesSystem* entitiesSystem) :
mEntitiesSystem(entitiesSystem)
{
}


LightsSystem::~LightsSystem()
{
	ReleaseLights(&mLights);
	ReleaseLights(&mNewLightsToAdd);	
	mLightsToRemove.clear();
}

void LightsSystem::Update(float elapsedTime)
{
	RemoveLights();
	AddNewLights();
}

void LightsSystem::AddLight(Light* light)
{
	mNewLightsToAdd.push_back(light);
}

void LightsSystem::RemoveLight(Light* light)
{
	mLightsToRemove.push_back(light);
}

void LightsSystem::AddNewLights()
{
	for (Light* light : mNewLightsToAdd)
	{
		mLights.push_back(light);
		if (light->GetRenderer() != nullptr)
		{
			mEntitiesSystem->AddEntity(light);
		}
	}
	mNewLightsToAdd.clear();
}

void LightsSystem::RemoveLights()
{
	for (Light* light : mLightsToRemove)
	{
		GameLightIterator it = std::find_if(mLights.begin(), mLights.end(), [&](Light* a) { return a == light; });
		mEntitiesSystem->RemoveEntity(light);
		mLights.erase(it);
	}
	mLightsToRemove.clear();
}

void LightsSystem::ReleaseLights(std::vector<Light*>* lights)
{
	for (Light* light : *lights)
	{
		delete light;
	}
	lights->clear();
}
