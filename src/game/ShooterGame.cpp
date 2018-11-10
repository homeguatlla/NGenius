#include "stdafx.h"
#include "ShooterGame.h"
#include "../resources/camera/PerspectiveCamera.h"
#include "hud/GameHUD.h"
#include "ShooterGameConstants.h"

#include "../Singleton.h"
#include "maps/MarsPlanet.h"
#include "inventory/Inventory.h"

typedef Singleton<Inventory> SInventory;

ShooterGame::ShooterGame()
{
}


ShooterGame::~ShooterGame()
{
}

void ShooterGame::Start(NGenius& engine)
{
	CreateCameras(engine);
	mScene = engine.CreateGameScene(GAME_SCENE_NAME);
	mGameHUD = new GameHUD(engine, mScene);
	mPlanet = new MarsPlanet(engine, mScene, mGameplayCamera);
	//mGameHUD->SetVisibility(false);
}

void ShooterGame::Update(float elapsedTime)
{
	mGameHUD->Update(elapsedTime);
}

void ShooterGame::CreateCameras(NGenius& engine)
{
	float screenWidth = static_cast<float>(engine.GetScreenWidth());
	float screenHeight = static_cast<float>(engine.GetScreenHeight());
	float aspectRatio = screenWidth / screenHeight;

	mGameplayCamera = new PerspectiveCamera("gameplay_camera", VIEW_ANGLE, aspectRatio, NEAR_PLANE, FAR_PLANE);
	mGameplayCamera->SetPosition(glm::vec3(0.0f, 5.0f, 5.0f));
	mGameplayCamera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	mGameplayCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
	engine.AddCamera(mGameplayCamera);
}