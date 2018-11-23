#include "stdafx.h"
#include "ShooterGame.h"
#include "hud/GameHUD.h"
#include "ShooterGameConstants.h"
#include "../EngineConstants.h"

#include "../Singleton.h"
#include "maps/MarsPlanet.h"
#include "inventory/Inventory.h"
#include "controllers/InventoryController.h"
#include "Player.h"

#include "../src/NGenius.h"
#include "../resources/camera/PerspectiveCamera.h"

#include "../resources/components/InputComponent.h"
#include "../resources/components/GameEventsComponent.h"
#include "../resources/components/CollisionComponent.h"
#include "../resources/components/ThirdPersonCameraComponent.h"
#include "../resources/components/OverWaterComponent.h"
#include "../input/bindings/MouseToEventBind.h"

#include "../resources/events/characterControllerEvents/PitchEvent.h"
#include "../resources/events/characterControllerEvents/ZoomEvent.h"

#include "../resources/materials/IMaterial.h"
#include "../resources/materials/effects/MaterialEffectDirectionalLightProperties.h"

#include "../resources/renderers/IRenderer.h"

#include "../resources/scene/GameScene.h"

#include "entities/EntitiesFactory.h"

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

void ShooterGame::Init()
{
	NGenius::GetInstance().SetGravity(MARS_GRAVITY_VALUE);
}

void ShooterGame::Start()
{
	mGameplayCamera = NGenius::GetInstance().GetCamera(EngineConstants::GAMEPLAY_CAMERA);
	//mGameplayCamera->SetPosition(glm::vec3(0.0f, 10.0f, 5.0f));
	mScene = NGenius::GetInstance().CreateGameScene(GAME_SCENE_NAME);

	mGameHUD = new GameHUD(mScene);
	mPlanet = new MarsPlanet(mScene, mGameplayCamera);

	mInventory = new Inventory(NUM_ITEMS_INVENTORY);
	//el juego tiene las distintas vistas del inventory, hud tipo fortnite o hud tipo menú a fullscreen
	//y por tanto, es quién le puede cambiar la vista en un momento dado
	//entonces el player solo tiene la mochila (inventory) que le dan con sus instrucciones para usarla el inventory controller
	//esto nos permite cambiar el inventory y la vista en cualquier momento
	mInventoryController = new InventoryController(mScene, mInventory, mGameHUD->GetItemsListHUD());
	mPlayer = new Player(mScene, new Transformation(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), glm::vec3(0.18f)), mInventoryController);

	
	NGenius::GetInstance().SetCastingShadowsTarget(mPlayer->GetEntity());

	CreateThirdpersonCamera();
	CreateEnvironment();

	CreateInitialProps();

	mGameHUD->SetVisibility(true);
}

void ShooterGame::Update(float elapsedTime)
{
	mPlayer->Update(elapsedTime);
	mGameHUD->Update(elapsedTime);
}

void ShooterGame::CreateThirdpersonCamera()
{
	InputComponent* inputComponent = new InputComponent();
	//inputComponent->AddConverter(new MouseToEventBind(GLFW_MOUSE_BUTTON_MIDDLE, new ZoomEvent()));
	inputComponent->AddConverter(new MouseToEventBind(-1, new PitchEvent()));

	mThirdPersonCameraEntity = new GameEntity(new Transformation(mGameplayCamera->GetPosition(), glm::vec3(0.0f), glm::vec3(0.0f)),
		nullptr);// new CubeRenderer(mengine->GetShader("default")));

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

	mThirdPersonCameraEntity->AddComponent(new GameEventsComponent());
	mThirdPersonCameraEntity->AddComponent(inputComponent);

	/*
	if (mIsWaterEnabled)
	{
		mThirdPersonCameraEntity->AddComponent(new OverWaterComponent(mWaterHeight));
	}*/

	mScene->AddEntity(mThirdPersonCameraEntity);
}

void ShooterGame::CreateEnvironment()
{
	//mars
	NGenius::GetInstance().AddSunLightFrame(1200.0f, 90.0f, glm::vec3(251.0f, 114.0f, 55.0f) / 255.0f, glm::vec3(251.0f, 114.0f, 55.0f) / 255.0f, 0.004f, 1.5f, "day_cubemap");
	NGenius::GetInstance().AddSunLightFrame(1800.0f, 135.0f, glm::vec3(0.93f, 0.64f, 0.78f), glm::vec3(218.0f, 74.0f, 43.0f) / 255.0f, 0.04f, 1.5f, "day_cubemap");
	NGenius::GetInstance().AddSunLightFrame(2400.0f, 270.0f, glm::vec3(0.86f, 0.64f, 0.93f), glm::vec3(0.0f), 0.004f, 1.5f, "night_cubemap");
	NGenius::GetInstance().AddSunLightFrame(600.0f, 45.0f, glm::vec3(251.0f, 114.0f, 55.0f) / 255.0f, glm::vec3(251.0f, 114.0f, 55.0f) / 255.0f, 0.08f, 1.5f, "day_cubemap");
}

void ShooterGame::CreateInitialProps()
{
	unsigned int numProps = 4;
	float areaSize = 20.0f;
	
	EntitiesFactory factory;

	for (unsigned int i = 0; i < numProps; ++i)
	{
		float randValue = (rand() % 1000) * areaSize / 1000.0f;
		float x = -areaSize * 0.5f + randValue;
		randValue = (rand() % 1000) * areaSize / 1000.0f;
		float z = -areaSize * 0.5f + randValue;

		x = 0.0f; z = 0.0f;
		float y = NGenius::GetInstance().GetHeight(glm::vec2(x, z));
		
		factory.Create(InventoryItem::ITEM_WATER_BATTERY, glm::vec3(x, y, z), mScene);
	}
}