#pragma once
#include <vector>

class NGenius;
class GameHUD;
class GameScene;
class MarsPlanet;
class ICamera;
class Player;
class GameEntity;
class Inventory;
class InventoryController;

class ShooterGame 
{
	GameScene* mScene;
	GameHUD* mGameHUD;
	MarsPlanet* mPlanet;
	ICamera* mGameplayCamera;
	Player* mPlayer;
	GameEntity* mThirdPersonCameraEntity;
	Inventory* mInventory;
	InventoryController* mInventoryController;

public:
	ShooterGame();
	~ShooterGame();

	void Init();
	void Start();
	void Update(float elapsedTime);

private:
	void CreateThirdpersonCamera();
	void CreateEnvironment();
	void CreateInitialProps();
};

