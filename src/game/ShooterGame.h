#pragma once
#include "../src/NGenius.h"

class GameHUD;
class GameScene;
class MarsPlanet;
class ICamera;
class Player;

class ShooterGame 
{
	GameScene* mScene;
	GameHUD* mGameHUD;
	MarsPlanet* mPlanet;
	ICamera* mGameplayCamera;
	Player* mPlayer;

public:
	ShooterGame();
	~ShooterGame();

	void Start(NGenius& engine);
	void Update(float elapsedTime);
};

