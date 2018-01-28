#include "stdafx.h"
#include "RenderSystem.h"
#include "ShadowsSystem.h"
#include "../GameEntity.h"
#include "../camera/ICamera.h"
#include "../models/ModelsLibrary.h"
#include "../shaders/ShadersLibrary.h"
#include "../font/FontsLibrary.h"
#include "../textures/TexturesLibrary.h"
#include "../textures/Texture.h"
#include "../../renderer/IRenderer.h"
#include "../../renderer/RenderPass.h"
#include "../../BitNumber.h"

#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/norm.hpp>

using namespace std;

RenderSystem::RenderSystem(float screenWidth, float screenHeight) :
mLastClipPlaneNumberUsed(0),
mScreenWidth(screenWidth),
mScreenHeight(screenHeight),
mShadersLibrary(nullptr),
mTexturesLibrary(nullptr),
mModelsLibrary(nullptr),
mFontsLibrary(nullptr),
mWindow(nullptr),
mIsFullScreen(false)
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

	delete mShadowsSystem;
}

void RenderSystem::Init(const std::string& applicationName, bool isFullscreen)
{
	mIsFullScreen = isFullscreen;
	bool initialized = InitializeWindowAndOpenGL(applicationName, mIsFullScreen);
	assert(initialized);

	CheckGLError();

	CreateResourcesLibraries();
	LoadResources();

	mShadowsSystem = new ShadowsSystem(	this,
										GetScreenWidth(), 
										GetScreenHeight(), 
										static_cast<const Texture*>(mTexturesLibrary->GetElement("shadow_texture")));
	mShadowsSystem->Init();
}

void RenderSystem::LoadResources()
{
	mShadersLibrary->Load();
	mModelsLibrary->Load();
	mFontsLibrary->Load();
	mTexturesLibrary->Load();
}

void RenderSystem::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const RenderPass* pass : mRenderPasses)
	{
		if (pass->IsEnabled())
		{
			if (pass->HasFrameBufferOutput())
			{
				pass->BindOutput();
				Render(pass);
				pass->UnbindOutput();
				pass->CopyBuffer();
			}
			else
			{
				Render(pass);
			}
		}
	}

	// Swap buffers
	glfwSwapBuffers(mWindow);
	glfwPollEvents();
}

void RenderSystem::Render(const RenderPass* renderPass)
{
	RenderersList renderers = mRenderersPerPass[renderPass->GetLayersMask()];

	//STEP 1 CALUCLATE DISTANCE TO CAMERA FOR EACH RENDERER
	if (renderPass->HasToCalculateDistanceToCamera())
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

void RenderSystem::AddRenderPass(const RenderPass* renderPass)
{
	mRenderPasses.push_back(renderPass);
}

void RenderSystem::AddToRender(IRenderer* renderer)
{
	//assert(renderer != nullptr);
	if (renderer != nullptr)
	{
		char layer = renderer->GetLayer();
		for (const RenderPass* pass : mRenderPasses)
		{
			char result = pass->GetLayersMask() & layer;
			if ( result != 0)
			{
				mRenderersPerPass[pass->GetLayersMask()].push_back(renderer);
			}
		}
	}
}

void RenderSystem::RenderInstances(const RenderPass* renderPass, IRenderer* renderer, std::vector<IRenderer*>& instances)
{
	//std::cout << instances.front()->GetBitRendererInformation().GetValue() << " : " << instances.size() << "\n";
	//Apply fog
	if (renderer->HasFog())
	{
		renderer->EnableFog(renderPass->IsFogEnabled());
	}

	if (renderer->IsCastingShadows())
	{
		renderer->SetShadowMapParameters(	mShadowsSystem->GetShadowMapTexture(), 
											mShadowsSystem->GetShadowMapMatrix(), 
											mShadowsSystem->GetShadowMapPFCCounter());
	}
	//Apply clipping planes
	if (renderPass->IsClippingEnabled())
	{
		mLastClipPlaneNumberUsed = renderPass->GetClippingPlaneNumber();
		glEnable(mLastClipPlaneNumberUsed);
		if (renderer->HasClippingPlane())
		{
			renderer->SetClippingPlane(renderPass->GetClippingPlane());
		}
	}
	else
	{
		glDisable(mLastClipPlaneNumberUsed);
	}

	if (instances.size() > 1)
	{
		renderer->SetInstances(instances);
		renderer->EnableInstancing(true);
	}

	renderer->Render(renderPass->GetCamera(), mVertexsBuffersManager, renderPass->GetShader());
}

float RenderSystem::GetScreenWidth() const
{
	return mScreenWidth;
}

float RenderSystem::GetScreenHeight() const
{
	return mScreenHeight;
}

GLFWmonitor* RenderSystem::GetCurrentMonitor(GLFWwindow *window)
{
	int nmonitors, i;
	int wx, wy, ww, wh;
	int mx, my, mw, mh;
	int overlap, bestoverlap;
	GLFWmonitor *bestmonitor;
	GLFWmonitor **monitors;
	const GLFWvidmode *mode;

	bestoverlap = 0;
	bestmonitor = NULL;

	glfwGetWindowPos(window, &wx, &wy);
	glfwGetWindowSize(window, &ww, &wh);
	monitors = glfwGetMonitors(&nmonitors);

	for (i = 0; i < nmonitors; i++) {
		mode = glfwGetVideoMode(monitors[i]);
		glfwGetMonitorPos(monitors[i], &mx, &my);
		mw = mode->width;
		mh = mode->height;

		overlap =
			glm::max(0, glm::min(wx + ww, mx + mw) - glm::max(wx, mx)) *
			glm::max(0, glm::min(wy + wh, my + mh) - glm::max(wy, my));

		if (bestoverlap < overlap) {
			bestoverlap = overlap;
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
	mWindow = glfwCreateWindow(static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), applicationName.c_str(), NULL, NULL);

	if (mWindow == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return false;
	}

	if (isFullscreen)
	{
		GLFWmonitor* monitor = GetCurrentMonitor(mWindow);
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		mScreenWidth = static_cast<float>(mode->width);
		mScreenHeight = static_cast<float>(mode->height);

		mWindow = glfwCreateWindow(static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), applicationName.c_str(), monitor, NULL);

		if (mWindow == NULL) {
			fprintf(stderr, "Failed to open GLFW window.\n");
			getchar();
			glfwTerminate();
			return false;
		}
	}

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
	glClearColor(0.0f, 0.4f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	// Cull triangles which normal is not towards the camera
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);

	DisableVSync(false);

	return true;
}

void RenderSystem::DisableVSync(bool enable)
{
	glfwSwapInterval(enable ? 1 : 0);
}

void RenderSystem::CreateResourcesLibraries()
{
	mShadersLibrary = new ShadersLibrary();
	mTexturesLibrary = new TexturesLibrary();
	mModelsLibrary = new ModelsLibrary(mTexturesLibrary);
	mFontsLibrary = new FontsLibrary(mTexturesLibrary);
}

void RenderSystem::DestroyResourcesLibraries()
{
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
		int distanceToCamera = static_cast<int>(glm::length2(cameraPosition - position));
		distanceToCamera = glm::min<int>(glm::max<int>(0, distanceToCamera), UINT_MAX);
		renderer->SetDistance(static_cast<int>(distanceToCamera));
	}
}

void RenderSystem::SetFullScreen(bool isFullScreen)
{
	mIsFullScreen = isFullScreen;
}

void RenderSystem::SetCastingShadowsParameters(const glm::vec3& lightDirection, int pfcCounter)
{
	assert(mShadowsSystem != nullptr);
	mShadowsSystem->SetCastingShadowsParameters(lightDirection, pfcCounter);
}

void RenderSystem::SetCastingShadowsTarget(const glm::vec3& position)
{
	assert(mShadowsSystem != nullptr);
	mShadowsSystem->SetCastingShadowsTarget(position);
}

void RenderSystem::CheckGLError()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "OpenGL error: " << err << std::endl;
	}
}