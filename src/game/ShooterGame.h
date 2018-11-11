#pragma once
#include "../src/NGenius.h"

class GameHUD;
class GameScene;
class MarsPlanet;
class ICamera;
class Player;
class GameEntity;

class ShooterGame 
{
	GameScene* mScene;
	GameHUD* mGameHUD;
	MarsPlanet* mPlanet;
	ICamera* mGameplayCamera;
	Player* mPlayer;
	GameEntity* mThirdPersonCameraEntity;

public:
	ShooterGame();
	~ShooterGame();

	void Start(NGenius& engine);
	void Update(float elapsedTime);

private:
	void CreateThirdpersonCamera();
};

