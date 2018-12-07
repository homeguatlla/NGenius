#include "stdafx.h"
#include "NGenius.h"

#include <Windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <algorithm>

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
#include "resources/materials/IMaterial.h"
#include "resources/materials/effects/MaterialEffectClippingPlane.h"
#include "resources/materials/effects/MaterialEffectDiffuseTexture.h"
#include "resources/materials/effects/MaterialEffectNormalTexture.h"
#include "resources/materials/effects/MaterialEffectDirectionalLightProperties.h"
#include "resources/materials/effects/MaterialEffectFogProperties.h"
#include "resources/materials/effects/MaterialEffectShadowProperties.h"
#include "resources/materials/effects/MaterialEffectParticle.h"

#include "resources/components/CollisionComponent.h"
#include "resources/components/PhysicsComponent.h"
#include "resources/components/DebugComponent.h"
#include "resources/components/SpacePartitionComponent.h"

#include "resources/renderers/IndicesRenderer.h"
#include "resources/renderers/WireframeRenderer.h"

#include "resources/scene/GameScene.h"
#include "resources/entities/ParticlesEmitter.h"
#include "resources/entities/ParticlesEmitterLibrary.h"
#include "resources/textures/Texture.h"

#include "resources/models/Model.h"

#include "statistics/Statistics.h"
#include "guiTool/GuiTool.h"

#include "resources/entities/Particle.h"



NGenius::NGenius() :
mRenderSystem(nullptr),
mPhysicsSystem(nullptr),
mEntitiesSystem(nullptr),
mParticlesSystem(nullptr),
mSpacePartitionSystem(nullptr),
mEnvironmentSystem(nullptr),
mAnimationSystem(nullptr),
mStatisticsSystem(nullptr),
mGameScene(nullptr),
mParticlesEmitterLibrary(nullptr),
mApplicationName(""),
mIsSpacePartitionEnabled(true)
{
	
}

NGenius::~NGenius()
{
	if (mGameScene != nullptr)
	{
		delete mGameScene;
	}
	DestroySystems();
}

void NGenius::Init(const std::string& applicationName, float screenWidth, float screenHeight, bool isFullscreen)
{
	mIsSpacePartitionEnabled = true;
	mApplicationName = applicationName;

	CreateSystems(screenWidth, screenHeight);
	
	mRenderSystem->Init(mApplicationName, isFullscreen);
	mInputHandler->Init(mRenderSystem->GetGLWindow());

	if (mInitHandler != nullptr)
	{
		mInitHandler();
	}
}

void NGenius::Start()
{
	mEnvironmentSystem->Start();
	mRenderSystem->SetEnvironmentSystem(mEnvironmentSystem);
	mRenderSystem->Start();
	mSpacePartitionSystem->Start();
	mDebugSystem->Start();

	mParticlesEmitterLibrary->Load(this);

	if (mStartHandler != nullptr)
	{
		mStartHandler();
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
			UpdatePreSystems(elapsedTime);

			if (mUpdateHandler != nullptr)
			{
				mUpdateHandler(elapsedTime);
			}
			lag -= frameTime;

			UpdatePostSystems(elapsedTime);
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

void NGenius::UpdatePreSystems(float elapsedTime)
{
	mInputSystem->Update(elapsedTime);
	mDebugSystem->Update(elapsedTime);
	mGameScene->Update(elapsedTime);
}

void NGenius::UpdatePostSystems(float elapsedTime)
{
	//Post systems will be updated after the Specific Game Update
	//the game will be updated and after that the space partition and render will be updated
	//that way, during the game update cycle we'll have previous spacepartition list available
	//to find game entities if the case
	//spacePartitionSystem always must be executed after gameScene because if not, 
	//won't have the aabb already calculated and entities won't be updated properly (removed, new entities)
	mSpacePartitionSystem->Update(elapsedTime);

	mEnvironmentSystem->Update(elapsedTime);
	mParticlesSystem->Update(elapsedTime);
	mPhysicsSystem->Update(elapsedTime);
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

	mParticlesEmitterLibrary = new ParticlesEmitterLibrary();
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

	delete mParticlesEmitterLibrary;
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

void NGenius::RegisterStartHandler(std::function<void()> callback)
{
	mStartHandler = callback;
}

void NGenius::RegisterInitHandler(std::function<void()> callback)
{
	mInitHandler = callback;
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

ParticlesEmitter* NGenius::GetParticlesEmitter(const std::string& name)
{
	assert(mParticlesEmitterLibrary != nullptr);
	return mParticlesEmitterLibrary->GetElement(name);
}

float NGenius::GetDayTime() const
{
	return mEnvironmentSystem->GetDayTime();
}

void NGenius::AddSunLightFrame(float hour, const float rotationAngle, const glm::vec3& color,
	const glm::vec3& fogColor, float fogDensity, float fogGradient, const std::string& cubemapName) 
{
	assert(mEnvironmentSystem != nullptr);
	mEnvironmentSystem->AddSunLightFrame(hour, rotationAngle, color, fogColor, fogDensity, fogGradient, cubemapName);
}

void NGenius::SetInitialEnvironmentTimer(float timer)
{
	mEnvironmentSystem->SetInitialTimer(timer);
}

void NGenius::SetGravity(const glm::vec3& gravity)
{
	assert(mPhysicsSystem != nullptr);
	mPhysicsSystem->SetGravity(gravity);
}

const glm::vec3& NGenius::GetGravity() const
{
	assert(mPhysicsSystem != nullptr);
	return mPhysicsSystem->GetGravity();
}

float NGenius::GetHeight(const glm::vec2& point) const
{
	assert(mPhysicsSystem != nullptr);
	return mPhysicsSystem->GetHeight(point);
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
	Transformation transformation(
		glm::vec3(10.0f, 20.0f, 0.0f),
		//glm::vec3(-GetScreenWidth() * 0.5f, GetScreenHeight() * 0.5f, 0.0f),
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

GameEntity* NGenius::CreateGameEntityFromModel(const std::string& modelName, Transformation* transformation, float introductionCoef, bool isInsideSpacePartition)
{
	Model* model = GetModel(modelName);
	if (model != nullptr)
	{
		IMaterial* material = GetMaterial(model->GetMaterialName());
		if (material == nullptr)
		{
			std::string shaderName = model->HasNormalmap() ? "normalmap" : "model";

			material = CreateMaterial(model->GetMaterialName(), GetShader(shaderName));
		}
		material->AddEffect(new MaterialEffectDiffuseTexture(
			static_cast<Texture*>(GetTexture(model->GetMaterialName() + "_diffuse")),
			glm::vec3(1.0f, 1.0f, 1.0f),
			1
		));
		material->AddEffect(new MaterialEffectDirectionalLightProperties());
		material->AddEffect(new MaterialEffectFogProperties());
		if (model->HasNormalmap())
		{
			material->AddEffect(new MaterialEffectNormalTexture(
				static_cast<Texture*>(GetTexture(model->GetMaterialName() + "_normalmap")),
				1
			));
		}
		material->AddEffect(new MaterialEffectShadowProperties(1));
		material->AddEffect(new MaterialEffectClippingPlane());


		IRenderer* renderer = new IndicesRenderer(model, material);

		GameEntity* modelEntity = new GameEntity(transformation, renderer);

		modelEntity->AddComponent(new PhysicsComponent(true, GetGravity(), introductionCoef));
		modelEntity->AddComponent(new CollisionComponent());
		if (isInsideSpacePartition)
		{
			modelEntity->AddComponent(new SpacePartitionComponent());
		}
		IRenderer* boundingBoxRenderer = new WireframeRenderer(GetModel("cube"), GetMaterial(MaterialsLibrary::WIREFRAME_MATERIAL_NAME));
		modelEntity->AddComponent(new DebugComponent(boundingBoxRenderer));

		return modelEntity;
	}

	return nullptr;
}

void NGenius::FillWithGameEntitiesVisibleInsideRadius(const glm::vec3& origin, float radius, std::vector<std::pair<GameEntity*, float>>& list, bool isSorted) const
{
	mSpacePartitionSystem->FillWithGameEntitiesVisibleInsideRadius(origin, radius, list, isSorted);
}

void NGenius::AddParticleEmitter(ParticlesEmitter* emitter)
{
	if (emitter != nullptr)
	{
		assert(mParticlesSystem != nullptr);
		assert(mGameScene != nullptr);

		emitter->SetGameScene(mGameScene);
		mParticlesSystem->AddParticleEmitter(emitter);
	}
}

void NGenius::AddRenderPass(RenderPass* renderPass, bool addAfterPostProcessing)
{
	if (renderPass != nullptr)
	{
		assert(mRenderSystem != nullptr);
		mRenderSystem->AddRenderPass(renderPass, addAfterPostProcessing);
	}
}

void NGenius::AddRenderPassAt(unsigned int index, RenderPass* renderPass, bool addAfterPostProcessing)
{
	if (renderPass != nullptr)
	{
		assert(mRenderSystem != nullptr);
		mRenderSystem->AddRenderPassAt(index, renderPass, addAfterPostProcessing);
	}
}

void NGenius::AddLight(Light* light)
{
	if (light != nullptr)
	{
		mLightsSystem->AddLight(light);
	}
}

void NGenius::AddCamera(ICamera* camera)
{
	if (camera != nullptr)
	{
		mRenderSystem->AddCamera(camera);
	}
}

void NGenius::AddSystem(ISystem* system)
{
	bool found = std::find_if(
		mSystems.begin(),
		mSystems.end(),
		[&](ISystem* sys) { return sys == system; }) != mSystems.end();

	if (!found)
	{
		mSystems.push_back(system);
	}
}

void NGenius::SetTerrain(const Terrain* terrain)
{
	if (terrain != nullptr)
	{
		assert(mPhysicsSystem != nullptr);
		mPhysicsSystem->SetTerrain(terrain);
	}
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

IMaterial* NGenius::CreateDiffuseGUIMaterial(const std::string& materialName, const std::string& textureName)
{
	IMaterial* material = NGenius::GetInstance().GetMaterial(materialName);
	if (material == nullptr)
	{
		material = NGenius::GetInstance().CreateMaterial(materialName, NGenius::GetInstance().GetShader("gui"));
		material->AddEffect(new MaterialEffectDiffuseTexture(NGenius::GetInstance().GetTexture(textureName), glm::vec3(1.0f), 1.0f));
	}

	return material;
}

Particle* NGenius::CreateParticle(Texture* texture, bool isAffectedByPhysics, bool canCollide)
{
	IMaterial* material = CreateMaterial("particle", GetShader("particle"));
	material->AddEffect(new MaterialEffectParticle(texture,
		GetTexture("depth_texture"),
		glm::vec2(GetScreenWidth(), GetScreenHeight()),
		1.0f)
	);
	material->AddEffect(new MaterialEffectClippingPlane());

	Particle* particle = new Particle(
		new Transformation(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.1f)),
		GetModel("particle_quad"),
		material,
		6.0f);
	if (isAffectedByPhysics)
	{
		PhysicsComponent* physicsComponent = new PhysicsComponent(false, mPhysicsSystem->GetGravity());
		particle->AddComponent(physicsComponent);
	}
	if (canCollide)
	{
		particle->AddComponent(new CollisionComponent());
	}

	return particle;
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