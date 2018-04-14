#include "stdafx.h"
#include "RenderSystem.h"
#include "ShadowsRenderPassSubSystem.h"
#include "WaterRenderPassSubSystem.h"

#include "PostProcessSubSystem.h"

#include "../../GameEntity.h"
#include "../../camera/ICamera.h"

#include "../../models/ModelsLibrary.h"
#include "../../models/Model.h"
#include "../../shaders/IShaderProgram.h"
#include "../../shaders/ShadersLibrary.h"
#include "../../font/FontsLibrary.h"
#include "../../textures/TexturesLibrary.h"
#include "../../textures/Texture.h"
#include "../../textures/TextureArray.h"
#include "../../textures/TextureCubemap.h"
#include "../../materials/IMaterial.h"
#include "../../materials/MaterialsLibrary.h"
#include "../../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../../materials/effects/MaterialEffectNormalTexture.h"
#include "../../materials/effects/MaterialEffectHeightMapTexture.h"
#include "../../materials/effects/MaterialEffectClippingPlane.h"
#include "../../materials/effects/MaterialEffectShadowProperties.h"
#include "../../materials/effects/MaterialEffectWater.h"
#include "../../materials/effects/MaterialEffectTextureArray.h"
#include "../../materials/effects/MaterialEffectTextureCubemap.h"
#include "../../materials/effects/MaterialEffectParticle.h"
#include "../../materials/effects/MaterialEffectDepthTexture.h"

#include "RenderPass.h"
#include "../../../BitNumber.h"

#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/norm.hpp>

using namespace std;

const char* RenderSystem::OVERDRAW_MATERIAL_NAME = "overdraw";
const char* RenderSystem::WIREFRAME_MATERIAL_NAME = "wireframe";

RenderSystem::RenderSystem(float screenWidth, float screenHeight) :
mScreenWidth(screenWidth),
mScreenHeight(screenHeight),
mShadersLibrary(nullptr),
mTexturesLibrary(nullptr),
mModelsLibrary(nullptr),
mFontsLibrary(nullptr),
mWindow(nullptr),
mShadowsRenderPass(nullptr),
mWaterRenderPass(nullptr),
mPostProcessSubsystem(nullptr),
mCurrentMaterial(nullptr),
mDiffuseTexture(nullptr),
mNormalTexture(nullptr),
mLastClipPlaneNumberUsed(0),
mIsFullScreen(false),
mIsClippingEnabled(false),
mIsOverdrawEnabled(false),
mIsPostprocessEnabled(true),
mNumberTrianglesRendered(0),
mNumberDrawCalls(0)
{
	BitNumber bit;
	bit.Test();
}

RenderSystem::~RenderSystem()
{
	for (const RenderPass* pass : mRenderPasses)
	{
		delete pass;
	}

	DestroyResourcesLibraries();
	DestroySubSystems();
	glfwDestroyWindow(mWindow);
}

//Initializing the engine basic stuff. The subsystems which need information apart will be initialized later
void RenderSystem::Init(const std::string& applicationName, bool isFullscreen)
{
	mIsFullScreen = isFullscreen;
	bool initialized = InitializeWindowAndOpenGL(applicationName, mIsFullScreen);
	assert(initialized);

	CheckGLError();

	CreateResourcesLibraries();
	LoadResources();
	CreateNewResources();
	CreateSubSystems();
}

void RenderSystem::Start()
{
	mShadowsRenderPass->Init();
	mWaterRenderPass->Init();
	mPostProcessSubsystem->Init();
}

void RenderSystem::CreateSubSystems()
{
	mShadowsRenderPass = new ShadowsRenderPassSubSystem(this, GetScreenWidth(), GetScreenHeight());
	mWaterRenderPass = new WaterRenderPassSubSystem(this, GetScreenWidth(), GetScreenHeight());

	mPostProcessSubsystem = new PostProcessSubSystem(this);
}

void RenderSystem::DestroySubSystems()
{
	delete mPostProcessSubsystem;
	delete mWaterRenderPass;
	delete mShadowsRenderPass;
}

void RenderSystem::LoadResources()
{
	mShadersLibrary->Load();
	mModelsLibrary->Load();
	mFontsLibrary->Load();
	mTexturesLibrary->Load();
	mMaterialsLibrary->Load();
}

void RenderSystem::CreateNewResources()
{
	mMaterialsLibrary->CreateMaterial(OVERDRAW_MATERIAL_NAME, mShadersLibrary->GetElement(OVERDRAW_MATERIAL_NAME));
	mMaterialsLibrary->CreateMaterial(WIREFRAME_MATERIAL_NAME, mShadersLibrary->GetElement("default"));
}

void RenderSystem::UpdateSubsystems()
{
	mShadowsRenderPass->Update();
	mWaterRenderPass->Update();
}

void RenderSystem::Render()
{
	UpdateSubsystems();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mNumberDrawCalls = 0;
	mNumberTrianglesRendered = 0;

	RenderPasses(mRenderPasses);

	if (mIsPostprocessEnabled)
	{
		mPostProcessSubsystem->Render();
	}

	RenderPasses(mRenderPassesAfterPostProcessing);

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
				pass->BindOutput();
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

void RenderSystem::Render(RenderPass* renderPass)
{
	RenderersList renderers = mRenderersPerPass[renderPass->GetLayersMask()];

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

void RenderSystem::AddRenderPass(RenderPass* renderPass, bool addAfterPostProcessing)
{
	assert(renderPass != nullptr);
	assert(renderPass->GetCamera() != nullptr);

	if (addAfterPostProcessing)
	{
		bool found = std::find(mRenderPassesAfterPostProcessing.begin(), mRenderPassesAfterPostProcessing.end(), renderPass) != mRenderPassesAfterPostProcessing.end();
		if (!found)
		{
			mRenderPassesAfterPostProcessing.push_back(renderPass);
		}
	}
	else
	{
		bool found = std::find(mRenderPasses.begin(), mRenderPasses.end(), renderPass) != mRenderPasses.end();
		if (!found)
		{
			mRenderPasses.push_back(renderPass);
		}
	}
}

void RenderSystem::RemoveRenderPass(RenderPass* renderPass)
{
	RenderPassesIterator it = std::find(mRenderPasses.begin(), mRenderPasses.end(), renderPass);
	bool found = it != mRenderPasses.end();
	if (found)
	{
		mRenderPasses.erase(it);
	}
	else
	{
		RenderPassesIterator it = std::find(mRenderPassesAfterPostProcessing.begin(), mRenderPassesAfterPostProcessing.end(), renderPass);
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
		AddToRender(renderer, mRenderPasses);
		AddToRender(renderer, mRenderPassesAfterPostProcessing);
	}
}

void RenderSystem::AddToRender(IRenderer* renderer, std::vector<RenderPass*>& renderPasses)
{
	char layer = renderer->GetLayer();
		
	for (const RenderPass* pass : renderPasses)
	{
		char result = pass->GetLayersMask() & layer;
		if (result != 0)
		{
			mRenderersPerPass[pass->GetLayersMask()].push_back(renderer);
		}
	}
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

	ApplyShadows(renderer);

	mCurrentMaterial->Use();

	SelectTextures();
	
	//TODO esto se tiene que mejorar. Sinó estamos haciendo una llamada a blend que igual no hace falta
	if (mIsOverdrawEnabled)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else 
	{
		glDisable(GL_BLEND);
	}

	renderer->Render(renderPass->GetCamera(), mVertexsBuffersManager, mCurrentMaterial);

	mCurrentMaterial->UnUse();

	mNumberDrawCalls++;
	mNumberTrianglesRendered += renderer->GetNumberTrianglesRendered();
}

void RenderSystem::ApplyShadows(IRenderer* renderer)
{
	if (mShadowsRenderPass->IsEnabled())
	{
		MaterialEffectShadowProperties* effect = mCurrentMaterial->GetEffect<MaterialEffectShadowProperties>();
		if (effect != nullptr)
		{
			effect->SetParameters(mShadowsRenderPass->GetShadowMapTexture(),
				mShadowsRenderPass->GetShadowMapMatrix(),
				mShadowsRenderPass->GetShadowMapPFCCounter());
		}
	}
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

void RenderSystem::SelectTextures()
{
	MaterialEffectDiffuseTexture* diffuseTexture = mCurrentMaterial->GetEffect<MaterialEffectDiffuseTexture>();
	if (diffuseTexture != nullptr)
	{
		if (diffuseTexture->GetDiffuseTexture() != mDiffuseTexture)
		{
			mDiffuseTexture = diffuseTexture->GetDiffuseTexture();
			mDiffuseTexture->SetActive(true);
		}
	}

	MaterialEffectNormalTexture* normalMaterial = mCurrentMaterial->GetEffect<MaterialEffectNormalTexture>();
	if (normalMaterial != nullptr)
	{
		if (normalMaterial->GetNormalTexture() != mNormalTexture)
		{
			mNormalTexture = normalMaterial->GetNormalTexture();
			mNormalTexture->SetActive(true);
		}
	}

	MaterialEffectDepthTexture* depthMaterial = mCurrentMaterial->GetEffect<MaterialEffectDepthTexture>();
	if (depthMaterial != nullptr)
	{
		//if (depth != mNormalTexture)
		{
			//mNormalTexture = normal;
			depthMaterial->GetDepthTexture()->SetActive(true);
		}
	}

	MaterialEffectHeightMapTexture* heightmapMaterial = mCurrentMaterial->GetEffect<MaterialEffectHeightMapTexture>();
	if (heightmapMaterial != nullptr)
	{
		heightmapMaterial->GetHeightMapTexture()->SetActive(true);
	}

	MaterialEffectTextureArray* textureArrayMaterial = mCurrentMaterial->GetEffect<MaterialEffectTextureArray>();
	if (textureArrayMaterial != nullptr)
	{
		textureArrayMaterial->GetTextureArray()->SetActive(true);
	}

	MaterialEffectTextureCubemap* textureCubemapMaterial = mCurrentMaterial->GetEffect<MaterialEffectTextureCubemap>();
	if (textureCubemapMaterial != nullptr)
	{
		textureCubemapMaterial->GetCubemap()->SetActive(true);
	}

	MaterialEffectWater* effectWater = mCurrentMaterial->GetEffect<MaterialEffectWater>();
	if (effectWater != nullptr)
	{
		effectWater->GetReflectionTexture()->SetActive(true);
		effectWater->GetRefractionTexture()->SetActive(true);
		effectWater->GetDistorsionTexture()->SetActive(true);
		effectWater->GetNormalTexture()->SetActive(true);
		effectWater->GetDepthTexture()->SetActive(true);
	}

	MaterialEffectParticle* effectParticle = mCurrentMaterial->GetEffect<MaterialEffectParticle>();
	if (effectParticle != nullptr)
	{
		effectParticle->GetTexture()->SetActive(true);
		effectParticle->GetDepthTexture()->SetActive(true);
	}
}

void RenderSystem::SelectMaterial(RenderPass* renderPass, IRenderer* renderer)
{
	IMaterial* material = renderPass->GetMaterial();

	if (mIsOverdrawEnabled)
	{
		material = GetMaterial(OVERDRAW_MATERIAL_NAME);
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

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	// Cull triangles which normal is not towards the camera
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	DisableVSync(true);

	return true;
}

void RenderSystem::DisableVSync(bool enable)
{
	glfwSwapInterval(enable ? 0 : 1);
}

void RenderSystem::CreateResourcesLibraries()
{
	mShadersLibrary = new ShadersLibrary();
	mTexturesLibrary = new TexturesLibrary();
	mModelsLibrary = new ModelsLibrary(mTexturesLibrary);
	mFontsLibrary = new FontsLibrary(mTexturesLibrary);
	mMaterialsLibrary = new MaterialsLibrary();
}

void RenderSystem::DestroyResourcesLibraries()
{
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

void RenderSystem::SetCastingShadowsTarget(const GameEntity* target)
{
	assert(mShadowsRenderPass != nullptr);
	mShadowsRenderPass->SetCastingShadowsTarget(target);
}

void RenderSystem::SetCastingShadowsEnabled(bool enabled)
{
	assert(mShadowsRenderPass != nullptr);
	mShadowsRenderPass->SetEnable(enabled);
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

void RenderSystem::CheckGLError()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "OpenGL error: " << err << std::endl;
	}
}

BaseVisitable<>::ReturnType RenderSystem::Accept(BaseVisitor& guest)
{
	return AcceptImpl(*this, guest);
}