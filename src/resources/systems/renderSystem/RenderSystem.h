#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include "../../../VertexBuffersManager.h"
#include "../../renderers/IRenderer.h"
#include "../../../visitor/BaseVisitable.h"
#include "../../../utils/serializer/IDeserializer.h"

class ICamera;
class RenderPass;
class Texture;
class ITexture;
class IGameEntity;

class ShadersLibrary;
class TexturesLibrary;
class ModelsLibrary;
class FontsLibrary;
class MaterialsLibrary;
class AnimationsLibrary;

class Model;
class IShaderProgram;
class ITexture;
class FontType;
class IMaterial;
class Animation;
class AnimatedModel;

struct GLFWwindow;
struct GLFWmonitor;

class ShadowsRenderPassSubSystem;
class WaterRenderPassSubSystem;
class PostProcessSubSystem;

class GuiTool;
class EnvironmentSystem;

class RenderSystem : public core::utils::ISerializable, public BaseVisitable<>
{
	typedef std::vector<IRenderer*> RenderersList;
	typedef std::vector<RenderPass*>::iterator RenderPassesIterator;
	typedef std::vector<RenderPass*>::const_iterator RenderPassesConstIterator;

	VertexBuffersManager mVertexsBuffersManager;

	std::unordered_map<char, RenderersList> mRenderersPerPass;
	std::vector<IRenderer*> mInstances;
	std::vector<RenderPass*> mRenderPasses;
	std::vector<RenderPass*> mRenderPassesAfterPostProcessing;
	
	typedef std::map<const std::string, ICamera*>::iterator CamerasListIterator;
	std::map<const std::string, ICamera*> mCamerasList;

	float mScreenWidth;
	float mScreenHeight;

	ShadersLibrary* mShadersLibrary;
	TexturesLibrary* mTexturesLibrary;
	ModelsLibrary* mModelsLibrary;
	FontsLibrary* mFontsLibrary;
	MaterialsLibrary* mMaterialsLibrary;
	AnimationsLibrary* mAnimationsLibrary;

	GLFWwindow* mWindow;

	EnvironmentSystem* mEnvironmentSystem;

	ShadowsRenderPassSubSystem* mShadowsRenderPass;
	WaterRenderPassSubSystem* mWaterRenderPass;

	PostProcessSubSystem* mPostProcessSubsystem;

	IMaterial* mCurrentMaterial;

	ITexture* mDiffuseTexture;
	ITexture* mNormalTexture;

	ICamera* mGameplayCamera;
	ICamera* mFreeCamera;

	GuiTool* mGuiTool;

	int mLastClipPlaneNumberUsed;
	bool mIsFullScreen;
	bool mIsClippingEnabled;
	bool mIsOverdrawEnabled;
	bool mIsPostprocessEnabled;
	bool mLastRendererHadCullingEnabled;
	bool mIsFogEnabled;
	bool mLastRendererHadBlendingEnabled;
	int mLastBlendSFactor;
	int mLastBlendDFactor;
	bool mIsLastRendererHadDepthBufferEnabled;
	int mLastDepthBufferMask;
	int mLastDepthBufferFunc;

	unsigned int mNumberTrianglesRendered;
	unsigned int mNumberDrawCalls;
	unsigned int mNumberRenderers;
	
public:
	
	RenderSystem(float screenWidth, float screenHeight);
	~RenderSystem();

	void Init(const std::string& applicationName, bool isFullscreen);
	//TODO this method will be removed once gameplay camera be created during the init.
	//init
	//cameras, 
	//subsystems pues waterpass necesita de la camera
	void Start();
	void Update(float elapsedTime);
	void Render();
	void AddToRender(IRenderer* renderer);
	
	void AddOrReplaceRenderPassFirst(RenderPass* renderPass, bool addAfterPostProcessing = false);
	void AddOrReplaceRenderPass(RenderPass* renderPass, bool addAfterPostProcessing = false);
	void RemoveRenderPass(RenderPass* renderPass);

	void AddCamera(ICamera* camera);
	void AddOrReplaceCamera(ICamera* camera);
	void RemoveCamera(const std::string& name);
	ICamera* GetCamera(const std::string& name);
	bool HasCamera(const std::string& key);
	ICamera* GetGameplayCamera() const { return mGameplayCamera; }
	ICamera* GetFreeCamera() const { return mFreeCamera; }

	float GetScreenWidth() const;
	float GetScreenHeight() const;

	unsigned int GetNumberTrianglesRendered() const;
	unsigned int GetNumberDrawCalls() const;
	unsigned int GetNumberRenderers() const;

	GLFWwindow* GetGLWindow() const;
	IShaderProgram* GetShader(const std::string& name) const;
	Model* GetModel(const std::string& name) const;
	ITexture* GetTexture(const std::string& name) const;
	FontType* GetFont(const std::string& name) const;
	IMaterial* GetMaterial(const std::string& name) const;
	
	//TODO esto igual se tendría que pasar fuera del rendersystem y meterlo en el animationssystem
	Animation* GetAnimation(const std::string& name) const;

	//esto no me gusta...
	VertexBuffersManager& GetVertexBufferManager();

	void SetCastingShadowsParameters(const glm::vec3& lightDirection, int pfcCounter);
	void SetCastingShadowsTarget(const IGameEntity* target);
	void SetCastingShadowsEnabled(bool enabled);

	void SetFogEnabled(bool enabled);

	void SetWaterEnabled(bool enabled);
	void SetWaterParameters(const ICamera* camera, float waterY);
	float GetWaterHeight() const;

	void SetFullScreen(bool isFullScreen);
	void SetOverdrawEnabled(bool isOverdrawEnabled);
	void SetPostProcessEnabled(bool isPostProcessEnabled);
	
	void SetEnvironmentSystem(EnvironmentSystem* environmentSystem);

	IMaterial* CreateMaterial(const std::string& name, IShaderProgram* shader);

	ITexture* CreateDepthTexture(const std::string& name, const glm::ivec2& size);
	ITexture* CreateColorTexture(const std::string& name, const glm::ivec2& size);

	GuiTool* GetGuiTool();

	void ChangeToCamera(const std::string& renderPassName, const ICamera* camera);
	void ChangeToCamera(const std::string& cameraName, const std::string& newCameraName);

	// Heredado vía ISerializable
	virtual void ReadFrom(core::utils::IDeserializer* source) override;
	virtual void WriteTo(core::utils::ISerializer* destination) override;

	//release
	void ReleaseRenderPasses();
	void ReleaseRenderers();

	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

private:
	void CreateResourcesLibraries();
	void DestroyResourcesLibraries();

	void CreateSubSystems();
	void DestroySubSystems();

	bool InitializeWindowAndOpenGL(const std::string& applicationName, bool isFullscreen);
	void EnableVSync(bool enable);
	GLFWmonitor* GetCurrentMonitor(float* screenWidth, float* screenHeight);

	void LoadDefaultResources();

	void BuildRenderPasses();

	bool AddToRender(IRenderer* renderer, std::vector<RenderPass*>& renderPasses);
	void AddRenderPass(RenderPass* renderPass, bool addAfterPostProcessing = false, bool insertFirst = false);

	void PushRenderPassFront(RenderPass* renderPass);
	void PushRenderPassBack(RenderPass* renderPass);

	bool ValidateRenderPassesLayerMasks(RenderPass* renderPass, std::vector<RenderPass*>& renderPasses) const;

	void Render(RenderPass* renderPass);
	void RenderPasses(std::vector<RenderPass*>& renderPasses);
	RenderPass* GetRenderPass(const std::string& renderPassName) const;

	void UpdateSubsystems();
	void UpdateCameras();
	void UpdateDistancesToCamera(const ICamera* camera, RenderersList* renderers);
	void RenderInstances(RenderPass* renderPass, IRenderer* renderer, std::vector<IRenderer*>& instances);
	
	void SelectMaterial(RenderPass* renderPass, IRenderer* renderer);
	void SelectTextures();
	void SelectClippingPlane(RenderPass* renderPass);
	void ApplyShadows(IRenderer* renderer);
	void ApplyFog(RenderPass* renderPass, IRenderer* renderer);
	void ApplyLights(IRenderer* renderer);
	void ApplyCulling(IRenderer* renderer);
	void ApplyBlending(IRenderer* renderer);
	void ApplyDepthBuffer(IRenderer* renderer);

	void ReadCamerasFrom(core::utils::IDeserializer* source);
	void ReadCameraFrom(core::utils::IDeserializer* source);
	void DestroyCameras();

	void ReadRenderPassesFrom(core::utils::IDeserializer* source);
	void ReadRenderPassFrom(core::utils::IDeserializer* source);
	void ReadRenderLayersFrom(core::utils::IDeserializer* source);
	
	void ReadFogParameters(core::utils::IDeserializer* source);

	void CheckGLError();
};

