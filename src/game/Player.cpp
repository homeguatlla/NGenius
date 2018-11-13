#include "stdafx.h"
#include "Player.h"
#include "ShooterGameConstants.h"

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

#include "../resources/events/characterControllerEvents/ForwardEvent.h"
#include "../resources/events/characterControllerEvents/BackwardEvent.h"
#include "../resources/events/characterControllerEvents/ZoomEvent.h"
#include "../resources/events/characterControllerEvents/JumpEvent.h"
#include "../resources/events/characterControllerEvents/TurnEvent.h"
#include "../resources/events/characterControllerEvents/PitchEvent.h"

#include "../input/IInputListener.h"
#include "../input/bindings/KeyToEventBind.h"
#include "../input/bindings/MouseToEventBind.h"

#include "../resources/components/InputComponent.h"
#include "../resources/components/PhysicsComponent.h"
#include "../resources/components/CollisionComponent.h"
#include "../resources/components/RotationComponent.h"
#include "../resources/components/GameEventsComponent.h"
#include "../resources/components/DebugComponent.h"
#include "../resources/components/AnimationComponent.h"
#include "../resources/components/EnergyWallCollisionComponent.h"
#include "../resources/components/EnvironmentModificatorComponent.h"

#include "../resources/renderers/IndicesRenderer.h"
#include "../resources/renderers/WireframeRenderer.h"

#include "../resources/systems/PhysicsSystem.h"

#include <GLFW/glfw3.h>

Player::Player(NGenius& engine, GameScene* scene, Transformation* transformation) :
	mEngine(engine),
	mScene(scene)
{
	Create(transformation);
}

Player::~Player()
{
}

void Player::Create(Transformation* transformation)
{
	//PLAYER
	IMaterial* material = mEngine.CreateMaterial("player", mEngine.GetShader("animated_model"));
	material->AddEffect(new MaterialEffectDiffuseTexture(static_cast<Texture*>(mEngine.GetTexture("material_farmer_diffuse")), glm::vec3(1.0f, 1.0f, 1.0f), 1));
	//material->AddEffect(new MaterialEffectNormalTexture(static_cast<Texture*>(mEngine.GetTexture("material_farmer_normalmap")), 1.0f));
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
	inputComponent->AddConverter(new MouseToEventBind(GLFW_MOUSE_BUTTON_MIDDLE, new ZoomEvent()));

	Model* model = mEngine.GetModel("farmer");
	//IRenderer* renderer = new VerticesRenderer(model, material);
	IRenderer* renderer = new IndicesRenderer(model, material);

	mCharacter = new Character(	transformation,
								renderer,
								inputComponent,
								new GameEventsComponent(),
								new PhysicsComponent(false, PhysicsSystem::GRAVITY_VALUE),
								new CollisionComponent(),
								PLAYER_RUN_SPEED,
								PLAYER_TURN_SPEED,
								PLAYER_UPWARDS_HEIGHT
							);
	mCharacter->AddComponent(new EnergyWallCollisionComponent());
	IRenderer* boundingBoxRenderer = new WireframeRenderer(mEngine.GetModel("cube"), mEngine.GetMaterial(MaterialsLibrary::WIREFRAME_MATERIAL_NAME));
	mCharacter->AddComponent(new DebugComponent(boundingBoxRenderer));
	mCharacter->AddComponent(new EnvironmentModificatorComponent());
	AnimationComponent* animationComponent = new AnimationComponent();
	animationComponent->AddAnimation(mEngine.GetAnimation("animation_0"));
	mCharacter->AddComponent(animationComponent);
	mScene->AddEntity(mCharacter);
}

GameEntity* Player::GetEntity()
{
	return mCharacter;
}