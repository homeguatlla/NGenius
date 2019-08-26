#include "stdafx.h"
#include "NGenius.h"

#include <Windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "input/InputHandler.h"

#include "resources/systems/renderSystem/RenderSystem.h"
#include "resources/systems/PhysicsSystem.h"
#include "resources/systems/ParticlesSystem.h"
#include "resources/systems/LightsSystem.h"
#include "resources/systems/InputSystem.h"
#include "resources/systems/DebugSystem.h"
#include "resources/systems/SpacePartitionSystem.h"
#include "resources/systems/EnvironmentSystem/EnvironmentSystem.h"
#include "resources/systems/AnimationSystem.h"

#include "resources/scene/GameScene.h"
#include "resources/entities/ParticlesEmitter.h"
#include "resources/textures/Texture.h"

#include "fsm/states/RunState.h"
#include "fsm/states/FreeModeOnlyCameraState.h"
#include "fsm/states/FreeModeOnlyPlayerState.h"
#include "fsm/states/InitialState.h"
#include "fsm/states/LoadingState.h"
#include "fsm/states/StartState.h"
#include "fsm/states/ShutDownState.h"
#include "fsm/transitions/EnterLoadingTransition.h"
#include "fsm/transitions/EnterStartTransition.h"
#include "fsm/transitions/EnterRunTransition.h"
#include "fsm/transitions/EnterShutDownTransition.h"
#include "fsm/transitions/EnterExitFreeModeOnlyCameraTransition.h"
#include "fsm/transitions/EnterExitFreeModeOnlyPlayerTransition.h"


#include "statistics/Statistics.h"
#include "guiTool/GuiTool.h"

#include "utils/serializer/XMLSerializer.h"
#include "utils/serializer/XMLDeserializer.h"
#include "../Memory.h"
#include "resources/InstantiableObject.h"
#include "resources/components/AnimationComponent.h"
#include "resources/components/BillboardComponent.h"
#include "resources/components/CameraComponent.h"
#include "resources/components/EnvironmentAffectedComponent.h"
#include "resources/components/EnergyWallCollisionComponent.h"
#include "resources/components/DebugComponent.h"
#include "resources/components/CharacterComponent.h"
#include "resources/components/InputComponent.h"
#include "resources/components/EnvironmentModificatorComponent.h"
#include "resources/components/CollisionComponent.h"
#include "resources/components/LODComponent.h"
#include "resources/components/SpacePartitionComponent.h"
#include "resources/components/RotationComponent.h"
#include "resources/components/PhysicsComponent.h"
#include "resources/components/OverWaterComponent.h"
#include "resources/components/ThirdPersonCameraComponent.h"

#include "resources/events/characterControllerEvents/BackwardEvent.h"
#include "resources/events/characterControllerEvents/ButtonLeftEvent.h"
#include "resources/events/characterControllerEvents/ButtonRightEvent.h"
#include "resources/events/characterControllerEvents/ForwardEvent.h"
#include "resources/events/characterControllerEvents/JumpEvent.h"
#include "resources/events/characterControllerEvents/PitchEvent.h"
#include "resources/events/characterControllerEvents/TurnEvent.h"
#include "resources/events/characterControllerEvents/ZoomEvent.h"

NGenius::NGenius(std::string applicationName, float screenWidth, float screenHeight) :
mRenderSystem(nullptr),
mPhysicsSystem(nullptr),
mEntitiesSystem(nullptr),
mParticlesSystem(nullptr),
mLightsSystem(nullptr),
mInputSystem(nullptr),
mSpacePartitionSystem(nullptr),
mDebugSystem(nullptr),
mEnvironmentSystem(nullptr),
mAnimationSystem(nullptr),
mInputHandler(nullptr),
mStatistics(nullptr),
mApplicationName(applicationName),
mIsSpacePartitionEnabled(true),
mGameScene(nullptr)
{
	CreateSystems(screenWidth, screenHeight);

	InstantiableObject::RegisterComponentType<AnimationComponent>();
	InstantiableObject::RegisterComponentType<BillboardComponent>();
	InstantiableObject::RegisterComponentType<CameraComponent>();
	InstantiableObject::RegisterComponentType<CharacterComponent>();
	InstantiableObject::RegisterComponentType<CollisionComponent>();
	InstantiableObject::RegisterComponentType<DebugComponent>();
	InstantiableObject::RegisterComponentType<EnergyWallCollisionComponent>();
	InstantiableObject::RegisterComponentType<EnvironmentAffectedComponent>();
	InstantiableObject::RegisterComponentType<EnvironmentModificatorComponent>();
	InstantiableObject::RegisterComponentType<InputComponent>();
	InstantiableObject::RegisterComponentType<LODComponent>();
	InstantiableObject::RegisterComponentType<OverWaterComponent>();
	InstantiableObject::RegisterComponentType<PhysicsComponent>();
	InstantiableObject::RegisterComponentType<RotationComponent>();
	InstantiableObject::RegisterComponentType<SpacePartitionComponent>();
	InstantiableObject::RegisterComponentType<ThirdPersonCameraComponent>();

	//Registering game events
	InstantiableObject::RegisterGameEvent<BackwardEvent>();
	InstantiableObject::RegisterGameEvent<ButtonLeftEvent>();
	InstantiableObject::RegisterGameEvent<ButtonRightEvent>();
	InstantiableObject::RegisterGameEvent<ForwardEvent>();
	InstantiableObject::RegisterGameEvent<JumpEvent>();
	InstantiableObject::RegisterGameEvent<PitchEvent>();
	InstantiableObject::RegisterGameEvent<TurnEvent>();
	InstantiableObject::RegisterGameEvent<ZoomEvent>();
}

NGenius::~NGenius()
{
	DestroySystems();
}

void NGenius::Create()
{
	CreateStatesMachine();
	//TODO esto hay que revisar
	if (mGameScene == nullptr)
	{
		mGameScene = DBG_NEW GameScene("mainscene", this, mRenderSystem);
	}
}

void NGenius::Init(bool isFullscreen)
{
	mRenderSystem->Init(mApplicationName, isFullscreen);
	mInputHandler->Init(mRenderSystem->GetGLWindow());
}

void NGenius::Start(bool isReload)
{
	mEnvironmentSystem->Start();
	mRenderSystem->SetEnvironmentSystem(mEnvironmentSystem);
	mRenderSystem->Start();
	mSpacePartitionSystem->Start();
	mDebugSystem->Start();

	AddListenersToGameScene();
	mGameScene->Start();	
}

void NGenius::ShutDown()
{
	mStatesMachine.reset();

	DestroySystems();
}

void NGenius::Run()
{
	double lastCurrentTime = glfwGetTime();
	int frames = 0;
	float accumulatedTime = 0.0f;
	float elapsedTime = 0.0f;
	float lag = 0.0f;
	float frameTime = 0.016666f;

	do
	{
		double currentTime = glfwGetTime();
		elapsedTime = static_cast<float>(currentTime - lastCurrentTime);
		lastCurrentTime = currentTime;
		frames++;

		accumulatedTime += elapsedTime;
		lag += elapsedTime;

		mInputHandler->Update(elapsedTime);

		AcceptGuiTool();

		UpdateStatesMachine(elapsedTime);

		NGeniusState currentState = mStatesMachine->GetCurrentState()->GetID();
		if (currentState >= NGeniusState::STATE_RUN)
		{
			UpdateSystems(elapsedTime);
			if (mUpdateHandler != nullptr)
			{
				mUpdateHandler(elapsedTime);
			}
			lag -= frameTime;
		}

		if (currentState >= NGeniusState::STATE_RUN)
		{
			Render();
		}

		if (accumulatedTime > 1.0f)
		{
			mNumberFPS = frames / accumulatedTime;
			//std::cout << "fps: " << mNumberFPS << "\n";
			accumulatedTime = 0.0f;
			frames = 0;
		}

		AcceptStatistics();

	} // Check if the ESC key was pressed or the window was closed
	while (	glfwGetKey(mRenderSystem->GetGLWindow(), GLFW_KEY_ESCAPE) != GLFW_PRESS && 
			glfwWindowShouldClose(mRenderSystem->GetGLWindow()) == 0);

	mStatesMachine->GetContext()->GoToState(NGeniusState::STATE_SHUT_DOWN);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

void NGenius::Render()
{
	//render all entities
	if (mIsSpacePartitionEnabled)
	{
		ICamera* camera = mRenderSystem->GetCamera("gameplay_camera");
		if (camera != nullptr)
		{
			mSpacePartitionSystem->MarkGameEntitiesInsideCameraAsVisible(camera);
		}
	}

	mGameScene->Render(mRenderSystem);
	mRenderSystem->Render();
}

void NGenius::SaveToFile()
{
	core::utils::XMLSerializer xmlSerializer;
	WriteTo(&xmlSerializer);

	xmlSerializer.Save(std::string("ngenius.xml"));

	mGameScene->SaveToFile();
}
void NGenius::SetFilename(const std::string& filename)
{
	mFilename = filename;
}

void NGenius::LoadFromFile(const std::string& filename)
{
	core::utils::XMLDeserializer xmlDeserializer;
	xmlDeserializer.Load(mFilename);
	ReadFrom(&xmlDeserializer);
}


void NGenius::ReadFrom(core::utils::IDeserializer* source)
{
	source->BeginAttribute("ngenius");
		mRenderSystem->ReadFrom(source);
		source->BeginAttribute("engine");
		ReadDebugParameters(source);
		source->EndAttribute();
		mGameScene->ReadFrom(source);		
	source->EndAttribute();
}

void NGenius::ReadDebugParameters(core::utils::IDeserializer* source)
{
	source->BeginAttribute("debug");
	bool isEnabled = false;
	source->ReadParameter("is_enabled", &isEnabled);
	source->EndAttribute();

	SetDebugModeEnabled(isEnabled);
}


void NGenius::WriteTo(core::utils::ISerializer* destination)
{
	destination->BeginAttribute("Configuration");
		destination->WriteParameter(std::string("version"), std::string("0.0.1"));
		destination->WriteParameter(std::string("position"), glm::vec3(1.0f, 3.0f, 3.4f));
	destination->EndAttribute();
}

void NGenius::AcceptGuiTool()
{
	if (mDebugSystem->IsDebugModeEnabled())
	{
		mEnvironmentSystem->Accept(*mRenderSystem->GetGuiTool());
	}
}

void NGenius::AcceptStatistics()
{
	if (mDebugSystem->IsDebugModeEnabled())
	{
		mAnimationSystem->Accept(*mStatistics);
		mRenderSystem->Accept(*mStatistics);
		mGameScene->Accept(*mStatistics);
		mPhysicsSystem->Accept(*mStatistics);
		mSpacePartitionSystem->Accept(*mStatistics);
		this->Accept(*mStatistics);
	}
}

void NGenius::UpdateSystems(float elapsedTime)
{
	mInputSystem->Update(elapsedTime);
	mDebugSystem->Update(elapsedTime);
	mEnvironmentSystem->Update(elapsedTime);
	mParticlesSystem->Update(elapsedTime);
	mPhysicsSystem->Update(elapsedTime);		
	mAnimationSystem->Update(elapsedTime);
	mSpacePartitionSystem->Update(elapsedTime);
	mGameScene->Update(elapsedTime);
	mRenderSystem->Update(elapsedTime);
	UpdateStatesMachine(elapsedTime);
}

void NGenius::UpdateStatesMachine(float elapsedTime)
{
	mStatesMachine->Update(elapsedTime);
}

void NGenius::CreateSystems(float screenWidth, float screenHeight)
{
	mStatistics = DBG_NEW  Statistics();
	mInputHandler = DBG_NEW  InputHandler();
	mRenderSystem = DBG_NEW  RenderSystem(screenWidth, screenHeight);
	mPhysicsSystem = DBG_NEW  PhysicsSystem();
	mInputSystem = DBG_NEW  InputSystem(mInputHandler);
	mDebugSystem = DBG_NEW  DebugSystem(this, mRenderSystem, mInputHandler);
	mParticlesSystem = DBG_NEW  ParticlesSystem();
	mSpacePartitionSystem = DBG_NEW  SpacePartitionSystem();
	mEnvironmentSystem = DBG_NEW  EnvironmentSystem();
	mAnimationSystem = DBG_NEW  AnimationSystem();
}

void NGenius::CreateStatesMachine()
{
	mFSMContext = std::make_shared<FSMContext>(shared_from_this());
	mStatesMachine = std::make_unique<core::utils::FSM::StatesMachine<NGeniusState, FSMContext>>(mFSMContext);

	auto runState = std::make_shared<RunState>();
	auto freeCameraState = std::make_shared<FreeModeOnlyCameraState>();
	auto freePlayerState = std::make_shared<FreeModeOnlyPlayerState>();
	auto initialState = std::make_shared<InitialState>();
	auto startState = std::make_shared<StartState>();
	auto loadingState = std::make_shared<LoadingState>();
	auto shutDownState = std::make_shared<ShutDownState>();

	mStatesMachine->AddState(initialState);
	mStatesMachine->AddState(startState);
	mStatesMachine->AddState(loadingState);
	mStatesMachine->AddState(runState);
	mStatesMachine->AddState(freeCameraState);
	mStatesMachine->AddState(freePlayerState);
	mStatesMachine->AddState(shutDownState);

	mStatesMachine->AddTransition(std::make_unique<EnterLoadingTransition>(initialState, loadingState));
	mStatesMachine->AddTransition(std::make_unique<EnterStartTransition>(loadingState, startState));
	mStatesMachine->AddTransition(std::make_unique<EnterRunTransition>(startState, runState));

	mStatesMachine->AddFromAnyTransition(std::make_unique<EnterShutDownTransition>(shutDownState));

	mStatesMachine->AddTransition(std::make_unique<EnterExitFreeModeOnlyCameraTransition>(freeCameraState, runState));
	mStatesMachine->AddTransition(std::make_unique<EnterExitFreeModeOnlyPlayerTransition>(freeCameraState, freePlayerState));

	mStatesMachine->AddTransition(std::make_unique<EnterExitFreeModeOnlyCameraTransition>(runState, freeCameraState));
	mStatesMachine->AddTransition(std::make_unique<EnterExitFreeModeOnlyPlayerTransition>(runState, freePlayerState));

	mStatesMachine->AddTransition(std::make_unique<EnterExitFreeModeOnlyCameraTransition>(freePlayerState, freeCameraState));
	mStatesMachine->AddTransition(std::make_unique<EnterExitFreeModeOnlyPlayerTransition>(freePlayerState, runState));

	mStatesMachine->SetInitialState(initialState->GetID());
}

void NGenius::DestroySystems()
{
	delete mGameScene;

	if (mAnimationSystem != nullptr)
	{
		delete mAnimationSystem;
	}
	if (mEnvironmentSystem != nullptr)
	{
		delete mEnvironmentSystem;
	}
	if (mSpacePartitionSystem != nullptr)
	{
		delete mSpacePartitionSystem;
	}
	if (mDebugSystem != nullptr)
	{
		delete mDebugSystem;
	}
	if (mInputSystem != nullptr)
	{
		delete mInputSystem;
	}
	if (mLightsSystem != nullptr)
	{
		delete mLightsSystem;
	}
	if (mParticlesSystem != nullptr)
	{
		delete mParticlesSystem;
	}
	if (mPhysicsSystem != nullptr)
	{
		delete mPhysicsSystem;
	}
	if (mRenderSystem != nullptr)
	{
		delete mRenderSystem;
	}
	if (mInputHandler != nullptr)
	{
		delete mInputHandler;
	}
	if (mStatistics != nullptr)
	{
		delete mStatistics;
	}
}

void NGenius::RegisterAllEventsInputListener(IInputListener* listener)
{
	mInputHandler->RegisterAllEventsInputListener(listener);
}

void NGenius::UnRegisterInputListener(IInputListener* listener)
{
	mInputHandler->UnRegisterInputListener(listener);
}

void NGenius::RegisterInputHandler(std::function<void(GLFWwindow* window)> callback)
{
	mInputHandler->RegisterInputHandler(callback);
}

void NGenius::RegisterUpdateHandler(std::function<void(float elapsedTime)> callback)
{
	mUpdateHandler = callback;
}

IShaderProgram* NGenius::GetShader(const std::string& name) const
{
	assert(mRenderSystem != nullptr);
	return mRenderSystem->GetShader(name);
}

FontType* NGenius::GetFont(const std::string& name) const
{
	assert(mRenderSystem != nullptr);
	return mRenderSystem->GetFont(name);
}

Model* NGenius::GetModel(const std::string& name) const
{
	assert(mRenderSystem != nullptr);
	return mRenderSystem->GetModel(name);
}

ITexture* NGenius::GetTexture(const std::string& name) const
{
	assert(mRenderSystem != nullptr);
	return mRenderSystem->GetTexture(name);
}

IMaterial* NGenius::GetMaterial(const std::string& name) const
{
	assert(mRenderSystem != nullptr);
	return mRenderSystem->GetMaterial(name);
}

Animation* NGenius::GetAnimation(const std::string& name) const
{
	assert(mRenderSystem != nullptr);
	return mRenderSystem->GetAnimation(name);
}

ICamera* NGenius::GetCamera(const std::string& name) const
{
	assert(mRenderSystem != nullptr);
	return mRenderSystem->GetCamera(name);
}

ICamera* NGenius::GetGameplayCamera() const
{
	assert(mRenderSystem != nullptr);
	return mRenderSystem->GetGameplayCamera();
}

ICamera* NGenius::GetFreeCamera() const
{
	assert(mRenderSystem != nullptr);
	return mRenderSystem->GetFreeCamera();
}

IGameEntity* NGenius::GetGameEntity(const std::string& name) const
{
	if (mGameScene != nullptr)
	{
		return mGameScene->GetGameEntity(name);
	}
	else
	{
		return nullptr;
	}
}

GameScene* NGenius::GetGameScene(const std::string& name) const
{
	return mGameScene;
}

GLFWwindow* NGenius::GetGLWindow() const
{
	assert(mRenderSystem != nullptr);
	return mRenderSystem->GetGLWindow();
}

float NGenius::GetNumberFPS() const
{
	return mNumberFPS;
}

const Statistics* NGenius::GetStatistics() const
{
	return mStatistics;
}

float NGenius::GetScreenWidth() const
{
	assert(mRenderSystem != nullptr);
	return mRenderSystem->GetScreenWidth();
}

float NGenius::GetScreenHeight() const
{
	assert(mRenderSystem != nullptr);
	return mRenderSystem->GetScreenHeight();
}

float NGenius::GetDayTime() const
{
	return mEnvironmentSystem->GetDayTime();
}

void NGenius::SetFullScreen(bool isFullScreen)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->SetFullScreen(isFullScreen);
}

void NGenius::AddListenersToGameScene()
{
	mGameScene->RegisterGameSceneListener(mDebugSystem);
	mGameScene->RegisterGameSceneListener(mInputSystem);
	mGameScene->RegisterGameSceneListener(mPhysicsSystem);
	mGameScene->RegisterGameSceneListener(mSpacePartitionSystem);
	mGameScene->RegisterGameSceneListener(mEnvironmentSystem);
	mGameScene->RegisterGameSceneListener(mAnimationSystem);
}

GameScene* NGenius::CreateGameScene(const std::string& name)
{
	//TODO ojo que esto es horrible, tener que crear el lightsystem cuando la escena se crea...no sé
	mLightsSystem = DBG_NEW  LightsSystem(mGameScene);
	AddListenersToGameScene();	

	return mGameScene;
}

void NGenius::AddParticleEmitter(ParticlesEmitter* emitter)
{
	assert(mParticlesSystem != nullptr);

	emitter->SetGameScene(mGameScene);
	mParticlesSystem->AddParticleEmitter(emitter);
}

void NGenius::AddRenderPass(RenderPass* renderPass, bool addAfterPostProcessing)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->AddOrReplaceRenderPass(renderPass, addAfterPostProcessing);
}

void NGenius::AddLight(Light* light)
{
	mLightsSystem->AddLight(light);
}

void NGenius::AddCamera(ICamera* camera)
{
	mRenderSystem->AddCamera(camera);
}

void NGenius::AddEntity(IGameEntity* entity)
{
	assert(entity != nullptr);
	assert(mGameScene != nullptr);
	mGameScene->AddEntity(entity);
}

void NGenius::SetTerrain(const Terrain* terrain)
{
	assert(mPhysicsSystem != nullptr);
	mPhysicsSystem->SetTerrain(terrain);
}

void NGenius::SetEnergyWall(const glm::vec3& position, float radius)
{
	assert(mPhysicsSystem != nullptr);
	mPhysicsSystem->SetEnergyWall(position, radius);
}

const ITexture* NGenius::CreateDepthTexture(const std::string& name, const glm::ivec2& size)
{
	assert(mRenderSystem != nullptr);
	return mRenderSystem->CreateDepthTexture(name, size);
}

IMaterial* NGenius::CreateMaterial(const std::string& name, IShaderProgram* shader)
{
	assert(mRenderSystem != nullptr);
	return mRenderSystem->CreateMaterial(name, shader);
}

void NGenius::SetCastingShadowsParameters(const glm::vec3& lightDirection, int pfcCounter)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->SetCastingShadowsParameters(lightDirection, pfcCounter);
}

void NGenius::SetCastingShadowsEnabled(bool enabled)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->SetCastingShadowsEnabled(enabled);
}

void NGenius::SetFogEnabled(bool enabled)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->SetFogEnabled(enabled);
}

void NGenius::SetWaterEnabled(bool enabled)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->SetWaterEnabled(enabled);
}

void NGenius::SetWaterParameters(const ICamera* camera, float waterY)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->SetWaterParameters(camera, waterY);
}

float NGenius::GetWaterHeight() const
{
	return mRenderSystem->GetWaterHeight();
}

void NGenius::SetCastingShadowsTarget(const IGameEntity* target)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->SetCastingShadowsTarget(target);
}

void NGenius::SetDebugModeEnabled(bool enabled)
{
	mDebugSystem->SetDebugModeEnabled(enabled);
}

BaseVisitable<>::ReturnType NGenius::Accept(BaseVisitor& guest)
{
	return AcceptImpl(*this, guest);
}

void NGenius::Reload()
{
	LoadFromFile(mFilename);
	mPhysicsSystem->Reload();
	mSpacePartitionSystem->Reload();
	mInputSystem->Reload();
	mEnvironmentSystem->Reload();
	mAnimationSystem->Reload();
	Start(true);
	mStatesMachine->Reload();
	mStatesMachine->ForceState(mStatesMachine->GetCurrentState()->GetID(), 0.0f);
}

void NGenius::Query(const AABB& aabb, std::vector<IGameEntity*>& result)
{
	mSpacePartitionSystem->Query(aabb, result);
}

void NGenius::SetIsSpacePartitionEnabled(bool enable)
{
	mIsSpacePartitionEnabled = enable;
	mSpacePartitionSystem->SetSpacePartitionComponentsEnabled(enable);
}

void NGenius::ChangeToCamera(const std::string& renderPassName, const ICamera* camera)
{
	mRenderSystem->ChangeToCamera(renderPassName, camera);
}

void NGenius::ChangeToCamera(const std::string& cameraName, const std::string& newCameraName)
{
	mRenderSystem->ChangeToCamera(cameraName, newCameraName);
}
