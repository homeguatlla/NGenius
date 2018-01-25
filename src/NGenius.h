#pragma once

#include <string>
#include <functional>

class RenderSystem;
class PhysicsSystem;
class EntitiesSystem;
class ParticlesSystem;

class ShadersLibrary;
class TexturesLibrary;
class ModelsLibrary;
class FontsLibrary;

class Model;
class IShaderProgram;
class ITexture;
class FontType;

class GameEntity;
class ParticlesEmitter;
class RenderPass;
class Terrain;
class ICamera;

struct GLFWwindow;
struct GLFWmonitor;

class NGenius
{
	RenderSystem* mRenderSystem;
	PhysicsSystem* mPhysicsSystem;
	EntitiesSystem* mEntitiesSystem;
	ParticlesSystem* mParticlesSystem;

	ShadersLibrary* mShadersLibrary;
	TexturesLibrary* mTexturesLibrary;
	ModelsLibrary* mModelsLibrary;
	FontsLibrary* mFontsLibrary;

	GLFWwindow* mWindow;
	float mScreenWidth;
	float mScreenHeight;
	std::string mApplicationName;
	bool mIsFullScreen;
	float mFPS;

	std::function<void(GLFWwindow* window)> mInputHandler;
	std::function<void(float elapsedTime)> mUpdateHandler;

public:
	explicit NGenius(std::string applicationName, float screenWidth, float screenHeight);
	~NGenius();

	void Init();
	void Update();

	IShaderProgram* GetShader(const std::string& name) const;
	Model* GetModel(const std::string& name) const;
	ITexture* GetTexture(const std::string& name) const;
	FontType* GetFont(const std::string& name) const;
	float GetFPS() const;
	float GetScreenWidth() const;
	float GetScreenHeight() const;

	GLFWwindow* GetGLWindow() const;

	void AddGameEntity(GameEntity* entity);
	void AddParticleEmitter(ParticlesEmitter* emitter);
	void AddRenderPass(RenderPass* renderPass);

	void RegisterInputHandler(std::function<void(GLFWwindow* window)> callback);
	void RegisterUpdateHandler(std::function<void(float elapsedTime)> callback);

	void SetFullScreen(bool isFullScreen);
	void SetTerrain(const Terrain* terrain);
	void SetEnergyWallRadius(float radius);
	void SetCameraCastingShadows(const ICamera* camera);
	
private:
	bool InitializeWindowAndOpenGL();
	void DisableVSync(bool enable);
	GLFWmonitor* GetCurrentMonitor(GLFWwindow *window);

	void CreateResourcesLibraries();
	void CreateSystems(); 
	
	void DestroySystems();
	void DestroyResourcesLibraries();

	void CheckGLError();

	void UpdateInput(float deltaTime);
	void UpdateSystems(float elapsedTime);
	void LoadResources();
	
};

