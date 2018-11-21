#pragma once

#include <string>
#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include "visitor/BaseVisitable.h"
#include "AABB.h"
#include "Singleton.h"

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
class StatisticsSystem;

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

class GameEntity;
class ParticlesEmitter;
class RenderPass;
class Terrain;
class ICamera;
class GameScene;
class Transformation;

class Particle;
class ParticlesEmitterLibrary;

struct GLFWwindow;


class NGenius : public BaseVisitable<>, public Singleton<NGenius>
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
	StatisticsSystem* mStatisticsSystem;
	InputHandler* mInputHandler;
	Statistics* mStatistics;
	GameScene* mGameScene;
	
	ParticlesEmitterLibrary* mParticlesEmitterLibrary;

	std::string mApplicationName;
	float mNumberFPS;
	bool mIsSpacePartitionEnabled;
	
	std::function<void(float elapsedTime)> mUpdateHandler;
	std::function<void()> mStartHandler;
	std::function<void()> mInitHandler;

public:
	NGenius();
	~NGenius();

	void Init(const std::string& applicationName, float screenWidth, float screenHeight, bool isFullscreen);
	void Start();
	void Run();

	IShaderProgram* GetShader(const std::string& name) const;
	Model* GetModel(const std::string& name) const;
	ITexture* GetTexture(const std::string& name) const;
	const ITexture* CreateDepthTexture(const std::string& name, const glm::ivec2& size);
	IMaterial* GetMaterial(const std::string& name) const;
	Animation* GetAnimation(const std::string& name) const;
	ICamera* GetCamera(const std::string& name) const;
	FontType* GetFont(const std::string& name) const;
	ParticlesEmitter* GetParticlesEmitter(const std::string& name);

	float GetNumberFPS() const;
	const Statistics* GetStatistics() const;
	float GetScreenWidth() const;
	float GetScreenHeight() const;
	GLFWwindow* GetGLWindow() const;

	GameScene* CreateGameScene(const std::string& name);
	GameScene* GetGameScene(const std::string& name);

	GameEntity* CreateGameEntityFromModel(const std::string& modelName, Transformation* transformation, float introductionCoef = 0.0f, bool isInsideSpacePartition = true);
	Particle* CreateParticle(Texture* texture, bool isAffectedByPhysics = false, bool canCollide = false);


	void AddParticleEmitter(ParticlesEmitter* emitter);
	void AddRenderPass(RenderPass* renderPass, bool addAfterPostProcessing);
	void AddRenderPassAt(unsigned int index, RenderPass* renderPass, bool addAfterPostProcessing);
	void AddLight(Light* light);
	void AddCamera(ICamera* camera);

	IMaterial* CreateMaterial(const std::string& name, IShaderProgram* shader);

	void FillWithGameEntitiesVisibleInsideRadius(const glm::vec3& origin, float radius, std::vector<std::pair<GameEntity*, float>>& list, bool isSorted = false) const;

	void RegisterAllEventsInputListener(IInputListener* listener);
	void UnRegisterInputListener(IInputListener* listener);
	void RegisterInputHandler(std::function<void(GLFWwindow* window)> callback);
	void RegisterUpdateHandler(std::function<void(float elapsedTime)> callback);
	void RegisterStartHandler(std::function<void()> callback);
	void RegisterInitHandler(std::function<void()> callback);

	void OnKey(int key, int action);
	void OnMouseScroll(int button, float scroll);
	void OnMouseButton(int button, int action, int mods);
	void OnMouseCursorPos(double x, double y);

	void SetFullScreen(bool isFullScreen);
	void SetTerrain(const Terrain* terrain);
	void SetEnergyWall(const glm::vec3& position, float radius);

	//shadows
	void SetCastingShadowsParameters(const glm::vec3& lightDirection, int pfcCounter);
	void SetCastingShadowsTarget(const GameEntity* target);
	void SetCastingShadowsEnabled(bool enabled);

	//fog
	void SetFogEnabled(bool enabled);

	//water
	void SetWaterEnabled(bool enabled);
	void SetWaterParameters(const ICamera* camera, float waterY);

	//GUI
	void SetGUIEnabled(bool enabled);

	//Gameplay
	void SetGameplayEnabled(bool enabled);

	//Particles
	void SetParticlesEnabled(bool enabled);

	//Transparent
	void SetTransparentEnabled(bool enabled);

	//debug
	void SetDebugModeEnabled(bool enabled);

	//statistics
	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

	//spatial partition
	void Query(const AABB& aabb, std::vector<GameEntity*>& result);
	void SetIsSpacePartitionEnabled(bool enable);

	//environment
	float GetDayTime() const;
	void AddSunLightFrame(float hour, const float rotationAngle, const glm::vec3& color,
		const glm::vec3& fogColor, float fogDensity, float fogGradient, const std::string& cubemapName);

	//physics
	void SetGravity(const glm::vec3& gravity);
	const glm::vec3& GetGravity() const;
	float GetHeight(const glm::vec2& point) const;

private:

	void CreateSystems(float screenWidth, float screenHeight);
	void DestroySystems();
	
	void UpdatePreSystems(float elapsedTime);
	void UpdatePostSystems(float elapsedTime);
	void AcceptStatistics();
	void AcceptGuiTool();

	void Render();
};

