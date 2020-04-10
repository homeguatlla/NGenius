#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <glm/glm.hpp>
#include "visitor/BaseVisitable.h"
#include "AABB.h"
#include "fsm/FSMContext.h"
#include "utils/serializer/ISerializable.h"
#include "utils/fsm/StatesMachine.h"
#include "fsm/states/StateTypes.h"
#include "resources/scene/GameScene.h"

class RenderSystem;
class PhysicsSystem;
class EntitiesSystem;
class ParticlesSystem;
class LightsSystem;
class InputSystem;
class DebugSystem;
class SpacePartitionSystem;
class Statistics;
class EnvironmentSystem;
class AnimationSystem;

class InputHandler;
class IInputListener;

class Model;
class IShaderProgram;
class ITexture;
class Texture;
class FontType;
class IMaterial;
class Light;
class Animation;
class AnimatedModel;

class ParticlesEmitter;
class RenderPass;
class Terrain;
class ICamera;

class IGameEntity;


struct GLFWwindow;

class NGenius : public core::utils::ISerializable, BaseVisitable<>, public std::enable_shared_from_this<NGenius>
{
	RenderSystem* mRenderSystem;
	PhysicsSystem* mPhysicsSystem;
	EntitiesSystem* mEntitiesSystem;
	ParticlesSystem* mParticlesSystem;
	LightsSystem* mLightsSystem;
	InputSystem* mInputSystem;
	SpacePartitionSystem* mSpacePartitionSystem;
	DebugSystem* mDebugSystem;
	EnvironmentSystem* mEnvironmentSystem;
	AnimationSystem* mAnimationSystem;
	InputHandler* mInputHandler;
	Statistics* mStatistics;
	GameScene* mGameScene;

	std::unique_ptr<core::utils::FSM::StatesMachine<NGeniusState, FSMContext>> mStatesMachine;
	std::shared_ptr<FSMContext> mFSMContext;

	std::string mApplicationName;
	float mNumberFPS;
	bool mIsSpacePartitionEnabled;
	std::string mFilename;

	std::function<void(float elapsedTime)> mUpdateHandler;

public:
	explicit NGenius(std::string applicationName, float screenWidth, float screenHeight);
	~NGenius();

	void Create();

	void Init(bool isFullscreen);
	void Start(bool isReload = false);
	void ShutDown();
	void Run();

	IShaderProgram* GetShader(const std::string& name) const;
	Model* GetModel(const std::string& name) const;
	ITexture* GetTexture(const std::string& name) const;
	const ITexture* CreateDepthTexture(const std::string& name, const glm::ivec2& size);
	IMaterial* GetMaterial(const std::string& name) const;
	Animation* GetAnimation(const std::string& name) const;
	ICamera* GetCamera(const std::string& name) const;
	ICamera* GetGameplayCamera() const;
	ICamera* GetFreeCamera() const;
	IGameEntity* GetGameEntity(const std::string& name) const;
	GameScene* GetGameScene(const std::string& name) const;


	FontType* GetFont(const std::string& name) const;
	float GetNumberFPS() const;
	const Statistics* GetStatistics() const;
	float GetScreenWidth() const;
	float GetScreenHeight() const;
	GLFWwindow* GetGLWindow() const;

	GameScene* CreateGameScene(const std::string& name);
	void AddParticleEmitter(ParticlesEmitter* emitter);
	void AddRenderPass(RenderPass* renderPass, bool addAfterPostProcessing);
	void AddLight(Light* light);
	void AddCamera(ICamera* camera);
	void AddEntity(IGameEntity* entity);

	IMaterial* CreateMaterial(const std::string& name, IShaderProgram* shader);

	void RegisterAllEventsInputListener(IInputListener* listener);
	void UnRegisterInputListener(IInputListener* listener);
	void RegisterInputHandler(std::function<void(GLFWwindow* window)> callback);
	void RegisterUpdateHandler(std::function<void(float elapsedTime)> callback);

	void SetFullScreen(bool isFullScreen);
	void SetTerrain(const Terrain* terrain);
	void SetEnergyWall(const glm::vec3& position, float radius);

	//shadows
	void SetCastingShadowsParameters(const glm::vec3& lightDirection, int pfcCounter);
	void SetCastingShadowsTarget(const IGameEntity* target);
	void SetCastingShadowsEnabled(bool enabled);

	//fog
	void SetFogEnabled(bool enabled);

	//water
	void SetWaterEnabled(bool enabled);
	void SetWaterParameters(const ICamera* camera, float waterY);
	float GetWaterHeight() const;

	//debug
	void SetDebugModeEnabled(bool enabled);

	//statistics
	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

	//spatial partition
	void Query(const AABB& aabb, std::vector<IGameEntity*>& result);
	void SetIsSpacePartitionEnabled(bool enable);

	//cameras
	void ChangeToCamera(const std::string& renderPassName, const ICamera* camera);
	void ChangeToCamera(const std::string& cameraName, const std::string& newCameraName);

	//environment
	float GetDayTime() const;

	//reload
	void Reload();

	//serialize
	void SaveToFile();

	void SetFilename(const std::string& filename);

	//deserialize
	void LoadFromFile(const std::string& filename);
	
	//physics engine
	void EnablePhysicsEngine(bool enable);

private:
	
	void CreateSystems(float screenWidth, float screenHeight);
	void DestroySystems();
	
	void UpdateSystems(float elapsedTime);
	void UpdateStatesMachine(float elapsedTime);
	void AcceptStatistics();
	void AcceptGuiTool();

	void Render();

	void AddListenersToGameScene();

	void CreateStatesMachine();

	// Heredado vía ISerializable
	void ReadFrom(core::utils::IDeserializer* source) override;
	void WriteTo(core::utils::ISerializer* destination) override;

	void ReadDebugParameters(core::utils::IDeserializer* source);
};

