#pragma once

#include <string>
#include <functional>
#include <glm/glm.hpp>

class RenderSystem;
class PhysicsSystem;
class EntitiesSystem;
class ParticlesSystem;
class LightsSystem;

class Model;
class IShaderProgram;
class ITexture;
class Texture;
class FontType;
class IMaterial;
class Light;

class GameEntity;
class ParticlesEmitter;
class RenderPass;
class Terrain;
class ICamera;


struct GLFWwindow;

class NGenius
{
	RenderSystem* mRenderSystem;
	PhysicsSystem* mPhysicsSystem;
	EntitiesSystem* mEntitiesSystem;
	ParticlesSystem* mParticlesSystem;
	LightsSystem* mLightsSystem;

	std::string mApplicationName;
	float mFPS;

	std::function<void(GLFWwindow* window)> mInputHandler;
	std::function<void(float elapsedTime)> mUpdateHandler;

public:
	explicit NGenius(std::string applicationName, float screenWidth, float screenHeight);
	~NGenius();

	void Init(bool isFullscreen);
	void Update();

	IShaderProgram* GetShader(const std::string& name) const;
	Model* GetModel(const std::string& name) const;
	ITexture* GetTexture(const std::string& name) const;
	const ITexture* CreateDepthTexture(const std::string& name, const glm::ivec2& size);
	IMaterial* GetMaterial(const std::string& name) const;

	FontType* GetFont(const std::string& name) const;
	float GetFPS() const;
	float GetScreenWidth() const;
	float GetScreenHeight() const;
	GLFWwindow* GetGLWindow() const;

	void AddGameEntity(GameEntity* entity);
	void AddParticleEmitter(ParticlesEmitter* emitter);
	void AddRenderPass(RenderPass* renderPass);
	void AddLight(Light* light);

	IMaterial* CreateMaterial(const std::string& name, IShaderProgram* shader);

	void RegisterInputHandler(std::function<void(GLFWwindow* window)> callback);
	void RegisterUpdateHandler(std::function<void(float elapsedTime)> callback);

	void SetFullScreen(bool isFullScreen);
	void SetTerrain(const Terrain* terrain);
	void SetEnergyWall(const glm::vec3& position, float radius);

	//shadows
	void SetCastingShadowsParameters(const glm::vec3& lightDirection, int pfcCounter);
	void SetCastingShadowsTarget(const glm::vec3& position);
	void SetCastingShadowsEnabled(bool enabled);

private:
	void CreateSystems(float screenWidth, float screenHeight);
	void DestroySystems();
	
	void UpdateInput(float deltaTime);
	void UpdateSystems(float elapsedTime);
};

