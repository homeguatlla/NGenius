#include "stdafx.h"
#include "ShooterGame.h"
#include "hud/GameHUD.h"
#include "ShooterGameConstants.h"
#include "../GameConstants.h"

#include "../Singleton.h"
#include "maps/MarsPlanet.h"
#include "inventory/Inventory.h"
#include "Player.h"

#include "../resources/camera/PerspectiveCamera.h"

#include "../resources/components/InputComponent.h"
#include "../resources/components/CharacterComponent.h"
#include "../resources/components/CollisionComponent.h"
#include "../resources/components/ThirdPersonCameraComponent.h"
#include "../resources/components/OverWaterComponent.h"

#include "../input/bindings/MouseToEventBind.h"

#include "../resources/events/characterControllerEvents/PitchEvent.h"
#include "../resources/events/characterControllerEvents/ZoomEvent.h"

#include "../resources/scene/GameScene.h"

typedef Singleton<Inventory> SInventory;


#include <GLFW/glfw3.h>

ShooterGame::ShooterGame() :
	mScene(nullptr),
	mGameHUD(nullptr),
	mPlanet(nullptr),
	mGameplayCamera(nullptr),
	mPlayer(nullptr),
	mThirdPersonCameraEntity(nullptr)
{
}


ShooterGame::~ShooterGame()
{
}

void ShooterGame::Start(NGenius& engine)
{
	mGameplayCamera = engine.GetCamera(EngineConstants::GAMEPLAY_CAMERA);
	//mGameplayCamera->SetPosition(glm::vec3(0.0f, 10.0f, 5.0f));
	mScene = engine.CreateGameScene(GAME_SCENE_NAME);
	mGameHUD = new GameHUD(engine, mScene);
	mPlanet = new MarsPlanet(engine, mScene, mGameplayCamera);
	mPlayer = new Player(engine, mScene, new Transformation(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), glm::vec3(0.18f)));
	
	engine.SetCastingShadowsTarget(mPlayer->GetEntity());

	CreateThirdpersonCamera();
	//mGameHUD->SetVisibility(false);
}

void ShooterGame::Update(float elapsedTime)
{
	mGameHUD->Update(elapsedTime);
}

void ShooterGame::CreateThirdpersonCamera()
{
	InputComponent* inputComponent = new InputComponent();
	inputComponent->AddConverter(new MouseToEventBind(GLFW_MOUSE_BUTTON_MIDDLE, new ZoomEvent()));
	inputComponent->AddConverter(new MouseToEventBind(-1, new PitchEvent()));

	mThirdPersonCameraEntity = new GameEntity(new Transformation(mGameplayCamera->GetPosition(), glm::vec3(0.0f), glm::vec3(0.0f)),
		nullptr);// new CubeRenderer(mEngine.GetShader("default")));

	glm::vec3 targetOffset(0.0f, 1.8f, 0.0f); //head
	ThirdPersonCameraComponent* thirdPersonCameraComponent = new ThirdPersonCameraComponent(
		static_cast<PerspectiveCamera*>(mGameplayCamera),
		mPlayer->GetEntity(),
		targetOffset,
		4.0f,
		EngineConstants::PLAYER_PITCH,
		EngineConstants::PLAYER_PITCH_SPEED,
		EngineConstants::PLAYER_ZOOM_SPEED);
	mThirdPersonCameraEntity->AddComponent(thirdPersonCameraComponent);
	mThirdPersonCameraEntity->AddComponent(new CollisionComponent());

	mThirdPersonCameraEntity->AddComponent(new CharacterComponent());
	mThirdPersonCameraEntity->AddComponent(inputComponent);

	/*
	if (mIsWaterEnabled)
	{
		mThirdPersonCameraEntity->AddComponent(new OverWaterComponent(mWaterHeight));
	}*/

	mScene->AddEntity(mThirdPersonCameraEntity);
}