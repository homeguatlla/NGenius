#pragma once
#include <vector>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include "../../../VertexBuffersManager.h"
#include "../../renderers/IRenderer_.h"

class ICamera;
class RenderPass;
class Texture;
class ITexture;

class ShadersLibrary;
class TexturesLibrary;
class ModelsLibrary;
class FontsLibrary;
class MaterialsLibrary;

class Model;
class IShaderProgram;
class ITexture;
class FontType;
class IMaterial;

struct GLFWwindow;
struct GLFWmonitor;

class ShadowsRenderPass;

class RenderSystem
{
	typedef std::vector<IRenderer_*> RenderersList;
	typedef std::vector<RenderPass*>::iterator RenderPassesIterator;

	VertexBuffersManager mVertexsBuffersManager;

	std::map<char, RenderersList> mRenderersPerPass;
	std::vector<IRenderer_*> mInstances;
	std::vector<RenderPass*> mRenderPasses;
	
	float mScreenWidth;
	float mScreenHeight;

	ShadersLibrary* mShadersLibrary;
	TexturesLibrary* mTexturesLibrary;
	ModelsLibrary* mModelsLibrary;
	FontsLibrary* mFontsLibrary;
	MaterialsLibrary* mMaterialsLibrary;

	GLFWwindow* mWindow;

	ShadowsRenderPass* mShadowsRenderPass;

	IMaterial* mCurrentMaterial;

	ITexture* mDiffuseTexture;
	ITexture* mNormalTexture;

	int mLastClipPlaneNumberUsed;
	bool mIsFullScreen;
	
public:
	RenderSystem(float screenWidth, float screenHeight);
	~RenderSystem();

	void Init(const std::string& applicationName, bool isFullscreen);
	void Render();
	void AddToRender(IRenderer_* renderer);
	
	void AddRenderPass(RenderPass* renderPass);
	void RemoveRenderPass(RenderPass* renderPass);

	float GetScreenWidth() const;
	float GetScreenHeight() const;
	GLFWwindow* GetGLWindow() const;
	IShaderProgram* GetShader(const std::string& name) const;
	Model* GetModel(const std::string& name) const;
	ITexture* GetTexture(const std::string& name) const;
	FontType* GetFont(const std::string& name) const;
	IMaterial* GetMaterial(const std::string& name) const;
		
	void SetCastingShadowsParameters(const glm::vec3& lightDirection, int pfcCounter);
	void SetCastingShadowsTarget(const glm::vec3& position);
	void SetCastingShadowsEnabled(bool enabled);
	void SetFullScreen(bool isFullScreen);
	
	IMaterial* CreateMaterial(const std::string& name, IShaderProgram* shader);

	const ITexture* CreateDepthTexture(const std::string& name, const glm::ivec2& size);

private:
	void CreateResourcesLibraries();
	void DestroyResourcesLibraries();

	void CreateRenderPasses();
	void DestroyRenderPasses();
	void CreateShadowsSystem();

	bool InitializeWindowAndOpenGL(const std::string& applicationName, bool isFullscreen);
	void DisableVSync(bool enable);
	GLFWmonitor* GetCurrentMonitor(GLFWwindow *window);

	void LoadResources();

	void Render(RenderPass* renderPass);
	void UpdateDistancesToCamera(const ICamera* camera, RenderersList* renderers);
	void RenderInstances(RenderPass* renderPass, IRenderer_* renderer, std::vector<IRenderer_*>& instances);
	
	void SelectMaterial(RenderPass* renderPass, IRenderer_* renderer);
	void SelectTextures();
	void SelectClippingPlane(RenderPass* renderPass);
	void ApplyShadows(IRenderer_* renderer);

	void CheckGLError();
};

