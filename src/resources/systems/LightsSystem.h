#pragma once

#include <vector>

class Light;
class EntitiesSystem;

class LightsSystem
{
	std::vector<Light*> mLights;
	std::vector<Light*> mNewLightsToAdd;
	std::vector<Light*> mLightsToRemove;
	typedef std::vector<Light*>::iterator GameLightIterator;
	EntitiesSystem* mEntitiesSystem;

public:
	LightsSystem(EntitiesSystem* entitiesSystem);
	~LightsSystem();

	void Update(float elapsedTime);
	void AddLight(Light* light);
	void RemoveLight(Light* light);

private:
	void ReleaseLights(std::vector<Light*>* lights);
	void RemoveLights();
	void AddNewLights();
};

