#include "stdafx.h"
#include "Player.h"
#include "ShooterGameConstants.h"
#include "controllers/InventoryController.h"
#include "controllers/HealthController.h"

#include "../NGenius.h"
#include "../resources/scene/GameScene.h"

#include "../resources/materials/MaterialsLibrary.h"
#include "../resources/materials/IMaterial.h"
#include "../resources/materials/effects/MaterialEffectDiffuseTexture.h"
#include "../resources/materials/effects/MaterialEffectNormalTexture.h"
#include "../resources/materials/effects/MaterialEffectDirectionalLightProperties.h"
#include "../resources/materials/effects/MaterialEffectFogProperties.h"
#include "../resources/materials/effects/MaterialEffectShadowProperties.h"
#include "../resources/materials/effects/MaterialEffectMatrix4Array.h"
#include "../resources/materials/effects/MaterialEffectClippingPlane.h"
#include "../resources/materials/effects/MaterialEffectFloat.h"

#include "../resources/textures/Texture.h"

#include "../resources/entities/Character.h"
#include "../resources/events/CharacterControllerEvent.h"
#include "../resources/GameEvent.h"


#include "../resources/events/characterControllerEvents/ForwardEvent.h"
#include "../resources/events/characterControllerEvents/BackwardEvent.h"
#include "../resources/events/characterControllerEvents/JumpEvent.h"
#include "../resources/events/characterControllerEvents/TurnEvent.h"
#include "../resources/events/characterControllerEvents/HealthEvent.h"

#include "events/PickupGameEntityEvent.h"
#include "events/DropItemInventoryEvent.h"
#include "events/NextPreviousInventoryItemEvent.h"


#include "../input/IInputListener.h"
#include "../input/bindings/KeyToEventBind.h"
#include "../input/bindings/MouseToEventBind.h"
#include "input/bindings/KeyToPickUpGameEntityEventBind.h"

#include "../resources/components/InputComponent.h"
#include "../resources/components/PhysicsComponent.h"
#include "../resources/components/CollisionComponent.h"
#include "../resources/components/RotationComponent.h"
#include "../resources/components/GameEventsComponent.h"
#include "../resources/components/DebugComponent.h"
#include "../resources/components/AnimationComponent.h"
#include "../resources/components/EnergyWallCollisionComponent.h"
#include "../resources/components/EnvironmentModificatorComponent.h"
#include "../resources/components/HealthComponent.h"
#include "../resources/components/DamageComponent.h"

#include "../resources/renderers/IndicesRenderer.h"
#include "../resources/renderers/WireframeRenderer.h"

#include "../resources/systems/PhysicsSystem.h"

#include <GLFW/glfw3.h>
#include <assert.h>

Player::Player(GameScene* scene, Transformation* transformation, InventoryController* inventoryController, HealthController* healthController) :
	mEngine(&NGenius::GetInstance()),
	mScene(scene),
	mCharacter(nullptr),
	mInventoryController(inventoryController),
	mHealthController(healthController)
{
	assert(inventoryController != nullptr);
	assert(healthController != nullptr);
	Create(transformation);
}

Player::~Player()
{
}

void Player::Create(Transformation* transformation)
{
	//PLAYER
	IMaterial* material = mEngine->CreateMaterial("player", mEngine->GetShader("animated_model"));
	material->AddEffect(new MaterialEffectDiffuseTexture(static_cast<Texture*>(mEngine->GetTexture("material_farmer_diffuse")), glm::vec3(1.0f, 1.0f, 1.0f), 1));
	//material->AddEffect(new MaterialEffectNormalTexture(static_cast<Texture*>(mEngine->GetTexture("material_farmer_normalmap")), 1.0f));
	material->AddEffect(new MaterialEffectDirectionalLightProperties());
	material->AddEffect(new MaterialEffectFogProperties());
	material->AddEffect(new MaterialEffectShadowProperties(3));
	material->AddEffect(new MaterialEffectClippingPlane());
	material->AddEffect(new MaterialEffectMatrix4Array());

	InputComponent* inputComponent = new InputComponent();
	inputComponent->AddConverter(new KeyToEventBind(GLFW_KEY_W, new ForwardEvent()));
	inputComponent->AddConverter(new KeyToEventBind(GLFW_KEY_S, new BackwardEvent()));
	inputComponent->AddConverter(new MouseToEventBind(-1, new TurnEvent()));
	inputComponent->AddConverter(new KeyToEventBind(GLFW_KEY_SPACE, new JumpEvent()));

	inputComponent->AddConverter(new KeyToPickUpGameEntityEventBind(GLFW_KEY_E, new PickupGameEntityEvent(), this));
	inputComponent->AddConverter(new KeyToEventBind(GLFW_KEY_Q, new DropItemInventoryEvent()));
	inputComponent->AddConverter(new MouseToEventBind(GLFW_MOUSE_BUTTON_MIDDLE, new NextPreviousInventoryItemEvent()));
	
	Model* model = mEngine->GetModel("farmer");
	//IRenderer* renderer = new VerticesRenderer(model, material);
	IRenderer* renderer = new IndicesRenderer(model, material);

	mCharacter = new Character(	transformation,
								renderer,
								inputComponent,
								new GameEventsComponent(),
								new PhysicsComponent(false, MARS_GRAVITY_VALUE),
								new CollisionComponent(),
								PLAYER_RUN_SPEED,
								PLAYER_TURN_SPEED,
								PLAYER_UPWARDS_HEIGHT
							);
	mCharacter->AddComponent(new EnergyWallCollisionComponent());
	IRenderer* boundingBoxRenderer = new WireframeRenderer(mEngine->GetModel("cube"), mEngine->GetMaterial(MaterialsLibrary::WIREFRAME_MATERIAL_NAME));
	mCharacter->AddComponent(new DebugComponent(boundingBoxRenderer));
	mCharacter->AddComponent(new EnvironmentModificatorComponent());
	AnimationComponent* animationComponent = new AnimationComponent();
	animationComponent->AddAnimation(mEngine->GetAnimation("animation_0"));
	mCharacter->AddComponent(animationComponent);
	
	mCharacter->AddComponent(new HealthComponent(MAX_PLAYER_LIFE));
	mCharacter->AddComponent(new DamageComponent(0.0f));

	mScene->AddEntity(mCharacter);
}

GameEntity* Player::GetEntity()
{
	return mCharacter;
}

void Player::Update(float elapsedTime)
{
	GameEventsComponent* gameEventsComponent = mCharacter->GetComponent<GameEventsComponent>();
	
	gameEventsComponent->StartIterate();
	while (gameEventsComponent->HasEvents())
	{
		GameEvent* event = gameEventsComponent->GetEvent();
		if (event->IsOfType<PickupGameEntityEvent>())
		{
			mInventoryController->OnPickUp(static_cast<const PickupGameEntityEvent*>(event));
			gameEventsComponent->ConsumeEvent();
		}
		else if (event->IsOfType<DropItemInventoryEvent>())
		{
			DropItemInventoryEvent* dropEvent = static_cast<DropItemInventoryEvent*>(event);
			if (dropEvent->IsPressed())
			{
				dropEvent->SetPosition(mCharacter->GetTransformation()->GetPosition());
				mInventoryController->OnDrop(dropEvent);
				gameEventsComponent->ConsumeEvent();
			}
		}
		else if (event->IsOfType<NextPreviousInventoryItemEvent>())
		{
			mInventoryController->OnNextPreviousItem(static_cast<NextPreviousInventoryItemEvent*>(event));
			gameEventsComponent->ConsumeEvent();
		}
		else if (event->IsOfType<HealthEvent>())
		{
			mHealthController->OnHealthEvent(static_cast<HealthEvent*>(event));
			gameEventsComponent->ConsumeEvent();
		}
	}
}