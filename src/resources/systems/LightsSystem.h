#pragma once

#include <vector>
#include <memory>

class Light;
class GameScene;

class LightsSystem
{
	std::vector<std::shared_ptr<Light>> mLights;
	std::vector<std::shared_ptr<Light>> mNewLightsToAdd;
	std::vector<std::shared_ptr<Light>> mLightsToRemove;
	typedef std::vector<std::shared_ptr<Light>>::iterator GameLightIterator;
	GameScene* mGameScene;

public:
	LightsSystem(GameScene* gameScene);
	~LightsSystem();

	void Update(float elapsedTime);
	void AddLight(std::shared_ptr<Light> light);
	void RemoveLight(std::shared_ptr<Light> light);

private:
	void RemoveLights();
	void AddNewLights();
};

