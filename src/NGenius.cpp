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
#include "resources/systems/StatisticsSystem.h"

#include "resources/materials/MaterialsLibrary.h"

#include "resources/scene/GameScene.h"
#include "resources/entities/ParticlesEmitter.h"
#include "resources/textures/Texture.h"

#include "statistics/Statistics.h"
#include "guiTool/GuiTool.h"


NGenius::NGenius(std::string applicationName, float screenWidth, float screenHeight) :
mRenderSystem(nullptr),
mPhysicsSystem(nullptr),
mEntitiesSystem(nullptr),
mParticlesSystem(nullptr),
mSpacePartitionSystem(nullptr),
mEnvironmentSystem(nullptr),
mAnimationSystem(nullptr),
mStatisticsSystem(nullptr),
mGameScene(nullptr),
mApplicationName(applicationName),
mIsSpacePartitionEnabled(true)
{
	CreateSystems(screenWidth, screenHeight);
}

NGenius::~NGenius()
{
	if (mGameScene != nullptr)
	{
		delete mGameScene;
	}
	DestroySystems();
}

void NGenius::Init(bool isFullscreen)
{
	mRenderSystem->Init(mApplicationName, isFullscreen);
	mInputHandler->Init(mRenderSystem->GetGLWindow());
}

void NGenius::Start()
{
	mEnvironmentSystem->Start();
	mRenderSystem->SetEnvironmentSystem(mEnvironmentSystem);
	mRenderSystem->Start();
	mSpacePartitionSystem->Start();
	mDebugSystem->Start();

	if (mStartHandler != nullptr)
	{
		mStartHandler(*this);
	}
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

		//while (lag >= frameTime)
		{
			UpdateSystems(elapsedTime);

			if (mUpdateHandler != nullptr)
			{
				mUpdateHandler(elapsedTime);
			}
			lag -= frameTime;
		}

		Render();

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

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

void NGenius::Render()
{
	//render all entities
	if (mIsSpacePartitionEnabled)
	{
		ICamera* camera = mRenderSystem->GetCamera("gameplay_camera");
		mSpacePartitionSystem->MarkGameEntitiesInsideCameraAsVisible(camera);
	}

	mGameScene->Render(mRenderSystem);
	mRenderSystem->Render();
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

		mStatisticsSystem->Update(*mStatistics);
	}
}

void NGenius::UpdateSystems(float elapsedTime)
{
	mInputSystem->Update(elapsedTime);
	mDebugSystem->Update(elapsedTime);
	mEnvironmentSystem->Update(elapsedTime);
	mParticlesSystem->Update(elapsedTime);
	mGameScene->Update(elapsedTime);
	mPhysicsSystem->Update(elapsedTime);
	mSpacePartitionSystem->Update(elapsedTime);
	mAnimationSystem->Update(elapsedTime);
	mRenderSystem->Update(elapsedTime);
}

void NGenius::CreateSystems(float screenWidth, float screenHeight)
{
	mStatistics = new Statistics();
	mInputHandler = new InputHandler();
	mRenderSystem = new RenderSystem(screenWidth, screenHeight);
	mPhysicsSystem = new PhysicsSystem();
	mInputSystem = new InputSystem(mInputHandler);
	mDebugSystem = new DebugSystem(mRenderSystem, mInputHandler);
	mParticlesSystem = new ParticlesSystem();
	mSpacePartitionSystem = new SpacePartitionSystem();
	mEnvironmentSystem = new EnvironmentSystem();
	mAnimationSystem = new AnimationSystem();
	mStatisticsSystem = new StatisticsSystem();
}

void NGenius::DestroySystems()
{
	delete mAnimationSystem;
	delete mEnvironmentSystem;
	delete mSpacePartitionSystem;
	delete mDebugSystem;
	delete mInputSystem;
	delete mLightsSystem;
	delete mParticlesSystem;
	delete mPhysicsSystem;
	delete mRenderSystem;
	delete mInputHandler;
	delete mStatisticsSystem;
	delete mStatistics;
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

void NGenius::RegisterStartHandler(std::function<void(NGenius& engine)> callback)
{
	mStartHandler = callback;
}

void NGenius::OnKey(int key, int action)
{
	mInputHandler->OnKey(key, action);
}

void NGenius::OnMouseScroll(int button, float scroll)
{
	mInputHandler->OnMouseScroll(button, scroll);
}

void NGenius::OnMouseButton(int button, int action, int mods)
{
	mInputHandler->OnMouseButton(button, action, mods);
}

void NGenius::OnMouseCursorPos(double x, double y)
{
	mInputHandler->OnMouseCursorPos(x, y);
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

GameScene* NGenius::GetGameScene(const std::string& name)
{
	return mGameScene;
}

GameScene* NGenius::CreateGameScene(const std::string& name)
{
	assert(mGameScene == nullptr);

	mGameScene = new GameScene(name);
	
	//TODO ojo que esto es horrible, tener que crear el lightsystem cuando la escena se crea...no sé
	//una posible solución sería que se generase un evento a listener, pero hay que tener en cuenta que te 
	//llegarán diversas notificaciones.
	//Seguramente, para el caso de las estadísticas lo mejor sería crear una escena a parte. Pero entonces
	//hay que gestionar múltiples escenas.
	//para las luces, pues de alguna manera habría que indicar a que escenas se le aplican las luces que podrían
	//ser más de una. Por ahora lo dejamos así pues no hay múltiples escenas
	mLightsSystem = new LightsSystem(mGameScene);
	Transformation transformation(glm::vec3(-GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f, 0.0f),
		glm::vec3(0.0f),
		glm::vec3(0.70f)
	);
	mStatisticsSystem->Start(mGameScene, &transformation, GetFont("OCR A Extended"), GetMaterial(MaterialsLibrary::TEXT_MATERIAL_NAME));

	
	mGameScene->RegisterGameSceneListener(mDebugSystem);
	mGameScene->RegisterGameSceneListener(mInputSystem);
	mGameScene->RegisterGameSceneListener(mPhysicsSystem);
	mGameScene->RegisterGameSceneListener(mSpacePartitionSystem);
	mGameScene->RegisterGameSceneListener(mEnvironmentSystem);
	mGameScene->RegisterGameSceneListener(mAnimationSystem);

	return mGameScene;
}

void NGenius::AddParticleEmitter(ParticlesEmitter* emitter)
{
	assert(mParticlesSystem != nullptr);
	assert(mGameScene != nullptr);

	emitter->SetGameScene(mGameScene);
	mParticlesSystem->AddParticleEmitter(emitter);
}

void NGenius::AddRenderPass(RenderPass* renderPass, bool addAfterPostProcessing)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->AddRenderPass(renderPass, addAfterPostProcessing);
}

void NGenius::AddRenderPassAt(unsigned int index, RenderPass* renderPass, bool addAfterPostProcessing)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->AddRenderPassAt(index, renderPass, addAfterPostProcessing);
}

void NGenius::AddLight(Light* light)
{
	mLightsSystem->AddLight(light);
}

void NGenius::AddCamera(ICamera* camera)
{
	mRenderSystem->AddCamera(camera);
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

void NGenius::SetGUIEnabled(bool enabled)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->SetGUIEnabled(enabled);
}

void NGenius::SetGameplayEnabled(bool enabled)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->SetGameplayEnabled(enabled);
}

void NGenius::SetParticlesEnabled(bool enabled)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->SetParticlesEnabled(enabled);
}

void NGenius::SetTransparentEnabled(bool enabled)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->SetTransparentEnabled(enabled);
}

void NGenius::SetCastingShadowsTarget(const GameEntity* target)
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

void NGenius::Query(const AABB& aabb, std::vector<GameEntity*>& result)
{
	mSpacePartitionSystem->Query(aabb, result);
}

void NGenius::SetIsSpacePartitionEnabled(bool enable)
{
	mIsSpacePartitionEnabled = enable;
	mSpacePartitionSystem->SetSpacePartitionComponentsEnabled(enable);
}