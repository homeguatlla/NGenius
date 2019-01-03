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
class HealthController;
class EnergyWall;
struct GLFWwindow;

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
	HealthController* mHealthController;
	EnergyWall* mEnergyWall;

public:
	ShooterGame();
	~ShooterGame();

	void Init();
	void Start();
	void Update(float elapsedTime);
	void UpdateInput(GLFWwindow* window);

private:
	void CreatePlayer();
	void CreateThirdpersonCamera();
	void CreateEnvironment();
	void CreateInitialProps();
	void CreateEnergyWall();
};

