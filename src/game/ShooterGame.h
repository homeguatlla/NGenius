#pragma once
#include "../src/NGenius.h"

class GameHUD;
class GameScene;
class MarsPlanet;
class ICamera;

class ShooterGame 
{
	GameScene* mScene;
	GameHUD* mGameHUD;
	MarsPlanet* mPlanet;
	ICamera* mGameplayCamera;

public:
	ShooterGame();
	~ShooterGame();

	void Start(NGenius& engine);
	void Update(float elapsedTime);

private:
	void CreateCameras(NGenius& engine);
};

