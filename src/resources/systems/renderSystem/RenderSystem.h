#pragma once
#include <vector>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include "../../../VertexBuffersManager.h"

class IRenderer;
class ICamera;
class RenderPass;
class Texture;
class ITexture;

class ShadersLibrary;
class TexturesLibrary;
class ModelsLibrary;
class FontsLibrary;

class Model;
class IShaderProgram;
class ITexture;
class FontType;

struct GLFWwindow;
struct GLFWmonitor;

class ShadowsSystem;

class RenderSystem
{
	VertexBuffersManager mVertexsBuffersManager;
	typedef std::vector<IRenderer*> RenderersList;

	std::map<char, RenderersList> mRenderersPerPass;
	std::vector<IRenderer*> mInstances;
	std::vector<const RenderPass*> mRenderPasses;
	float mScreenWidth;
	float mScreenHeight;
	ShadersLibrary* mShadersLibrary;
	TexturesLibrary* mTexturesLibrary;
	ModelsLibrary* mModelsLibrary;
	FontsLibrary* mFontsLibrary;

	GLFWwindow* mWindow;

	ShadowsSystem* mShadowsSystem;

	bool mIsFullScreen;
	int mLastClipPlaneNumberUsed;
	
public:
	RenderSystem(float screenWidth, float screenHeight);
	~RenderSystem();

	void Init(const std::string& applicationName, bool isFullscreen);
	void Render();
	void AddToRender(IRenderer* renderer);
	void AddRenderPass(const RenderPass* renderPass);

	float GetScreenWidth() const;
	float GetScreenHeight() const;
	GLFWwindow* GetGLWindow() const;
	IShaderProgram* GetShader(const std::string& name) const;
	Model* GetModel(const std::string& name) const;
	ITexture* GetTexture(const std::string& name) const;
	FontType* GetFont(const std::string& name) const;
		
	void SetCastingShadowsParameters(const glm::vec3& lightDirection, int pfcCounter);
	void SetCastingShadowsTarget(const glm::vec3& position);
	void SetCastingShadowsEnabled(bool enabled);
	void SetFullScreen(bool isFullScreen);
	
	const ITexture* CreateDepthTexture(const std::string& name, const glm::ivec2& size);

private:
	void CreateResourcesLibraries();
	void DestroyResourcesLibraries();

	void CreateShadowsSystem();

	bool InitializeWindowAndOpenGL(const std::string& applicationName, bool isFullscreen);
	void DisableVSync(bool enable);
	GLFWmonitor* GetCurrentMonitor(GLFWwindow *window);

	void LoadResources();

	void Render(const RenderPass* renderPass);
	void UpdateDistancesToCamera(const ICamera* camera, RenderersList* renderers);
	void RenderInstances(const RenderPass* renderPass, IRenderer* renderer, std::vector<IRenderer*>& instances);
	
	void CheckGLError();
};

