#pragma once
#include <vector>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include "../../../VertexBuffersManager.h"
#include "../../renderers/IRenderer.h"

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

class ShadowsRenderPassSubSystem;
class WaterRenderPassSubSystem;

class RenderSystem
{
	typedef std::vector<IRenderer*> RenderersList;
	typedef std::vector<RenderPass*>::iterator RenderPassesIterator;

	VertexBuffersManager mVertexsBuffersManager;

	std::map<char, RenderersList> mRenderersPerPass;
	std::vector<IRenderer*> mInstances;
	std::vector<RenderPass*> mRenderPasses;
	
	float mScreenWidth;
	float mScreenHeight;

	ShadersLibrary* mShadersLibrary;
	TexturesLibrary* mTexturesLibrary;
	ModelsLibrary* mModelsLibrary;
	FontsLibrary* mFontsLibrary;
	MaterialsLibrary* mMaterialsLibrary;

	GLFWwindow* mWindow;

	ShadowsRenderPassSubSystem* mShadowsRenderPass;
	WaterRenderPassSubSystem* mWaterRenderPass;

	IMaterial* mCurrentMaterial;

	ITexture* mDiffuseTexture;
	ITexture* mNormalTexture;

	int mLastClipPlaneNumberUsed;
	bool mIsFullScreen;
	bool mIsClippingEnabled;
	bool mIsOverdrawEnabled;
	
public:
	static const char* OVERDRAW_MATERIAL_NAME;
	static const char* WIREFRAME_MATERIAL_NAME;

	RenderSystem(float screenWidth, float screenHeight);
	~RenderSystem();

	void Init(const std::string& applicationName, bool isFullscreen);
	//TODO this method will be removed once gameplay camera be created during the init.
	//init
	//cameras, 
	//subsystems pues waterpass necesita de la camera
	void InitSubsystems();
	void Render();
	void AddToRender(IRenderer* renderer);
	
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
	void SetCastingShadowsTarget(const GameEntity* target);
	void SetCastingShadowsEnabled(bool enabled);

	void SetWaterEnabled(bool enabled);
	void SetWaterParameters(const ICamera* camera, float waterY);

	void SetFullScreen(bool isFullScreen);
	void SetOverdrawEnabled(bool isOverdrawEnabled);
	
	IMaterial* CreateMaterial(const std::string& name, IShaderProgram* shader);

	ITexture* CreateDepthTexture(const std::string& name, const glm::ivec2& size);

private:
	void CreateResourcesLibraries();
	void DestroyResourcesLibraries();

	void CreateSubSystems();
	void DestroySubSystems();

	bool InitializeWindowAndOpenGL(const std::string& applicationName, bool isFullscreen);
	void DisableVSync(bool enable);
	GLFWmonitor* GetCurrentMonitor(float* screenWidth, float* screenHeight);

	void LoadResources();
	void CreateNewResources();

	void Render(RenderPass* renderPass);
	void UpdateSubsystems();
	void UpdateDistancesToCamera(const ICamera* camera, RenderersList* renderers);
	void RenderInstances(RenderPass* renderPass, IRenderer* renderer, std::vector<IRenderer*>& instances);
	
	void SelectMaterial(RenderPass* renderPass, IRenderer* renderer);
	void SelectTextures();
	void SelectClippingPlane(RenderPass* renderPass);
	void ApplyShadows(IRenderer* renderer);

	void CheckGLError();
};

