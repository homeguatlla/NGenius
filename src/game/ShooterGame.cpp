#include "stdafx.h"
#include "ShooterGame.h"
#include "../resources/camera/PerspectiveCamera.h"
#include "hud/GameHUD.h"
#include "ShooterGameConstants.h"
#include "../GameConstants.h"

#include "../Singleton.h"
#include "maps/MarsPlanet.h"
#include "inventory/Inventory.h"
#include "Player.h"

typedef Singleton<Inventory> SInventory;

ShooterGame::ShooterGame()
{
}


ShooterGame::~ShooterGame()
{
}

void ShooterGame::Start(NGenius& engine)
{
	mGameplayCamera = engine.GetCamera(EngineConstants::GAMEPLAY_CAMERA);
	mGameplayCamera->SetPosition(glm::vec3(0.0f, 30.0f, 5.0f));
	mScene = engine.CreateGameScene(GAME_SCENE_NAME);
	mGameHUD = new GameHUD(engine, mScene);
	mPlanet = new MarsPlanet(engine, mScene, mGameplayCamera);
	mPlayer = new Player(engine, mScene, new Transformation(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), glm::vec3(0.18f)));
	//mGameHUD->SetVisibility(false);
}

void ShooterGame::Update(float elapsedTime)
{
	mGameHUD->Update(elapsedTime);
}