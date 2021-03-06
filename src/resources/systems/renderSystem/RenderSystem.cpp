#include "stdafx.h"
#include "RenderSystem.h"
#include "ShadowsRenderPassSubSystem.h"
#include "WaterRenderPassSubSystem.h"

#include "PostProcessSubSystem.h"

#include "src/resources/renderers/SkyBoxRenderer.h"
#include "src/resources/renderers/IndicesRenderer.h"
#include "src/resources/renderers/PointsRenderer.h"

#include "src/resources/IGameEntity.h"
#include "src/resources/camera/ICamera.h"
#include "src/resources/camera/PerspectiveCamera.h"
#include "src/resources/camera/OrthogonalCamera.h"

#include "src/resources/models/ModelsLibrary.h"
#include "src/resources/models/Model.h"
#include "src/resources/shaders/IShaderProgram.h"
#include "src/resources/shaders/ShadersLibrary.h"
#include "src/resources/font/FontsLibrary.h"
#include "src/resources/textures/TexturesLibrary.h"
#include "src/resources/textures/Texture.h"
#include "src/resources/textures/TextureArray.h"
#include "src/resources/textures/TextureCubemap.h"
#include "src/resources/materials/IMaterial.h"
#include "src/resources/materials/MaterialsLibrary.h"
#include "src/resources/models/animation/AnimationsLibrary.h"

#include "src/resources/materials/effects/MaterialEffectDiffuseTexture.h"
#include "src/resources/materials/effects/MaterialEffectNormalTexture.h"
#include "src/resources/materials/effects/MaterialEffectHeightMapTexture.h"
#include "src/resources/materials/effects/MaterialEffectClippingPlane.h"
#include "src/resources/materials/effects/MaterialEffectShadowProperties.h"
#include "src/resources/materials/effects/MaterialEffectWater.h"
#include "src/resources/materials/effects/MaterialEffectTextureArray.h"
#include "src/resources/materials/effects/MaterialEffectTextureCubemap.h"
#include "src/resources/materials/effects/MaterialEffectParticle.h"
#include "src/resources/materials/effects/MaterialEffectDepthTexture.h"
#include "src/resources/materials/effects/MaterialEffectFogProperties.h"
#include "src/resources/materials/effects/MaterialEffectDirectionalLightProperties.h"

#include "src/resources/systems/environmentSystem/EnvironmentSystem.h"
#include "src/resources/systems/environmentSystem/SunLight.h"

#include "RenderPass.h"

#include "src/BitNumber.h"
#include "src/guiTool/GuiTool.h"
#include "GameConstants.h"
#include "src/resources/InstantiableObject.h"
#include "src/utils/Log.h"

#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/norm.hpp>

using namespace std;

RenderSystem::RenderSystem(float screenWidth, float screenHeight) :
mScreenWidth(screenWidth),
mScreenHeight(screenHeight),
mShadersLibrary(nullptr),
mTexturesLibrary(nullptr),
mModelsLibrary(nullptr),
mFontsLibrary(nullptr),
mAnimationsLibrary(nullptr),
mWindow(nullptr),
mShadowsRenderPass(nullptr),
mWaterRenderPass(nullptr),
mPostProcessSubsystem(nullptr),
mEnvironmentSystem(nullptr),
mCurrentMaterial(nullptr),
mDiffuseTexture(nullptr),
mNormalTexture(nullptr),
mGuiTool(nullptr),
mLastClipPlaneNumberUsed(0),
mIsFullScreen(false),
mIsClippingEnabled(false),
mIsOverdrawEnabled(false),
mIsPostprocessEnabled(true),
mLastRendererHadCullingEnabled(true),
mIsFogEnabled(false),
mLastRendererHadBlendingEnabled(false),
mIsLastRendererHadDepthBufferEnabled(false),
mNumberTrianglesRendered(0),
mNumberDrawCalls(0),
mNumberRenderers(0)
{
	BitNumber bit;
	bit.Test();

	InstantiableObject::RegisterCameraType<PerspectiveCamera>();
	InstantiableObject::RegisterCameraType<OrthogonalCamera>();

	InstantiableObject::RegisterRendererType<SkyBoxRenderer>();
	InstantiableObject::RegisterRendererType<IndicesRenderer>();
	InstantiableObject::RegisterRendererType<PointsRenderer>();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glDisable(GL_CLIP_PLANE0);
}

RenderSystem::~RenderSystem()
{
	ReleaseRenderPasses();

	DestroyCameras();
	DestroyResourcesLibraries();
	DestroySubSystems();
	delete mGuiTool;
	glfwDestroyWindow(mWindow);
}

void RenderSystem::ReleaseRenderPasses()
{
	for (const RenderPass* pass : mRenderPasses)
	{
		delete pass;
	}

	for (const RenderPass* pass : mRenderPassesAfterPostProcessing)
	{
		delete pass;
	}
	mRenderPasses.clear();
	mRenderPassesAfterPostProcessing.clear();
}

void RenderSystem::ReleaseRenderers()
{
	mRenderersPerPass.clear();
}

//Initializing the engine basic stuff. The subsystems which need information apart will be initialized later
void RenderSystem::Init(const std::string& applicationName, bool isFullscreen)
{
	mIsFullScreen = isFullscreen;
	bool initialized = InitializeWindowAndOpenGL(applicationName, mIsFullScreen);
	assert(initialized);

	CheckGLError();
	mGuiTool = DBG_NEW GuiTool(mWindow);

	CreateResourcesLibraries();
	LoadDefaultResources();
	CreateSubSystems();
}

void RenderSystem::Start()
{
	ReleaseRenderers();

	UpdateCameras();
	mGuiTool->Initialize();
	mShadowsRenderPass->Init();
	mWaterRenderPass->Init();
	mPostProcessSubsystem->Init();

	mTexturesLibrary->LoadTexturesPendingToLoad();
	mMaterialsLibrary->Build();
	
	BuildRenderPasses();
}

void RenderSystem::BuildRenderPasses()
{
	for (RenderPass* pass : mRenderPasses)
	{
		pass->Build(this);
	}
	for (RenderPass* pass : mRenderPassesAfterPostProcessing)
	{
		pass->Build(this);
	}
}

void RenderSystem::CreateSubSystems()
{
	mShadowsRenderPass = DBG_NEW ShadowsRenderPassSubSystem(this, GetScreenWidth(), GetScreenHeight());
	mWaterRenderPass = DBG_NEW WaterRenderPassSubSystem(this, GetScreenWidth(), GetScreenHeight());

	mPostProcessSubsystem = DBG_NEW PostProcessSubSystem(this);
}

void RenderSystem::DestroySubSystems()
{
	delete mPostProcessSubsystem;
	delete mWaterRenderPass;
	delete mShadowsRenderPass;
}

void RenderSystem::LoadDefaultResources()
{
	mShadersLibrary->Load();
	mModelsLibrary->Load();
	mFontsLibrary->Load();
	mTexturesLibrary->Load();
	mMaterialsLibrary->Load();
}

void RenderSystem::Update(float elapsedTime)
{
	mNumberRenderers = 0;
	UpdateSubsystems();
}

void RenderSystem::UpdateSubsystems()
{
	//UpdateCameras();
	mShadowsRenderPass->Update();
	mWaterRenderPass->Update();
	//UpdateCameras();
}

void RenderSystem::UpdateCameras()
{
	for (CamerasListIterator it = mCamerasList.begin(); it != mCamerasList.end(); ++it)
	{
		it->second->Update();
	}
}

void RenderSystem::Render()
{
	ApplyDepthTest(true, true, GL_LESS);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	mNumberDrawCalls = 0;
	mNumberTrianglesRendered = 0;
	
	RenderPasses(mRenderPasses);

	if (mIsPostprocessEnabled)
	{
		mPostProcessSubsystem->Render();
	}

	RenderPasses(mRenderPassesAfterPostProcessing);
	
	mGuiTool->Render();
	
	// Swap buffers
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}

void RenderSystem::RenderPasses(std::vector<RenderPass*>& renderPasses)
{
	for (RenderPass* pass : renderPasses)
	{
		if (pass->IsEnabled())
		{
			if (pass->HasFrameBufferOutput())
			{
				pass->BindOutput(this);
				Render(pass);
				pass->UnbindOutput();
				pass->CopyDepthBuffer();
			}
			else
			{
				Render(pass);
			}
		}
	}
}

RenderPass* RenderSystem::GetRenderPass(const std::string& renderPassName) const
{
	RenderPassesConstIterator it = std::find_if(
		mRenderPasses.begin(), 
		mRenderPasses.end(), 
		[&] (RenderPass* renderPass) { return renderPass->GetName() == renderPassName; });

	if (it != mRenderPasses.end())
	{
		return *it;
	}
	else
	{
		return nullptr;
	}
}

void RenderSystem::Render(RenderPass* renderPass)
{
	RenderersList renderers = mRenderersPerPass[renderPass->GetLayersMask()];

	//std::cout << "render pass " << renderPass->GetName() << "\n";

	if (renderers.size() > 0)
	{
		//STEP 1 CALUCLATE DISTANCE TO CAMERA FOR EACH RENDERER
		if (renderPass->HasToCalculateDistanceToCamera() && renderPass->GetCamera() != nullptr)
		{
			UpdateDistancesToCamera(renderPass->GetCamera(), &renderers);
		}

		//STEP 2 ORDER RENDERERS FOLLOWING THE BITRENDERERINFORMATION
		//double time = glfwGetTime();
		sort(renderers.begin(), renderers.end(), [](IRenderer* a, IRenderer* b)
		{
			assert(a != nullptr && b != nullptr);
			return a->GetBitRendererInformation().GetValue() > b->GetBitRendererInformation().GetValue();
		});

		//cout << "sort time: " << (glfwGetTime() - time) * 1000.0f << "\n";

		//STEP 3 JOIN ALL RENDERERS WITH THE SAME BITRENDERINFORMATION TO USE INSTANCING
		unsigned int i = 0;
		while (i < renderers.size())
		{
			mInstances.clear();
			if (renderers[i]->IsVisible())
			{
				mInstances.push_back(renderers[i]);
				//std::cout << mInstances.front()->GetBitRendererInformation().GetValue() << " : " << mInstances.front()->GetBitRendererInformation().GetString() << "\n";
				++i;

				while (i < renderers.size() &&
					mInstances.front()->IsInstancingAllowed() &&
					mInstances.front()->GetBitRendererInformation().GetValueWithoutDistance() == renderers[i]->GetBitRendererInformation().GetValueWithoutDistance())
				{
					if (renderers[i]->IsVisible())
					{
						if (renderers[i]->IsPrerendered())
						{
							mInstances.insert(mInstances.begin(), renderers[i]);
						}
						else
						{
							mInstances.push_back(renderers[i]);
						}
					}
					++i;
				}
				//std::cout << " renderer " << mInstances[0]->GetModel()->GetName() << "\n";
				RenderInstances(renderPass, mInstances[0], mInstances);
			}
			else
			{
				++i;
			}
		}

		mInstances.clear();
		mRenderersPerPass[renderPass->GetLayersMask()].clear();
	}
	else
	{
		assert(mInstances.size() == 0);
	}
}

void RenderSystem::AddOrReplaceRenderPass(RenderPass* renderPass, bool addAfterPostProcessing)
{
	RemoveRenderPass(renderPass);
	AddRenderPass(renderPass, addAfterPostProcessing);
}

void RenderSystem::AddOrReplaceRenderPassFirst(RenderPass* renderPass, bool addAfterPostProcessing)
{
	RemoveRenderPass(renderPass);
	AddRenderPass(renderPass, addAfterPostProcessing, true);
}

void RenderSystem::AddRenderPass(RenderPass* renderPass, bool addAfterPostProcessing, bool insertFirst)
{
	assert(renderPass != nullptr);
	assert(renderPass->GetCamera() != nullptr);

	if (addAfterPostProcessing)
	{
		bool found = std::find(mRenderPassesAfterPostProcessing.begin(), mRenderPassesAfterPostProcessing.end(), renderPass) != mRenderPassesAfterPostProcessing.end();
		if (!found)
		{
			bool isRenderPassOK = ValidateRenderPassesLayerMasks(renderPass, mRenderPassesAfterPostProcessing);
			
			if (isRenderPassOK)
			{
				mRenderPassesAfterPostProcessing.push_back(renderPass);
			}
			else
			{
				Log(Log::LOG_WARNING) << "There is a render pass with the same layer masks as another render pass. Must be different!" << "\n";
				assert(isRenderPassOK);
			}
		}
	}
	else
	{
		bool found = std::find(mRenderPasses.begin(), mRenderPasses.end(), renderPass) != mRenderPasses.end();
		if (!found)
		{
			bool isRenderPassOK = ValidateRenderPassesLayerMasks(renderPass, mRenderPasses);
			
			if (isRenderPassOK)
			{
				insertFirst ? PushRenderPassFront(renderPass) : PushRenderPassBack(renderPass);
			}
			else
			{
				Log(Log::LOG_WARNING) << "There is a render pass with the same layer masks as another render pass. Must be different!" << "\n";
				assert(isRenderPassOK);
			}
		}
	}
}

void RenderSystem::PushRenderPassFront(RenderPass* renderPass)
{
	mRenderPasses.push_back(renderPass);
	std::rotate(mRenderPasses.rbegin(), mRenderPasses.rbegin() + 1, mRenderPasses.rend());
}

void RenderSystem::PushRenderPassBack(RenderPass* renderPass)
{
	mRenderPasses.push_back(renderPass);
}

bool RenderSystem::ValidateRenderPassesLayerMasks(RenderPass* renderPass, std::vector<RenderPass*>& renderPasses) const
{
	for (const RenderPass* pass : renderPasses)
	{
		if (pass->GetLayersMask() == renderPass->GetLayersMask())
		{
			return false;
		}
	}

	return true;
}

void RenderSystem::RemoveRenderPass(RenderPass* renderPass)
{
	RenderPassesIterator it = std::find_if(mRenderPasses.begin(), mRenderPasses.end(), [&](RenderPass* pass) { return pass->GetName() == renderPass->GetName(); });
	bool found = it != mRenderPasses.end();
	if (found)
	{
		mRenderPasses.erase(it);
	}
	else
	{
		RenderPassesIterator it = std::find_if(mRenderPassesAfterPostProcessing.begin(), mRenderPassesAfterPostProcessing.end(), [&](RenderPass* pass) { return pass->GetName() == renderPass->GetName(); });
		bool found = it != mRenderPassesAfterPostProcessing.end();
		if (found)
		{
			mRenderPassesAfterPostProcessing.erase(it);
		}
	}
}

void RenderSystem::AddToRender(IRenderer* renderer)
{
	//assert(renderer != nullptr);
	if (renderer != nullptr)
	{
		bool added = AddToRender(renderer, mRenderPasses);
		added |= AddToRender(renderer, mRenderPassesAfterPostProcessing);

		if (added)
		{
			mNumberRenderers++;
		}
	}
}

bool RenderSystem::AddToRender(IRenderer* renderer, std::vector<RenderPass*>& renderPasses)
{
	bool added = false;
	for (const RenderPass* pass : renderPasses)
	{
		if (pass->CanAcceptRenderer(renderer))
		{
			mRenderersPerPass[pass->GetLayersMask()].push_back(renderer);
			added = true;
		}
	}
	return added;
}

void RenderSystem::AddCamera(ICamera* camera)
{
	mCamerasList[camera->GetName()] = camera;
}

void RenderSystem::RemoveCamera(const std::string& key)
{
	if (HasCamera(key))
	{
		mCamerasList.erase(key);
	}
}

bool RenderSystem::HasCamera(const std::string& key)
{
	return mCamerasList.find(key) != mCamerasList.end();
}

void RenderSystem::AddOrReplaceCamera(ICamera* camera)
{
	if (HasCamera(camera->GetName()))
	{
		RemoveCamera(camera->GetName());
	}
	AddCamera(camera);
}

ICamera* RenderSystem::GetCamera(const std::string& name)
{
	if (mCamerasList.find(name) != mCamerasList.end())
	{
		return mCamerasList.at(name);
	}
	else
	{
		return nullptr;
	}
}

void RenderSystem::ReadCamerasFrom(core::utils::IDeserializer* source)
{
	source->BeginAttribute(std::string("cameras"));
	unsigned int numElements = source->ReadNumberOfElements();

	source->BeginAttribute(std::string("camera"));
	do
	{
		ReadCameraFrom(source);

		source->NextAttribute();
		numElements--;

	} while (numElements > 0);

	source->EndAttribute();
	source->EndAttribute();
}

void RenderSystem::ReadCameraFrom(core::utils::IDeserializer* source)
{
	//TODO esto no me termina de gustar. Veo una soluci�n que es leer aqu� los datos de la c�mara, lo que tampoco es estupendo
	//crear un constructor vac�o y meter un Build dentro de la c�mara, pero no me convence tampoco.
	ICamera* camera;
	float fov;

	if (source->ReadParameter("fov", &fov))
	{
		camera = InstantiableObject::CreatePerspectiveCamera("PerspectiveCamera", fov, 0.0f, 0.0f, 0.0f);
	}
	else 
	{
		//orthogonal camera
		camera = InstantiableObject::CreateOrthogonalCamera("OrthogonalCamera", 0.0f, 0.0f, 0.0f, 0.0f);
	}

	bool isGameplayCamera = false;
	bool isFreeCamera = false;
	source->ReadParameter("is_gameplay_camera", &isGameplayCamera);
	source->ReadParameter("is_free_camera", &isFreeCamera);

	if (isGameplayCamera)
	{
		mGameplayCamera = camera;
	}
	else if (isFreeCamera)
	{
		mFreeCamera = camera;
	}
	
	camera->ReadFrom(source);
	AddOrReplaceCamera(camera);
}

void RenderSystem::DestroyCameras()
{
	for (CamerasListIterator it = mCamerasList.begin(); it != mCamerasList.end(); ++it)
	{
		delete it->second;
	}
	mCamerasList.clear();
}

void RenderSystem::RenderInstances(RenderPass* renderPass, IRenderer* renderer, std::vector<IRenderer*>& instances)
{
	//std::cout << instances.front()->GetBitRendererInformation().GetValue() << " : " << instances.size() << "\n";

	renderer->SetInstances(instances);

	if (instances.size() > 1)
	{
		renderer->EnableInstancing(true);
	}

	if (!renderer->GetModel()->IsBuilt())
	{
		//initializing the model with the material it has. Once initialized, will be ok 
		//to change the material.
		renderer->GetModel()->Build(mVertexsBuffersManager, renderer->GetMaterial());
	}

	SelectMaterial(renderPass, renderer);

	SelectClippingPlane(renderPass);

	ApplyLights(renderer);

	ApplyShadows(renderer);

	ApplyFog(renderPass, renderer);

	mCurrentMaterial->Use();

	SelectTextures();	

	ApplyDepthBuffer(renderer);

	ApplyBlending(renderer);

	ApplyCulling(renderer);

	renderer->Render(renderPass->GetCamera(), mVertexsBuffersManager, mCurrentMaterial);

	mCurrentMaterial->UnUse();

	mNumberDrawCalls++;
	mNumberTrianglesRendered += renderer->GetNumberTrianglesRendered();
}

void RenderSystem::ApplyDepthTest(bool isEnabled, int mask, int func)
{
	if (isEnabled)
	{
		if (!mIsLastRendererHadDepthBufferEnabled || (mLastDepthBufferMask != mask || mLastDepthBufferFunc != func))
		{
			if (mLastDepthBufferFunc != func)
			{
				mLastDepthBufferFunc = func;
				glDepthFunc(mLastDepthBufferFunc);
			}

			if (mLastDepthBufferMask != mask)
			{
				mLastDepthBufferMask = mask;
				glDepthMask(mLastDepthBufferMask);
			}
		}
		if (!mIsLastRendererHadDepthBufferEnabled)
		{
			glEnable(GL_DEPTH_TEST);
			mIsLastRendererHadDepthBufferEnabled = true;
		}
	}
	else if (mIsLastRendererHadDepthBufferEnabled)
	{
		mIsLastRendererHadDepthBufferEnabled = false;
		glDisable(GL_DEPTH_TEST);
	}
}

void RenderSystem::ApplyDepthBuffer(IRenderer* renderer)
{
	//std::cout << " renderer depth? " << renderer->IsDepthBufferEnabled() << " mask? " << renderer->GetDepthBufferMask() << " \n";

	ApplyDepthTest(renderer->IsDepthBufferEnabled(), renderer->GetDepthBufferMask(), renderer->GetDepthBufferFunc());
}

void RenderSystem::ApplyBlending(IRenderer* renderer)
{
	if (renderer->IsBlendingEnabled() || mIsOverdrawEnabled)
	{
		int mNextBlendSFactor;
		int mNextBlendDFactor;

		if (mIsOverdrawEnabled)
		{
			mNextBlendSFactor = GL_SRC_ALPHA;
			mNextBlendDFactor = GL_ONE_MINUS_SRC_ALPHA;
		}
		else
		{
			mNextBlendSFactor = renderer->GetBlendingSFactor();
			mNextBlendDFactor = renderer->GetBlendingDFactor();
		}

		if (!mLastRendererHadBlendingEnabled || (mLastBlendSFactor != mNextBlendSFactor || 
													mLastBlendDFactor != mNextBlendDFactor))
		{
				mLastBlendSFactor = mNextBlendSFactor;
				mLastBlendDFactor = mNextBlendDFactor;
				glBlendFunc(mLastBlendSFactor, mLastBlendDFactor);
		}

		if (!mLastRendererHadBlendingEnabled)
		{
			mLastRendererHadBlendingEnabled = true;
			glEnable(GL_BLEND);
		}
	}
	
	if(!mIsOverdrawEnabled && !renderer->IsBlendingEnabled())
	{
		if (mLastRendererHadBlendingEnabled)
		{
			mLastRendererHadBlendingEnabled = false;
			glDisable(GL_BLEND);
		}
	}
}

void RenderSystem::ApplyCulling(IRenderer* renderer)
{
	if (renderer->IsCullingEnabled())
	{
		if (!mLastRendererHadCullingEnabled)
		{
			mLastRendererHadCullingEnabled = true;
			glEnable(GL_CULL_FACE);
		}
	}
	else
	{
		if (mLastRendererHadCullingEnabled)
		{
			mLastRendererHadCullingEnabled = false;
			glDisable(GL_CULL_FACE);
		}
	}
}

void RenderSystem::ApplyLights(IRenderer* renderer)
{
	if (mEnvironmentSystem != nullptr)
	{
		MaterialEffectDirectionalLightProperties* effect = mCurrentMaterial->GetEffect<MaterialEffectDirectionalLightProperties>();
		if (effect != nullptr)
		{
			effect->SetSunLightProperties(mEnvironmentSystem->GetSunLightDirection(), mEnvironmentSystem->GetSunLightColor());
		}
	}
	MaterialEffectTextureCubemap* textureCubemapMaterial = mCurrentMaterial->GetEffect<MaterialEffectTextureCubemap>();
	if (textureCubemapMaterial != nullptr)
	{
		TextureCubemap* cubemap1 = static_cast<TextureCubemap*>(mTexturesLibrary->GetElement(mEnvironmentSystem->GetSkyBoxCubemapOrigin()));
		TextureCubemap* cubemap2 = static_cast<TextureCubemap*>(mTexturesLibrary->GetElement(mEnvironmentSystem->GetSkyBoxCubemapDestination()));

		textureCubemapMaterial->SetCubemap1(cubemap1);
		textureCubemapMaterial->SetCubemap2(cubemap2);
		textureCubemapMaterial->SetBlendFactor(mEnvironmentSystem->GetSunLightBlendFactor());
	}
}

void RenderSystem::ApplyShadows(IRenderer* renderer)
{
	if (mShadowsRenderPass->IsEnabled())
	{
		assert(mEnvironmentSystem != nullptr);		

		MaterialEffectShadowProperties* effect = mCurrentMaterial->GetEffect<MaterialEffectShadowProperties>();
		if (effect != nullptr)
		{
			glm::vec3 sunLightDirection = -mEnvironmentSystem->GetSunLightDirection();
			SetCastingShadowsParameters(sunLightDirection, SHADOWS_PFC_COUNTER);
			int effectPFCCounter = effect->GetPFCCounter() == -1 ? mShadowsRenderPass->GetShadowMapPFCCounter() : effect->GetPFCCounter();
			effect->SetParameters(mShadowsRenderPass->GetShadowMapTexture(),
				mShadowsRenderPass->GetShadowMapMatrix(),
				effectPFCCounter);
		}
	}
}

void RenderSystem::ApplyFog(RenderPass* renderPass, IRenderer* renderer)
{
	MaterialEffectFogProperties* effect = mCurrentMaterial->GetEffect<MaterialEffectFogProperties>();
	if (effect != nullptr)
	{
		if (mIsFogEnabled && renderPass->IsFogEnabled())
		{
			effect->SetProperties(mEnvironmentSystem->GetFogColor(), mEnvironmentSystem->GetFogDensity(), mEnvironmentSystem->GetFogGradient());
		}
		else 
		{
			effect->SetProperties(glm::vec3(0.0f), 0.0f, 1.0f); // to make fog not enabled
		}
	}
}

void RenderSystem::SetEnvironmentSystem(EnvironmentSystem* environmentSystem)
{
	mEnvironmentSystem = environmentSystem;
}

void RenderSystem::SelectClippingPlane(RenderPass* renderPass)
{
	//Apply clipping planes
	if (renderPass->IsClippingEnabled())
	{
		mIsClippingEnabled = true;
		mLastClipPlaneNumberUsed = renderPass->GetClippingPlaneNumber();
		glEnable(mLastClipPlaneNumberUsed);
		MaterialEffectClippingPlane* effect = mCurrentMaterial->GetEffect<MaterialEffectClippingPlane>();
		if (effect != nullptr)
		{
			effect->SetClippingPlane(renderPass->GetClippingPlane());
		}
	}
	else
	{
		if (mIsClippingEnabled)
		{
			mIsClippingEnabled = false;
			glDisable(mLastClipPlaneNumberUsed);
		}
	}
}

//Texture units:
//GL_TEXTURE0 free
//GL_TEXTURE1 color frame buffer
//GL_TEXTURE2 depth frame buffer
//GL_TEXTURE3-16 in SelectTextures
//GL_TEXTURE7 texture array
//GL_TEXTURE9 cubemap1
//GL_TEXTURE10 cubemap2
//GL_TEXTURE17-19 free
//GL_TEXTURE20 shadows
//GL_TEXTURE21-31 free
//GL_TEXTURE25 depth

void RenderSystem::SelectTextures()
{
	MaterialEffectDiffuseTexture* diffuseTexture = mCurrentMaterial->GetEffect<MaterialEffectDiffuseTexture>();
	if (diffuseTexture != nullptr)
	{
		if (diffuseTexture->GetDiffuseTexture() != mDiffuseTexture)
		{
			mDiffuseTexture = diffuseTexture->GetDiffuseTexture();
			mDiffuseTexture->SetUnit(GL_TEXTURE3);
			mDiffuseTexture->SetActive(true);
		}
	}

	MaterialEffectNormalTexture* normalMaterial = mCurrentMaterial->GetEffect<MaterialEffectNormalTexture>();
	if (normalMaterial != nullptr)
	{
		if (normalMaterial->GetNormalTexture() != mNormalTexture)
		{
			mNormalTexture = normalMaterial->GetNormalTexture();
			mNormalTexture->SetUnit(GL_TEXTURE4);
			mNormalTexture->SetActive(true);
		}
	}

	MaterialEffectDepthTexture* depthMaterial = mCurrentMaterial->GetEffect<MaterialEffectDepthTexture>();
	if (depthMaterial != nullptr)
	{
		//if (depth != mNormalTexture)
		{
			//mNormalTexture = normal;
			depthMaterial->GetDepthTexture()->SetUnit(GL_TEXTURE25);
			depthMaterial->GetDepthTexture()->SetActive(true);
		}
	}

	MaterialEffectHeightMapTexture* heightmapMaterial = mCurrentMaterial->GetEffect<MaterialEffectHeightMapTexture>();
	if (heightmapMaterial != nullptr)
	{
		heightmapMaterial->GetHeightMapTexture()->SetUnit(GL_TEXTURE6);
		heightmapMaterial->GetHeightMapTexture()->SetActive(true);
	}

	MaterialEffectTextureArray* textureArrayMaterial = mCurrentMaterial->GetEffect<MaterialEffectTextureArray>();
	if (textureArrayMaterial != nullptr)
	{
		textureArrayMaterial->GetTextureArray()->SetUnit(GL_TEXTURE7);
		textureArrayMaterial->GetTextureArray()->SetActive(true);
	}

	MaterialEffectTextureCubemap* textureCubemapMaterial = mCurrentMaterial->GetEffect<MaterialEffectTextureCubemap>();
	if (textureCubemapMaterial != nullptr)
	{
		glEnable(GL_TEXTURE_CUBE_MAP);
		textureCubemapMaterial->GetCubemap1()->SetUnit(GL_TEXTURE9);
		textureCubemapMaterial->GetCubemap1()->SetActive(true);
		if (textureCubemapMaterial->GetCubemap2() != nullptr)
		{
			textureCubemapMaterial->GetCubemap2()->SetUnit(GL_TEXTURE10);
			textureCubemapMaterial->GetCubemap2()->SetActive(true);
		}
	}
	else
	{
		glDisable(GL_TEXTURE_CUBE_MAP);
	}

	MaterialEffectWater* effectWater = mCurrentMaterial->GetEffect<MaterialEffectWater>();
	if (effectWater != nullptr)
	{
		if (mWaterRenderPass->IsEnabled())
		{
			effectWater->GetReflectionTexture()->SetUnit(GL_TEXTURE1); //because is frame buffer
			effectWater->GetReflectionTexture()->SetActive(true);

			effectWater->GetRefractionTexture()->SetUnit(GL_TEXTURE11);
			effectWater->GetRefractionTexture()->SetActive(true);

			effectWater->GetDistorsionTexture()->SetUnit(GL_TEXTURE12);
			effectWater->GetDistorsionTexture()->SetActive(true);

			effectWater->GetNormalTexture()->SetUnit(GL_TEXTURE13);
			effectWater->GetNormalTexture()->SetActive(true);

			effectWater->GetDepthTexture()->SetUnit(GL_TEXTURE25);
			effectWater->GetDepthTexture()->SetActive(true);
		}
		else
		{
			Log(Log::LOG_ERROR) << "Water is not enabled and there is a entity trying to use it. Try to enable water or remove the water material from that entity.\n";
		}
	}

	MaterialEffectParticle* effectParticle = mCurrentMaterial->GetEffect<MaterialEffectParticle>();
	if (effectParticle != nullptr)
	{
		effectParticle->GetTexture()->SetUnit(GL_TEXTURE3);
		effectParticle->GetTexture()->SetActive(true);

		effectParticle->GetDepthTexture()->SetUnit(GL_TEXTURE25);
		effectParticle->GetDepthTexture()->SetActive(true);
	}
}

void RenderSystem::SelectMaterial(RenderPass* renderPass, IRenderer* renderer)
{
	IMaterial* material = renderPass->GetMaterial();

	if (mIsOverdrawEnabled)
	{
		material = GetMaterial(MaterialsLibrary::OVERDRAW_MATERIAL_NAME);
		material->CopyMaterialEffectsValuesFrom(renderer->GetMaterial());
	}
	else if (material == nullptr || !mShadowsRenderPass->IsEnabled())
	{
		material = renderer->GetMaterial();
	}
	else 
	{
		material->CopyMaterialEffectsValuesFrom(renderer->GetMaterial());
	}

	if (mCurrentMaterial == nullptr || mCurrentMaterial->GetMaterialID() != material->GetMaterialID())
	{
		mCurrentMaterial = material;
	}
}

float RenderSystem::GetScreenWidth() const
{
	return mScreenWidth;
}

float RenderSystem::GetScreenHeight() const
{
	return mScreenHeight;
}

unsigned int RenderSystem::GetNumberTrianglesRendered() const
{
	return mNumberTrianglesRendered;
}

unsigned int RenderSystem::GetNumberDrawCalls() const
{
	return mNumberDrawCalls;
}

unsigned int RenderSystem::GetNumberRenderers() const
{
	return mNumberRenderers;
}

GLFWmonitor* RenderSystem::GetCurrentMonitor(float* screenWidth, float* screenHeight)
{
	int nmonitors, i;
	int mx, my, mw, mh;
	GLFWmonitor *bestmonitor;
	GLFWmonitor **monitors;
	const GLFWvidmode *mode;

	bestmonitor = NULL;

	monitors = glfwGetMonitors(&nmonitors);

	for (i = 0; i < nmonitors; i++) {
		mode = glfwGetVideoMode(monitors[i]);
		glfwGetMonitorPos(monitors[i], &mx, &my);
		mw = mode->width;
		mh = mode->height;

		if (*screenWidth < mw && *screenHeight < mh)
		{
			*screenWidth = static_cast<float>(mw);
			*screenHeight = static_cast<float>(mh);
			bestmonitor = monitors[i];
		}
	}

	return bestmonitor;
}

GLFWwindow* RenderSystem::GetGLWindow() const
{
	return mWindow;
}

IShaderProgram* RenderSystem::GetShader(const std::string& name) const
{
	return mShadersLibrary->GetElement(name);
}

FontType* RenderSystem::GetFont(const std::string& name) const
{
	return mFontsLibrary->GetElement(name);
}

Model* RenderSystem::GetModel(const std::string& name) const
{
	return mModelsLibrary->GetElement(name);
}

ITexture* RenderSystem::GetTexture(const std::string& name) const
{
	return mTexturesLibrary->GetElement(name);
}

IMaterial* RenderSystem::GetMaterial(const std::string& name) const
{
	return mMaterialsLibrary->GetElement(name);
}

Animation* RenderSystem::GetAnimation(const std::string& name) const
{
	return mAnimationsLibrary->GetElement(name);
}

VertexBuffersManager& RenderSystem::GetVertexBufferManager()
{
	return mVertexsBuffersManager;
}

bool RenderSystem::InitializeWindowAndOpenGL(const std::string& applicationName, bool isFullscreen)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // We want OpenGL 4.5
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Open a window and create its OpenGL context
	GLFWmonitor* monitor = NULL;
	if(isFullscreen)
	{
		mScreenHeight = 0.0f;
		mScreenWidth = 0.0f;
		monitor = GetCurrentMonitor(&mScreenWidth, &mScreenHeight);
	}
	//Log(Log::LOG_INFO) << " screen size ( " << mScreenWidth << ", " << mScreenHeight << ") \n";
	mWindow = glfwCreateWindow(static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), applicationName.c_str(), monitor, NULL);

	if (mWindow == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return false;
	}
	
	/*
	if (isFullscreen)
	{
		GLFWmonitor* monitor = GetCurrentMonitor(mWindow);
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		mScreenWidth = static_cast<float>(mode->width);
		mScreenHeight = static_cast<float>(mode->height);

		glfwDestroyWindow(mWindow);
		mWindow = glfwCreateWindow(static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), applicationName.c_str(), monitor, NULL);

		if (mWindow == NULL) {
			fprintf(stderr, "Failed to open GLFW window.\n");
			getchar();
			glfwTerminate();
			return false;
		}
	}*/

	glfwMakeContextCurrent(mWindow);
	glewExperimental = true; // Needed in core profile 
	CheckGLError();

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return false;
	}
	CheckGLError(); //esto dispara un error 1282 que se puede descartar. Es un error que depende de la version que inicialicemos.

					// Ensure we can capture the escape key being pressed below
					//glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
					//// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//// Set the mouse at the center of the screen
	glfwPollEvents();
	//glfwSetCursorPos(window, 1024/2, 768/2);

	// Light blue background
	//glClearColor(0.66f, 0.87f, 0.9f, 0.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	EnableVSync(true);

	return true;
}

void RenderSystem::EnableVSync(bool enable)
{
	glfwSwapInterval(enable ? 1 : 0);
}

void RenderSystem::CreateResourcesLibraries()
{
	mShadersLibrary = DBG_NEW ShadersLibrary();
	mTexturesLibrary = DBG_NEW TexturesLibrary(glm::vec2(mScreenWidth, mScreenHeight));
	mAnimationsLibrary = DBG_NEW AnimationsLibrary();
	mModelsLibrary = DBG_NEW ModelsLibrary(mTexturesLibrary, mAnimationsLibrary);
	mFontsLibrary = DBG_NEW FontsLibrary(mTexturesLibrary);
	mMaterialsLibrary = DBG_NEW MaterialsLibrary(mTexturesLibrary, mShadersLibrary);
}

void RenderSystem::DestroyResourcesLibraries()
{
	delete mAnimationsLibrary;
	delete mMaterialsLibrary;
	delete mFontsLibrary;
	delete mModelsLibrary;
	delete mTexturesLibrary;
	delete mShadersLibrary;
}

void RenderSystem::UpdateDistancesToCamera(const ICamera* camera, RenderersList* renderers)
{
	glm::vec3 cameraPosition = camera->GetPosition();
	for (IRenderer* renderer : *renderers)
	{
		glm::vec3 position = renderer->GetParent()->GetTransformation()->GetPosition();
		float distanceToCamera = glm::length2(cameraPosition - position);
		distanceToCamera = glm::round(distanceToCamera);
		renderer->SetDistance(static_cast<int>(distanceToCamera));
	}
}

void RenderSystem::SetFullScreen(bool isFullScreen)
{
	mIsFullScreen = isFullScreen;
}

void RenderSystem::SetOverdrawEnabled(bool isOverdrawEnabled)
{
	mIsOverdrawEnabled = isOverdrawEnabled;
}

void RenderSystem::SetPostProcessEnabled(bool isPostProcessEnabled)
{
	mIsPostprocessEnabled = isPostProcessEnabled;
}

void RenderSystem::SetCastingShadowsParameters(const glm::vec3& lightDirection, int pfcCounter)
{
	assert(mShadowsRenderPass != nullptr);
	mShadowsRenderPass->SetCastingShadowsParameters(lightDirection, pfcCounter);
}

void RenderSystem::SetCastingShadowsTarget(const std::shared_ptr<IGameEntity> target)
{
	assert(mShadowsRenderPass != nullptr);
	mShadowsRenderPass->SetCastingShadowsTarget(target);
}

void RenderSystem::SetCastingShadowsEnabled(bool enabled)
{
	assert(mShadowsRenderPass != nullptr);
	mShadowsRenderPass->SetEnable(enabled);
}

void RenderSystem::SetFogEnabled(bool enabled)
{
	mIsFogEnabled = enabled;
}

void RenderSystem::SetWaterEnabled(bool enabled)
{
	assert(mWaterRenderPass != nullptr);
	mWaterRenderPass->SetEnable(enabled);
}

void RenderSystem::SetWaterParameters(const ICamera* camera, float waterY)
{
	mWaterRenderPass->SetWaterParameters(camera, waterY);
}

float RenderSystem::GetWaterHeight() const
{
	return mWaterRenderPass->GetWaterHeight();
}

ITexture* RenderSystem::CreateDepthTexture(const std::string& name, const glm::ivec2& size)
{
	assert(mTexturesLibrary != nullptr);
	return mTexturesLibrary->CreateDepthTexture(name, size);
}

ITexture* RenderSystem::CreateColorTexture(const std::string& name, const glm::ivec2& size)
{
	assert(mTexturesLibrary != nullptr);
	return mTexturesLibrary->CreateColorTexture(name, size);
}

IMaterial* RenderSystem::CreateMaterial(const std::string& name, IShaderProgram* shader)
{
	return mMaterialsLibrary->CreateMaterial(name, shader);
}

GuiTool* RenderSystem::GetGuiTool()
{
	return mGuiTool;
}

void RenderSystem::ChangeToCamera(const std::string& renderPassName, const ICamera* camera)
{
	assert(camera != nullptr);

	RenderPass* renderPass = GetRenderPass(renderPassName);
	if (renderPass != nullptr)
	{
		renderPass->SetCamera(camera);
	}
}

void RenderSystem::ChangeToCamera(const std::string& cameraName, const std::string& newCameraName)
{
	ICamera* camera = GetCamera(cameraName);
	ICamera* newCamera = GetCamera(newCameraName);

	assert(camera != nullptr);
	assert(newCamera != nullptr);

	for (RenderPass* pass : mRenderPasses)
	{
		if (pass->GetCamera()->GetName() == cameraName)
		{
			pass->SetCamera(newCamera);
		}
	}
	mWaterRenderPass->SetWaterParameters(newCamera, mWaterRenderPass->GetWaterHeight());
}

void RenderSystem::CheckGLError()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		Log(Log::LOG_ERROR) << "OpenGL error: " << err;
	}
}

void RenderSystem::ReadFrom(core::utils::IDeserializer* source)
{
	source->BeginAttribute("libraries");
		mFontsLibrary->ReadFrom(source);
		mShadersLibrary->ReadFrom(source);
		mModelsLibrary->ReadFrom(source);
		mTexturesLibrary->ReadFrom(source);
		mMaterialsLibrary->ReadFrom(source);
		ReadCamerasFrom(source);
	source->EndAttribute();
	source->BeginAttribute("renderer");
		ReadRenderLayersFrom(source);
		mWaterRenderPass->ReadFrom(source);
		mShadowsRenderPass->ReadFrom(source);
		ReadFogParameters(source);
		ReadRenderPassesFrom(source);
	source->EndAttribute();
}

void RenderSystem::ReadRenderLayersFrom(core::utils::IDeserializer* source)
{
	source->BeginAttribute("render_layers");
	
	source->EndAttribute();
}

void RenderSystem::ReadFogParameters(core::utils::IDeserializer* source)
{
	source->BeginAttribute("fog");
	source->ReadParameter("is_enabled", &mIsFogEnabled);
	source->EndAttribute();
}

void RenderSystem::ReadRenderPassesFrom(core::utils::IDeserializer* source)
{
	source->BeginAttribute(std::string("render_passes"));
	unsigned int numElements = source->ReadNumberOfElements();

	source->BeginAttribute(std::string("render_pass"));
	do
	{
		ReadRenderPassFrom(source);

		source->NextAttribute();
		numElements--;

	} while (numElements > 0);

	source->EndAttribute();
	source->EndAttribute();
}

void RenderSystem::ReadRenderPassFrom(core::utils::IDeserializer* source)
{
	bool addAfterPostProcessing = false;
	std::string cameraName, renderPassName;
	int layerMask = IRenderer::LAYER_OTHER;

	source->ReadParameter("name", renderPassName);
	source->ReadParameter("camera_name", cameraName);
	source->ReadParameter("layer_mask", &layerMask);
	source->ReadParameter("add_after_post_processing", &addAfterPostProcessing);

	ICamera* camera = GetCamera(cameraName);
	if (camera != nullptr)
	{
		RenderPass* renderPass = DBG_NEW RenderPass(renderPassName, camera, layerMask);
		renderPass->ReadFrom(source);
		AddOrReplaceRenderPass(renderPass, addAfterPostProcessing);
	}
	else
	{
		Log(Log::LOG_ERROR) << "Couldn't load render pass " << renderPassName << " because camera " << cameraName << " not available." <<"\n";
	}
}

void RenderSystem::WriteTo(core::utils::ISerializer* destination)
{
}

BaseVisitable<>::ReturnType RenderSystem::Accept(BaseVisitor& guest)
{
	return AcceptImpl(*this, guest);
}