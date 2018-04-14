#pragma once
#include <vector>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include "../../../VertexBuffersManager.h"
#include "../../renderers/IRenderer.h"
#include "../../../visitor/BaseVisitable.h"

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
class PostProcessSubSystem;

class RenderSystem : public BaseVisitable<>
{
	typedef std::vector<IRenderer*> RenderersList;
	typedef std::vector<RenderPass*>::iterator RenderPassesIterator;

	VertexBuffersManager mVertexsBuffersManager;

	std::map<char, RenderersList> mRenderersPerPass;
	std::vector<IRenderer*> mInstances;
	std::vector<RenderPass*> mRenderPasses;
	std::vector<RenderPass*> mRenderPassesAfterPostProcessing;
	
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

	PostProcessSubSystem* mPostProcessSubsystem;

	IMaterial* mCurrentMaterial;

	ITexture* mDiffuseTexture;
	ITexture* mNormalTexture;

	int mLastClipPlaneNumberUsed;
	bool mIsFullScreen;
	bool mIsClippingEnabled;
	bool mIsOverdrawEnabled;
	bool mIsPostprocessEnabled;

	unsigned int mNumberTrianglesRendered;
	unsigned int mNumberDrawCalls;
	
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
	void Start();
	void Render();
	void AddToRender(IRenderer* renderer);
	
	void AddRenderPass(RenderPass* renderPass, bool addAfterPostProcessing = false);
	void RemoveRenderPass(RenderPass* renderPass);

	float GetScreenWidth() const;
	float GetScreenHeight() const;

	unsigned int GetNumberTrianglesRendered() const;
	unsigned int GetNumberDrawCalls() const;

	GLFWwindow* GetGLWindow() const;
	IShaderProgram* GetShader(const std::string& name) const;
	Model* GetModel(const std::string& name) const;
	ITexture* GetTexture(const std::string& name) const;
	FontType* GetFont(const std::string& name) const;
	IMaterial* GetMaterial(const std::string& name) const;

	//esto no me gusta...
	VertexBuffersManager& GetVertexBufferManager();

	void SetCastingShadowsParameters(const glm::vec3& lightDirection, int pfcCounter);
	void SetCastingShadowsTarget(const GameEntity* target);
	void SetCastingShadowsEnabled(bool enabled);

	void SetWaterEnabled(bool enabled);
	void SetWaterParameters(const ICamera* camera, float waterY);

	void SetFullScreen(bool isFullScreen);
	void SetOverdrawEnabled(bool isOverdrawEnabled);
	void SetPostProcessEnabled(bool isPostProcessEnabled);
	
	IMaterial* CreateMaterial(const std::string& name, IShaderProgram* shader);

	ITexture* CreateDepthTexture(const std::string& name, const glm::ivec2& size);
	ITexture* CreateColorTexture(const std::string& name, const glm::ivec2& size);

	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

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

	void AddToRender(IRenderer* renderer, std::vector<RenderPass*>& renderPasses);

	void Render(RenderPass* renderPass);
	void RenderPasses(std::vector<RenderPass*>& renderPasses);

	void UpdateSubsystems();
	void UpdateDistancesToCamera(const ICamera* camera, RenderersList* renderers);
	void RenderInstances(RenderPass* renderPass, IRenderer* renderer, std::vector<IRenderer*>& instances);
	
	void SelectMaterial(RenderPass* renderPass, IRenderer* renderer);
	void SelectTextures();
	void SelectClippingPlane(RenderPass* renderPass);
	void ApplyShadows(IRenderer* renderer);

	void CheckGLError();
};

