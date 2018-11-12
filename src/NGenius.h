#pragma once

#include <string>
#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include "visitor/BaseVisitable.h"
#include "AABB.h"

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


struct GLFWwindow;

class NGenius : public BaseVisitable<>
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
	std::string mApplicationName;
	float mNumberFPS;
	bool mIsSpacePartitionEnabled;
	
	std::function<void(float elapsedTime)> mUpdateHandler;
	std::function<void(NGenius& engine)> mStartHandler;

public:
	explicit NGenius(std::string applicationName, float screenWidth, float screenHeight);
	~NGenius();

	void Init(bool isFullscreen);
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
	float GetNumberFPS() const;
	const Statistics* GetStatistics() const;
	float GetScreenWidth() const;
	float GetScreenHeight() const;
	GLFWwindow* GetGLWindow() const;

	GameScene* CreateGameScene(const std::string& name);
	GameScene* GetGameScene(const std::string& name);

	void AddParticleEmitter(ParticlesEmitter* emitter);
	void AddRenderPass(RenderPass* renderPass, bool addAfterPostProcessing);
	void AddRenderPassAt(unsigned int index, RenderPass* renderPass, bool addAfterPostProcessing);
	void AddLight(Light* light);
	void AddCamera(ICamera* camera);

	IMaterial* CreateMaterial(const std::string& name, IShaderProgram* shader);

	void RegisterAllEventsInputListener(IInputListener* listener);
	void UnRegisterInputListener(IInputListener* listener);
	void RegisterInputHandler(std::function<void(GLFWwindow* window)> callback);
	void RegisterUpdateHandler(std::function<void(float elapsedTime)> callback);
	void RegisterStartHandler(std::function<void(NGenius& engine)> callback);

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

private:

	void CreateSystems(float screenWidth, float screenHeight);
	void DestroySystems();
	
	void UpdateSystems(float elapsedTime);
	void AcceptStatistics();
	void AcceptGuiTool();

	void Render();
};

