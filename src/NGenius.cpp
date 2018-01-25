#include "stdafx.h"
#include "NGenius.h"

#include <Windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "resources/systems/EntitiesSystem.h"
#include "resources/systems/RenderSystem.h"
#include "resources/systems/PhysicsSystem.h"
#include "resources/systems/ParticlesSystem.h"

#include "resources/models/ModelsLibrary.h"
#include "resources/shaders/ShadersLibrary.h"
#include "resources/textures/TexturesLibrary.h"
#include "resources/font/FontsLibrary.h"

#include "resources/entities/ParticlesEmitter.h"

#include "resources/textures/Texture.h"

NGenius::NGenius(std::string applicationName, float screenWidth, float screenHeight) :
mRenderSystem(nullptr),
mPhysicsSystem(nullptr),
mEntitiesSystem(nullptr),
mParticlesSystem(nullptr),
mShadersLibrary(nullptr),
mTexturesLibrary(nullptr),
mModelsLibrary(nullptr),
mFontsLibrary(nullptr),
mWindow(nullptr),
mScreenWidth(screenWidth), 
mScreenHeight(screenHeight),
mApplicationName(applicationName),
mIsFullScreen(false)
{
}


NGenius::~NGenius()
{
	DestroyResourcesLibraries();
	DestroySystems();
}

void NGenius::Init()
{
	bool initialized = InitializeWindowAndOpenGL();
	assert(initialized);

	CheckGLError();

	CreateResourcesLibraries();
	CreateSystems();
	LoadResources();

	mRenderSystem->SetTextureShadowMap(static_cast<Texture*>(GetTexture("shadow_texture")));
}

void NGenius::Update()
{
	double lastCurrentTime = glfwGetTime();
	int frames = 0;
	float accumulatedTime = 0.0f;
	float elapsedTime = 0.0f;

	do
	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//CheckError();

		UpdateInput(elapsedTime / 20.0f);

		UpdateSystems(elapsedTime);

		if (mUpdateHandler != nullptr)
		{
			mUpdateHandler(elapsedTime);
		}

		mRenderSystem->Render();

		// Swap buffers
		glfwSwapBuffers(mWindow);
		glfwPollEvents();

		double currentTime = glfwGetTime();
		elapsedTime = static_cast<float>(currentTime - lastCurrentTime);
		lastCurrentTime = currentTime;
		frames++;

		accumulatedTime += elapsedTime;
		if (accumulatedTime > 1.0f)
		{
			mFPS = frames / accumulatedTime;
			std::cout << "fps: " << mFPS << "\n";
			accumulatedTime = 0.0f;
			frames = 0;
		}

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(mWindow) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

void NGenius::UpdateInput(float deltaTime)
{
	mInputHandler(mWindow);

	if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		if (glfwGetKey(mWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}
}

void NGenius::UpdateSystems(float elapsedTime)
{
	mParticlesSystem->Update(elapsedTime);
	mEntitiesSystem->Update(elapsedTime);
	mPhysicsSystem->Update(elapsedTime);
}

GLFWmonitor* NGenius::GetCurrentMonitor(GLFWwindow *window)
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

bool NGenius::InitializeWindowAndOpenGL()
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
	mWindow = glfwCreateWindow(static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), mApplicationName.c_str(), NULL, NULL);
	
	if (mWindow == NULL){
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return false;
	}

	if (mIsFullScreen)
	{
		GLFWmonitor* monitor = GetCurrentMonitor(mWindow);
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		mScreenWidth = mode->width;
		mScreenHeight = mode->height;
		
		mWindow = glfwCreateWindow(static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), mApplicationName.c_str(), monitor, NULL);

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

void NGenius::DisableVSync(bool enable)
{
	glfwSwapInterval(enable ? 1 : 0);
}

void NGenius::CreateResourcesLibraries()
{
	mShadersLibrary = new ShadersLibrary();
	mTexturesLibrary = new TexturesLibrary();
	mModelsLibrary = new ModelsLibrary(mTexturesLibrary);
	mFontsLibrary = new FontsLibrary(mTexturesLibrary);
}

void NGenius::CreateSystems()
{
	mRenderSystem = new RenderSystem();
	mPhysicsSystem = new PhysicsSystem();
	mEntitiesSystem = new EntitiesSystem(mRenderSystem, mPhysicsSystem);
	mParticlesSystem = new ParticlesSystem();
}

void NGenius::DestroySystems()
{
	delete mParticlesSystem;
	delete mEntitiesSystem;
	delete mPhysicsSystem;
	delete mRenderSystem;
}

void NGenius::DestroyResourcesLibraries()
{
	delete mFontsLibrary;
	delete mModelsLibrary; 
	delete mTexturesLibrary;
	delete mShadersLibrary;
}

void NGenius::LoadResources()
{
	mShadersLibrary->Load();
	mModelsLibrary->Load();
	mFontsLibrary->Load();
	mTexturesLibrary->Load();	
}

void NGenius::RegisterInputHandler(std::function<void(GLFWwindow* window)> callback)
{
	mInputHandler = callback;
}

void NGenius::RegisterUpdateHandler(std::function<void(float elapsedTime)> callback)
{
	mUpdateHandler = callback;
}

IShaderProgram* NGenius::GetShader(const std::string& name) const
{
	return mShadersLibrary->GetElement(name);
}

FontType* NGenius::GetFont(const std::string& name) const
{
	return mFontsLibrary->GetElement(name);
}

Model* NGenius::GetModel(const std::string& name) const
{
	return mModelsLibrary->GetElement(name);
}

ITexture* NGenius::GetTexture(const std::string& name) const
{
	return mTexturesLibrary->GetElement(name);
}

GLFWwindow* NGenius::GetGLWindow() const
{
	return mWindow;
}

float NGenius::GetFPS() const
{
	return mFPS;
}

float NGenius::GetScreenWidth() const
{
	return mScreenWidth;
}

float NGenius::GetScreenHeight() const
{
	return mScreenHeight;
}

void NGenius::SetFullScreen(bool isFullScreen)
{
	mIsFullScreen = isFullScreen;
}

void NGenius::AddGameEntity(GameEntity* entity)
{
	assert(mEntitiesSystem != nullptr);
	mEntitiesSystem->AddEntity(entity);
}

void NGenius::AddParticleEmitter(ParticlesEmitter* emitter)
{
	assert(mParticlesSystem != nullptr);
	assert(mEntitiesSystem != nullptr);

	emitter->SetEntitiesSystem(mEntitiesSystem);
	mParticlesSystem->AddParticleEmitter(emitter);
}

void NGenius::AddRenderPass(RenderPass* renderPass)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->AddRenderPass(renderPass);
}

void NGenius::SetTerrain(const Terrain* terrain)
{
	assert(mPhysicsSystem != nullptr);
	mPhysicsSystem->SetTerrain(terrain);
}

void NGenius::SetEnergyWallRadius(float radius)
{
	assert(mPhysicsSystem != nullptr);
	mPhysicsSystem->SetEnergyWallRadius(radius);
}

void NGenius::SetCameraCastingShadows(const ICamera* camera)
{
	assert(mRenderSystem != nullptr);
	mRenderSystem->SetCameraCastingShadows(camera);
}

void NGenius::CheckGLError()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cout << "OpenGL error: " << err << std::endl;
	}
}