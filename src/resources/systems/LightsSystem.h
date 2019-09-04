#pragma once

#include <vector>

class Light;
class GameScene;

class LightsSystem
{
	std::vector<Light*> mLights;
	std::vector<Light*> mNewLightsToAdd;
	std::vector<Light*> mLightsToRemove;
	typedef std::vector<Light*>::iterator GameLightIterator;
	GameScene* mGameScene;

public:
	LightsSystem(GameScene* gameScene);
	~LightsSystem();

	void Update(float elapsedTime);
	void AddLight(Light* light);
	void RemoveLight(Light* light);

private:
	void ReleaseLights(std::vector<Light*>& lights);
	void RemoveLights();
	void AddNewLights();
};

