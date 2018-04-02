#include "stdafx.h"
#include "NGenius.h"

#include <Windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "input/InputHandler.h"

#include "resources/systems/EntitiesSystem.h"
#include "resources/systems/renderSystem/RenderSystem.h"
#include "resources/systems/PhysicsSystem.h"
#include "resources/systems/ParticlesSystem.h"
#include "resources/systems/LightsSystem.h"
#include "resources/systems/InputSystem.h"
#include "resources/systems/DebugSystem.h"
#include "statistics/Statistics.h"

#include "resources/entities/ParticlesEmitter.h"

#include "resources/textures/Texture.h"

NGenius::NGenius(std::string applicationName, float screenWidth, float screenHeight) :
mRenderSystem(nullptr),
mPhysicsSystem(nullptr),
mEntitiesSystem(nullptr),
mParticlesSystem(nullptr),
mApplicationName(applicationName)
{
	CreateSystems(screenWidth, screenHeight);
}

NGenius::~NGenius()
{
	DestroySystems();
}

void NGenius::Init(bool isFullscreen)
{
	mRenderSystem->Init(mApplicationName, isFullscreen);
	mInputHandler->Init(mRenderSystem->GetGLWindow());
}

void NGenius::PostInit()
{
	mRenderSystem->PostInit();
}

void NGenius::Update()
{
	double lastCurrentTime = glfwGetTime();
	int frames = 0;
	float accumulatedTime = 0.0f;
	float elapsedTime = 0.0f;

	do
	{
		mInputHandler->Update(elapsedTime);

		UpdateSystems(elapsedTime);

		if (mUpdateHandler != nullptr)
		{
			mUpdateHandler(elapsedTime);
		}

		mRenderSystem->Render();

		double currentTime = glfwGetTime();
		elapsedTime = static_cast<float>(currentTime - lastCurrentTime);
		lastCurrentTime = currentTime;
		frames++;

		accumulatedTime += elapsedTime;
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

void NGenius::AcceptStatistics()
{
	if (mDebugSystem->IsDebugModeEnabled())
	{
		mRenderSystem->Accept(*mStatistics);
		mEntitiesSystem->Accept(*mStatistics);
		mPhysicsSystem->Accept(*mStatistics);
		this->Accept(*mStatistics);
	}
}

void NGenius::UpdateSystems(float elapsedTime)
{
	mInputSystem->Update(elapsedTime);
	mDebugSystem->Update(elapsedTime);
	mParticlesSystem->Update(elapsedTime);
	mEntitiesSystem->Update(elapsedTime);
	mPhysicsSystem->Update(elapsedTime);
}

void NGenius::CreateSystems(float screenWidth, float screenHeight)
{
	mStatistics = new Statistics();
	mInputHandler = new InputHandler();
	mRenderSystem = new RenderSystem(screenWidth, screenHeight);
	mPhysicsSystem = new PhysicsSystem();
	mInputSystem = new InputSystem(mInputHandler);
	mDebugSystem = new DebugSystem(mRenderSystem, mInputHandler);
	mEntitiesSystem = new EntitiesSystem(mRenderSystem, mPhysicsSystem, mInputSystem, mDebugSystem);
	mParticlesSystem = new ParticlesSystem();
	mLightsSystem = new LightsSystem(mEntitiesSystem);
}

void NGenius::DestroySystems()
{
	delete mDebugSystem;
	delete mInputSystem;
	delete mLightsSystem;
	delete mParticlesSystem;
	delete mEntitiesSystem;
	delete mPhysicsSystem;
	delete mRenderSystem;
	delete mInputHandler;
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

void NGenius::SetFullScreen(bool isFullScreen)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->SetFullScreen(isFullScreen);
}

void NGenius::AddGameEntity(GameEntity* entity)
{
	assert(mEntitiesSystem != nullptr);
	mEntitiesSystem->AddEntity(entity);
}

void NGenius::AddParticleEmitter(ParticlesEmitter* emitter)
{
	assert(mParticlesSystem != nullptr);
	assert(mEntitiesSystem != nullptr);

	emitter->SetEntitiesSystem(mEntitiesSystem);
	mParticlesSystem->AddParticleEmitter(emitter);
}

void NGenius::AddRenderPass(RenderPass* renderPass)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->AddRenderPass(renderPass);
}

void NGenius::AddLight(Light* light)
{
	mLightsSystem->AddLight(light);
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