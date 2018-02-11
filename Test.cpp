
// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <string>
#include <vector>
#include <iostream>
#include <complex>
#include <ctime>

#include "src/NGenius.h"
#include "src/resources/systems/PhysicsSystem.h"

#include "src/TerrainGrid.h"

#include "src/renderer/TerrainRenderer.h"
#include "src/renderer/CubeRenderer.h"
#include "src/renderer/NormalRenderer.h"
#include "src/renderer/ModelRenderer.h"
#include "src/renderer/ModelNormalMapRenderer.h"
#include "src/renderer/SkyBoxRenderer.h"
#include "src/renderer/QuadRenderer.h"
#include "src/renderer/WaterRenderer.h"
#include "src/renderer/GUIRenderer.h"
#include "src/renderer/EnergyWallRenderer.h"
#include "src/renderer/TextRenderer.h"
#include "src/renderer/GUITextRenderer.h"

#include "src/renderer/RenderPass.h"

#include "src/resources/textures/PNGLoader.h"
#include "src/resources/textures/TextureGenerator.h"

#include "src/resources/shaders/IShaderProgram.h"
#include "src/resources/shaders/TerrainShader.h"
#include "src/resources/shaders/SkyBoxShader.h"


#include "src/resources/textures/Texture.h"
#include "src/resources/textures/TextureArray.h"
#include "src/resources/textures/TextureCubemap.h"

#include "src/resources/font/FontType.h"

#include "src/resources/models/Model.h"


#include "src/resources/entities/Light.h"
#include "src/resources/entities/Terrain.h"
#include "src/resources/entities/Player.h"
#include "src/resources/entities/Particle.h"
#include "src/resources/entities/ParticlesEmitter.h"
#include "src/resources/entities/EnergyWall.h"
#include "src/resources/entities/Text.h"

#include "src/resources/camera/ICamera.h"
#include "src/resources/camera/PerspectiveCamera.h"
#include "src/resources/camera/OrthogonalCamera.h"

#include "src/resources/GameEntity.h"
#include "src/resources/Transformation.h"

#include "src/resources/components/VerticalInputComponent.h"
#include "src/resources/components/DebugInputComponent.h"
#include "src/resources/components/PlayerInputComponent.h"
#include "src/resources/components/PhysicsComponent.h"
#include "src/resources/components/CollisionComponent.h"
#include "src/resources/components/EnergyWallCollisionComponent.h"
#include "src/resources/components/ThirdPersonCameraComponent.h"
#include "src/resources/components/CameraComponent.h"
#include "src/resources/components/RotationComponent.h"
#include "src/resources/components/LODComponent.h"
#include "src/resources/components/BillboardComponent.h"
#include "src/resources/components/OverWaterComponent.h"

#include "src/renderer/IFrameBuffer.h"

#include "src/resources/entities/Particle.h"
#include "src/renderer/ParticleRenderer.h"

using namespace glm;
using namespace std;

#define and &&
#define or ||

static const float PLAYER_UPWARDS_HEIGHT = 2.0f;
static const float PLAYER_RUN_SPEED = 2.0f;
static const float PLAYER_TURN_SPEED = glm::radians(160.0f);
static const float VIEW_ANGLE = 45.0f;
static const float FAR_PLANE = 1000.0f;
static const float NEAR_PLANE = 0.1f;
static const float SCREEN_WIDTH = 1024.0f;
static const float SCREEN_HEIGHT = 768.0f;

static const float SKYBOX_ROTATION_SPEED = 0.01f;
static const float MIN_FPS_ALLOWED = 30.0f;

enum Configuration
{
	DEBUG,
	TEXT,
	ENERGY_WALL,
	SHADOWS,
	PARTICLES,
	PROPS,
	REFACTOR,
	RELEASE
};
Configuration mConfiguration = REFACTOR;

int movx[] = { 1, 1, 0, -1, -1, -1, 0, 1 };
int movy[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int despi[] = { 1, 1, -1, -1 };
int despj[] = { 1, -1, 1, -1 };
int muevei[] = { 1, 0, -1, 0 };
int muevej[] = { 0, 1, 0, -1 };


const float Pi = 3.141592f;

float terrainHeightScale = 7.5f;
float mWaterHeight = 8.2f * terrainHeightScale / 15.0f;


bool mIsDebugModeEnabled = false;
bool mIsWaterEnabled = true;
bool mIsGameplayCameraEnabled = true;
bool mIsFogEnabled = true;
bool mIsVegetationEnabled = true;
bool mIsEnergyWallEnabled = true;
bool mIsSkyboxEnabled = true;
bool mIsTerrainFlat = false;
bool mIsTextEnabled = false;
bool mIsStatisticsVisible = false;
bool mIsShadowEnabled = false;
bool mIsParticlesEnabled = false;
bool mIsFullScreen = false;
bool mIsPropsEnabled = false;

NGenius mEngine("Demo", SCREEN_WIDTH, SCREEN_HEIGHT);
ICamera* mGameplayCamera;
ICamera* mGuiCamera;
ICamera* mReflectionWaterCamera;
ICamera* mRefractionWaterCamera;
ICamera* mMapCamera;
ICamera* mEagleEyeCamera;

RenderPass* mMapPass;
Light* mSunLight;
glm::vec3 mSunLightDirection(100000.0f, 100000.0f, 100000.0f);
Terrain* mTerrain;
Player* mPlayer;
GameEntity* mCamera;
GameEntity* mWater;
GameEntity* mWaterReflectionCameraEntity;
GameEntity* mWaterRefractionCameraEntity;
EnergyWall* mEnergyWall;
Text* mFPSText;

float mFogDensity = 0.04f;
const float mFogGradient = 1.5f;
glm::vec3 mFogColor = vec3(89.0f, 120.0f, 143.0f) / 255.0f;
//red glm::vec3 mFogColor = vec3(218.0f, 74.0f, 43.0f) / 255.0f; 
float mEnergyWallRadius = 22.0f;

double aleatori()
{
	int r = rand();
	return double(r % 100) / 100;
}

void CreateTerrainNormals(vector<glm::vec3>& vertexs, int numVertexsSide)
{
	IShaderProgram* defaultShaderProgram = mEngine.GetShader("default");
	NormalRenderer* normalRenderer = new NormalRenderer(defaultShaderProgram);

	//TERRAIN GAME ENTITY
	GameEntity* entity = new GameEntity(	new Transformation(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
											normalRenderer
										);
	std::vector<glm::vec3> normals;
	PNGLoader loader;
	bool loaded = loader.ReadPNGFile("data/terrain_256.png");
	unsigned char* data = loader.GetData();

	for (int j = 0; j < numVertexsSide; ++j)
	{
		for (int i = 0; i < numVertexsSide; ++i)
		{
			int index = i + j * numVertexsSide;
			int d = j + i * numVertexsSide;
			float height = data[4 * d + 3];
			vertexs[index].y = height / 255.0f * terrainHeightScale;

			glm::vec3 normal = glm::vec3(data[4 * d] / 255.0f, data[4 * d + 1] / 255.0f, data[4 * d + 2] / 255.0f);
			//normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
			normals.push_back(normalize(normal * 2.0f + glm::vec3(-1.0f)));
		}
	}
	//terrainRenderer.SetVertexs(vertexs);

	//normalRenderer->SetVertexs(vertexs);
	//normalRenderer->SetNormals(normals);
	normalRenderer->Create();

	mEngine.AddGameEntity(entity);
}

void AddBoundingBoxesFrom(GameEntity* gameEntity)
{
	if (mIsDebugModeEnabled)
	{
		IRenderer* renderer = gameEntity->GetRenderer();
		if (renderer != nullptr)
		{
			const AABB boundingBox = renderer->GetBoundingBox();
			IRenderer* cubeRenderer = new CubeRenderer(mEngine.GetShader("default"));
			cubeRenderer->SetLayer(IRenderer::LAYER_DEBUG);
			Transformation* transformation = new Transformation(*gameEntity->GetTransformation());
			mEngine.AddGameEntity(new GameEntity(transformation, cubeRenderer));
		}
	}
}

void CreateShadowPlane()
{
	//QUAD
	IRenderer* guiShadowRenderer = new GUIRenderer(	mEngine.GetShader("gui"),
														static_cast<Texture*>(mEngine.GetTexture("shadow_texture")),
														10.0f,
														10.0f
													);
	GameEntity* quadShadow = new GameEntity(	new Transformation(glm::vec3(0.0f, -300.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
												guiShadowRenderer
											);
	mEngine.AddGameEntity(quadShadow);
}

void CreateWaterHudPlanes()
{
	//QUAD
	IRenderer* guiReflectionRenderer = new GUIRenderer(	mEngine.GetShader("gui"),
															static_cast<Texture*>(mEngine.GetTexture("reflection_water")),
															128.0f,
															128.0f
														);
	GameEntity* quadReflection = new GameEntity(	new Transformation(glm::vec3(420.0f, -300.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
		guiReflectionRenderer
												);
	mEngine.AddGameEntity(quadReflection);

	IRenderer* guiRefractionRenderer = new GUIRenderer(mEngine.GetShader("gui"),
															static_cast<Texture*>(mEngine.GetTexture("refraction_depth_water")),
															128.0f,
															128.0f
														);
	GameEntity* guiRefraction = new GameEntity(	new Transformation(glm::vec3(-320.0f, -300.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
													guiRefractionRenderer
												);
	mEngine.AddGameEntity(guiRefraction);
}

void CreateHUD()
{
	//QUAD
	IRenderer* guiRenderer = new GUIRenderer(	mEngine.GetShader("gui"),
												static_cast<Texture*>(mEngine.GetTexture("hud_map")),
												128.0f,
												128.0f
												);
	GameEntity* quad = new GameEntity(	new Transformation(glm::vec3(420.0f, -300.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
										guiRenderer
									 );
	mEngine.AddGameEntity(quad);

	IRenderer* mapRenderer = new GUIRenderer(mEngine.GetShader("gui"),
												static_cast<Texture*>(mEngine.GetTexture("map")),
												87.0f,
												73.0f
												);
	GameEntity* map = new GameEntity(new Transformation(glm::vec3(420.0f, -300.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
										mapRenderer
									);
	mEngine.AddGameEntity(map);
}

void CreateSpecificCubes()
{
	std::vector<glm::vec3> positions;
	//positions.push_back(glm::vec3(3.5f, 0.0f, 3.3f));
	positions.push_back(glm::vec3(mEnergyWallRadius, 0.0f, 0.0f));
	//positions.push_back(glm::vec3(1.0f, 5.0f, 4.0f));
	//positions.push_back(glm::vec3(0.0f, 0.0f, 4.0f));

	std::vector<glm::vec3> sizes;
	//sizes.push_back(glm::vec3(0.2f, 0.2f, 0.2f));
	sizes.push_back(glm::vec3(0.13f, 0.13f, 0.13f));
	//sizes.push_back(glm::vec3(0.2f, 1.0f, 0.2f));
	//sizes.push_back(glm::vec3(0.2f, 0.2f, 0.2f));

	for (unsigned long i = 0; i < positions.size(); i++)
	{
		ModelNormalMapRenderer* modelRenderer = new ModelNormalMapRenderer(mEngine.GetModel("cube"),
			mEngine.GetShader("model"),
			static_cast<Texture*>(mEngine.GetTexture("cube_diffuse")),
			static_cast<Texture*>(mEngine.GetTexture("cube_normalmap")),
			mSunLight
			);
		modelRenderer->SetFogParameters(mFogColor, mFogDensity, mFogGradient);
		modelRenderer->SetTile(4.0f);

		GameEntity* modelEntity = new GameEntity(
			new Transformation(positions[i], glm::vec3(0.0f), sizes[i]),
			modelRenderer
			);

		modelEntity->AddComponent(new PhysicsComponent(true, PhysicsSystem::GRAVITY_VALUE));
		modelEntity->AddComponent(new CollisionComponent());
		//modelEntity->AddComponent(new BillboardComponent(mGameplayCamera));

		//AddBoundingBoxesFrom(modelEntity);
		mEngine.AddGameEntity(modelEntity);
	}
}

GameEntity* CreateModelWithLod(const glm::vec3& position, const glm::vec3& scale, const std::vector<std::string>& models, const std::vector<float>& distances, Texture* texture, Texture* normal)
{
	GameEntity* modelEntity = new GameEntity(
		new Transformation(position, glm::vec3(0.0f), scale),
		nullptr
		);

	modelEntity->AddComponent(new PhysicsComponent(true, PhysicsSystem::GRAVITY_VALUE));
	modelEntity->AddComponent(new CollisionComponent());

	LODComponent* lodComponent = new LODComponent(mGameplayCamera);
	modelEntity->AddComponent(lodComponent);
	//WARNING!!! debemos a�adir el componente a la entity primero para que se le asigne el parent, a partir de ah�, agregar los dem�s lod que ya les asignar� el padre.

	for (unsigned int i = 0; i < models.size(); ++i)
	{
		IRenderer* renderer = nullptr;
		if (i == 0 && normal != nullptr)
		{
			renderer = new ModelNormalMapRenderer(mEngine.GetModel(models[i]),
				mEngine.GetShader("normalmap"),
				texture,
				normal,
				mSunLight
				);
		}
		else
		{
			renderer = new ModelRenderer(mEngine.GetModel(models[i]),
				mEngine.GetShader("model"),
				texture,
				mSunLight
				);
		}
		renderer->SetFogParameters(mFogColor, mFogDensity, mFogGradient);
		lodComponent->AddLevelOfDetail(renderer, distances[i]);
	}

	return modelEntity;
}

GameEntity* CreateModel(const glm::vec3& position, const glm::vec3& scale, const std::string& model, Texture* texture, Texture* normal)
{
	GameEntity* modelEntity = new GameEntity(
												new Transformation(position, glm::vec3(0.0f), scale),
												nullptr
											);

	modelEntity->AddComponent(new PhysicsComponent(true, PhysicsSystem::GRAVITY_VALUE));
	modelEntity->AddComponent(new CollisionComponent());

	IRenderer* renderer = nullptr;
	if (normal != nullptr)
	{
		renderer = new ModelNormalMapRenderer(	mEngine.GetModel(model),
												mEngine.GetShader("normalmap"),
												texture,
												normal,
												mSunLight
											);
	}
	else
	{
		renderer = new ModelRenderer(	mEngine.GetModel(model),
										mEngine.GetShader("model"),
										texture,
										mSunLight
									);
	}
	
	renderer->SetFogParameters(mFogColor, mFogDensity, mFogGradient);
	modelEntity->SetRenderer(renderer);

	return modelEntity;
}

void CreateTrees()
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> sizes;

	int areaSize = 50;
	int numTrees = 200;
	for (int i = 0; i < numTrees; i++)
	{
		float x = static_cast<float>(-areaSize / 2 + 2 * rand() % areaSize);
		float z = static_cast<float>(-areaSize / 2 + 2 * rand() % areaSize);
		float height = mTerrain->GetHeight(glm::vec2(x, z));
		if (height > mWaterHeight + 0.2f)
		{
			positions.push_back(glm::vec3(x, height, z));
			float scale = .5f;// (rand() % 5) / 200.0f + 0.02f;
			sizes.push_back(glm::vec3(scale));
		}
	}

	//positions.push_back(glm::vec3(6.4f, 0.0f, 3.3f));	
	//sizes.push_back(glm::vec3(0.2f, 1.2f, 0.2f));
	std::vector<std::string> modelsFoliage;
	modelsFoliage.push_back("tree_foliage_0");
	modelsFoliage.push_back("tree_foliage_1");
	modelsFoliage.push_back("tree_foliage_2");

	std::vector<float> distances;
	distances.push_back(100.0f);
	distances.push_back(500.0f);
	distances.push_back(1000.0f);

	std::vector<std::string> modelsTrunk;
	modelsTrunk.push_back("tree_trunk_0");
	modelsTrunk.push_back("tree_trunk_1");
	modelsTrunk.push_back("tree_trunk_2");
	
	for (unsigned long i = 0; i < positions.size(); i++)
	{
		GameEntity* entity = CreateModelWithLod(	positions[i], sizes[i], modelsFoliage, distances, 
													static_cast<Texture*>(mEngine.GetTexture("tree_foliage_diffuse")),
													nullptr/*static_cast<Texture*>(mEngine.GetTexture("tree_foliage_normalmap"))*/);
		mEngine.AddGameEntity(entity);
		
		entity = CreateModelWithLod(	positions[i], sizes[i], modelsTrunk, distances, 
										static_cast<Texture*>(mEngine.GetTexture("tree_trunk_diffuse")),
										static_cast<Texture*>(mEngine.GetTexture("tree_trunk_normalmap")));
		mEngine.AddGameEntity(entity);
	}
}

void CreateProps()
{
	int areaSize = 5;
	int numProps = 5;

	std::vector<std::string> models;
	std::vector<glm::vec3> positions;

	models.push_back(std::string("barrel"));
	models.push_back(std::string("chest"));
	models.push_back(std::string("brazier"));
	models.push_back(std::string("stall"));
	models.push_back(std::string("cube2"));

	positions.push_back(glm::vec3(0.8f, 0.0f, -2.3f));
	positions.push_back(glm::vec3(0.4f, 0.0f, -2.0f));
	positions.push_back(glm::vec3(1.0f, 0.0f, -1.7f));
	positions.push_back(glm::vec3(10.0f, 0.0f, 10.0f));
	positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

	for (int i = 0; i < numProps; i++)
	{
		float x = static_cast<float>(-areaSize / 2 + 2 * rand() % areaSize);
		float z = static_cast<float>(-areaSize / 2 + 2 * rand() % areaSize);

		x = positions[i % models.size()].x;
		z = positions[i % models.size()].z;

		float height = mTerrain->GetHeight(glm::vec2(x, z))-0.1f;
		//if (height > mWaterHeight + 0.2f)
		{
			glm::vec3 position(x, height, z);
			glm::vec3 scale(0.3f);
			std::string model = models[i % models.size()];
			std::string textureName("MedievalDungeonPropsAtlas02_diffuse");
			std::string textureNormalName("MedievalDungeonPropsAtlas02_normalmap");

			if (model.compare("stall") == 0)
			{
				textureName = "stall";
				textureNormalName = "";
				scale = glm::vec3(0.1f);
			}
			else if (model.compare("cube2") == 0)
			{
				textureName = "cube_diffuse";
				textureNormalName = "";
				scale = glm::vec3(0.005f);
			}
			Texture* texture = static_cast<Texture*>(mEngine.GetTexture(textureName));
			Texture* normal = static_cast<Texture*>(mEngine.GetTexture(textureNormalName));

			GameEntity* entity = CreateModel(position, scale, model, texture, normal);
			mEngine.AddGameEntity(entity);
		}
	}
}

Particle* CreateParticle(bool canCollide, Texture* texture, glm::vec3& gravity)
{
	ParticleRenderer* renderer = new ParticleRenderer(mEngine.GetShader("particle"), texture, static_cast<Texture*>(mEngine.GetTexture("depth_texture")), 1.0f, 1.0f);
	renderer->SetLayer(IRenderer::LAYER_PARTICLES);
	renderer->SetTransparency(true);
	renderer->SetBillboard(true);
	renderer->SetFogParameters(mFogColor, mFogDensity, mFogGradient);

	Particle* particle = new Particle(	new Transformation(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.1f)),
										renderer,
										6.0f);
	PhysicsComponent* physicsComponent = new PhysicsComponent(false, gravity);
	particle->AddComponent(physicsComponent);

	if (canCollide)
	{
		particle->AddComponent(new CollisionComponent());
	}

	return particle;
}

void CreateParticlesFire()
{
	Particle* particle = CreateParticle(false, static_cast<Texture*>(mEngine.GetTexture("smoke")), glm::vec3(0.0f));
	particle->SetLiveTime(2.0f);
	
	float x = 1.0f;
	float z = -1.7f;

	float height = mTerrain->GetHeight(glm::vec2(x, z)) + 0.28f;

	ParticlesEmitter* particlesEmitter = new ParticlesEmitter(particle,
		new Transformation(glm::vec3(x, height, z), glm::vec3(0.0f), glm::vec3(0.1f)),
		nullptr,
		100);
	particlesEmitter->SetColorGradientValues(glm::vec4(1.0f, 1.0f, 0.25f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	particlesEmitter->SetScaleValues(0.03f, 0.005f);
	particlesEmitter->SetVelocity(glm::vec3(0.0f), glm::vec3(0.02f, 0.2f, 0.02f));
	particlesEmitter->SetSpawnArea(glm::vec3(-0.02f, 0.0f, -0.02f), glm::vec3(0.03f, 0.0f, 0.03f));
	mEngine.AddGameEntity(particlesEmitter);
	mEngine.AddParticleEmitter(particlesEmitter);
}

void CreateParticlesSparkles()
{
	Particle* particle = CreateParticle(false, static_cast<Texture*>(mEngine.GetTexture("sparkle")), PhysicsSystem::GRAVITY_VALUE * 0.1f);
	particle->SetLiveTime(0.5f);

	ParticlesEmitter* particlesEmitter = new ParticlesEmitter(particle,
		new Transformation(glm::vec3(0.0f, 4.7f, 0.0f), glm::vec3(0.0f), glm::vec3(0.1f)),
		nullptr,
		10);
	particlesEmitter->SetColorGradientValues(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	particlesEmitter->SetScaleValues(0.003f, 0.001f);
	particlesEmitter->SetVelocity(glm::vec3(-0.7f), glm::vec3(0.7f));
	particlesEmitter->SetSpawnArea(glm::vec3(0.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	mEngine.AddGameEntity(particlesEmitter);
	mEngine.AddParticleEmitter(particlesEmitter);
}

void CreateParticlesTest()
{
	Particle* particle = CreateParticle(false, static_cast<Texture*>(mEngine.GetTexture("smoke")), glm::vec3(0.0f));
	particle->SetLiveTime(1.0f);

	glm::vec3 position(5.5f, 0.0f, 3.3f); 
	float height = mTerrain->GetHeight(glm::vec2(position.x, position.z)) + 0.01f;
	position.y = height;
	

	ParticlesEmitter* particlesEmitter = new ParticlesEmitter(	particle,
																new Transformation(position, glm::vec3(0.0f), glm::vec3(0.1f)),
																nullptr,
																20);
	particlesEmitter->SetColorGradientValues(glm::vec4(0.8f, 0.0f, 0.0f, 1.0f), glm::vec4(0.6f, 0.0f, 0.6f, 0.0f));
	particlesEmitter->SetScaleValues(0.03f, 0.4f + (rand() % 4) / 10.0f);
	particlesEmitter->SetVelocity(glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f));
	mEngine.AddGameEntity(particlesEmitter);
	mEngine.AddParticleEmitter(particlesEmitter);	

	particle = CreateParticle(false, static_cast<Texture*>(mEngine.GetTexture("smoke")), glm::vec3(0.0f));
	particle->SetLiveTime(1.0f);

	position = glm::vec3(5.0f, 0.0f, 3.3f);
	height = mTerrain->GetHeight(glm::vec2(position.x, position.z)) + 0.01f;
	position.y = height;
	particlesEmitter = new ParticlesEmitter(	particle,
												new Transformation(position, glm::vec3(0.0f), glm::vec3(0.1f)),
												nullptr,
												20);
	particlesEmitter->SetColorGradientValues(glm::vec4(0.0f, 0.8f, 0.0f, 1.0f), glm::vec4(0.0f, 0.6f, 0.6f, 0.0f));
	particlesEmitter->SetScaleValues(0.03f, 0.4f + (rand() % 4) / 10.0f);
	particlesEmitter->SetVelocity(glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f));
	mEngine.AddGameEntity(particlesEmitter);
	mEngine.AddParticleEmitter(particlesEmitter);
}

void CreateEnergyWall()
{
	EnergyWallRenderer* renderer = new EnergyWallRenderer(	mEngine.GetModel("sphere"), 
															mEngine.GetShader("energy_wall"),
															static_cast<Texture*>(mEngine.GetTexture("yellow_grid")),
															static_cast<Texture*>(mEngine.GetTexture("depth_texture")));
	
	renderer->SetLayer(IRenderer::LAYER_PARTICLES);
	renderer->SetTransparency(true);
	renderer->SetTile(50.0f);
	//renderer->SetVisibility(false);

	mEnergyWall = new EnergyWall(new Transformation(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(mEnergyWallRadius)),
									renderer, 2.0f);
	//mEnergyWall->SetEnabled(false);
	mEngine.AddGameEntity(mEnergyWall);
	mEngine.SetEnergyWallRadius(mEnergyWallRadius);
}

void CreateTextTest()
{
	mFPSText = new Text(	new Transformation(
												glm::vec3(-mEngine.GetScreenWidth() * 0.5f, mEngine.GetScreenHeight() * 0.5f, 0.0f), 
												glm::vec3(0.0f), 
												glm::vec3(0.70f)
							),
							mEngine.GetShader("text"), mEngine.GetFont("OCR A Extended"),
							"FPS:", false, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, false);
	mEngine.AddGameEntity(mFPSText);

	float x = 0.0f;
	float z = 0.0f;
	float height = mTerrain->GetHeight(glm::vec2(x, z)) + 1.0f;


	Text* mTestText = new Text(new Transformation(glm::vec3(x, height, z), glm::vec3(0.0f), glm::vec3(.01f)),
		mEngine.GetShader("text"), mEngine.GetFont("OCR A Extended"),
		"Origin", true, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 1, 1, false);
	mTestText->SetOutlineColor(glm::vec4(0.0f, 1.0f, 1.0f, 0.8f));
	mTestText->SetBorderParameters(0.4f, 0.1f, 0.3f, 0.7f);
	mEngine.AddGameEntity(mTestText);

	x = 10.0f;
	z = 10.0f;
	height = mTerrain->GetHeight(glm::vec2(x, z)) + 1.0f;

	mTestText = new Text(new Transformation(glm::vec3(x, height, z), glm::vec3(0.0f), glm::vec3(.01f)),
		mEngine.GetShader("text"), mEngine.GetFont("OCR A Extended"),
		"Market", true, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 1, 1, false);
	mTestText->SetOutlineColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
	mTestText->SetBorderParameters(0.5f, 0.1f, 0.5f, 0.4f);
	mTestText->SetShadow(glm::vec2(0.002f, 0.002f));
	mEngine.AddGameEntity(mTestText);

	/*
	text = new Text(	new Transformation(glm::vec3(-512.0f, 1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(2.0f)),
						mEngine.GetShader("text"), mEngine.GetFont("OCR A Extended"),
						"Texto de prueba", false, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 1, 1, false);
	text->SetOutlineColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
	text->SetBorderParameters(0.5f, 0.1f, 0.0f, 0.4f);
	text->SetShadow(glm::vec2(0.002f, 0.002f));
	mEngine.AddGameEntity(text);

	text = new Text(new Transformation(glm::vec3(-512.0f, -100.0f, 0.0f), glm::vec3(0.0f), glm::vec3(2.0f)),
		mEngine.GetShader("text"), mEngine.GetFont("OCR A Extended Field"),
		"Texto de prueba", false, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 1, 1, false);
	
	mEngine.AddGameEntity(text);*/
}

void CreateEntities()
{

}

void CreateEntities2()
{
	const Texture* texture = static_cast<const Texture*>(mEngine.CreateDepthTexture("depth_texture", glm::vec2(mEngine.GetScreenWidth(), mEngine.GetScreenHeight())));

	//LIGHT GAME ENTITY
	mSunLight = new Light(mSunLightDirection, glm::vec3(1, 1, 1), nullptr);// new CubeRenderer(mEngine.GetShader("default")));
	mSunLight->AddComponent(new VerticalInputComponent(mEngine.GetGLWindow()));
	mSunLight->GetTransformation()->SetScale(glm::vec3(0.05f));

	mEngine.AddGameEntity(mSunLight);

	//TERRAIN GAME ENTITY
	//mSunCamera = new OrthogonalCamera(20.0f, 20.0f, -10.0f, 20.0f);
	//glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	//glm::vec3 direction = glm::vec3(camera->GetTarget() - glm::vec3(100000.0f, 100000.0f, 100000.0f));
	//glm::mat4 depthViewMatrix = glm::lookAt(direction, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	//glm::mat4 depthModelMatrix = glm::mat4(1.0);
	//glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	terrainHeightScale = mIsTerrainFlat ? 0.0f : terrainHeightScale;
	mTerrain = new Terrain(new Transformation(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
								  mEngine.GetShader("terrain"),
								  static_cast<Texture*>(mEngine.GetTexture("terrain_heightmap")),
								  static_cast<Texture*>(mEngine.GetTexture("terrain_blendmap")),
								  static_cast<TextureArray*>(mEngine.GetTexture("terrain_array")),
								  static_cast<Texture*>(mEngine.GetTexture("shadow_texture")),
								  mSunLight,
								  terrainHeightScale);

	mTerrain->AddComponent(new DebugInputComponent(mEngine.GetGLWindow()));
	mTerrain->SetFlat(mIsTerrainFlat);

	TerrainRenderer* renderer = static_cast<TerrainRenderer*>(mTerrain->GetRenderer());
	renderer->SetFogParameters(mFogColor, mFogDensity, mFogGradient);
	//AddBoundingBoxesFrom(mTerrain);

	mEngine.AddGameEntity(mTerrain);

	mEngine.SetTerrain(mTerrain);
	
	//TERRAIN NORMALS ENTITY
	//CreateTerrainNormals(vertexs, numVertexsSide);

	//WATER
	if (mIsWaterEnabled)
	{
		WaterRenderer* waterRenderer = new WaterRenderer(	mEngine.GetShader("water"),
															static_cast<Texture*>(mEngine.GetTexture("reflection_water")),
															static_cast<Texture*>(mEngine.GetTexture("refraction_water")),
															static_cast<Texture*>(mEngine.GetTexture("distorsion_water")),
															static_cast<Texture*>(mEngine.GetTexture("normal_water")),
															static_cast<Texture*>(mEngine.GetTexture("refraction_depth_water")),
															50.0f, //8.0f,
															50.0f,//11.0f,
															0.0005f,
															glm::vec4(0.0f, 0.3f, 0.8f, 0.0f),
															mSunLight
														);
		mWater = new GameEntity(	new Transformation(glm::vec3(4.0f, mWaterHeight, 4.5f), glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), glm::vec3(1.0f)),
									waterRenderer
								);
		waterRenderer->SetLayer(IRenderer::LAYER_WATER);
		waterRenderer->SetFogParameters(mFogColor, mFogDensity, mFogGradient);
		mEngine.AddGameEntity(mWater);
	}
	//PLAYER
	ModelNormalMapRenderer* modelRenderer = new ModelNormalMapRenderer(mEngine.GetModel("enano"),
																		mEngine.GetShader("normalmap"),
																		static_cast<Texture*>(mEngine.GetTexture("enano_diffuse")),
																		nullptr,
																		mSunLight
																	);
	/*
	ModelRenderer* modelRenderer = new ModelRenderer(mEngine.GetModel("barrel"),
		mEngine.GetShader("model"),
		static_cast<Texture*>(mEngine.GetTexture("MedievalDungeonPropsAtlas02_diffuse")),
		mSunLight
	);*/
	modelRenderer->SetFogParameters(mFogColor, mFogDensity, mFogGradient);
	modelRenderer->SetTile(4.0f);

	mPlayer = new Player(	new Transformation(glm::vec3(0.0f, 4.9f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.03f)),
							modelRenderer,
							new PlayerInputComponent(mEngine.GetGLWindow(), PLAYER_RUN_SPEED, PLAYER_TURN_SPEED, PLAYER_UPWARDS_HEIGHT),
							new PhysicsComponent(false, PhysicsSystem::GRAVITY_VALUE),
							new CollisionComponent()
						);
	mPlayer->AddComponent(new EnergyWallCollisionComponent());
	//mPlayer->AddComponent(new RotationComponent(glm::vec3(0.0f, 1.0f, 0.0f), 30.0f));
	//AddBoundingBoxesFrom(mPlayer);

	mEngine.AddGameEntity(mPlayer);

	//CAMERA
	mEagleEyeCamera = new PerspectiveCamera(VIEW_ANGLE, mEngine.GetScreenWidth()/mEngine.GetScreenHeight(), NEAR_PLANE, FAR_PLANE);
	mEagleEyeCamera->SetPosition(glm::vec3(0.0f, 15.0f, 15.0f));
	mEagleEyeCamera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	mEagleEyeCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));

	mGameplayCamera = new PerspectiveCamera(VIEW_ANGLE, mEngine.GetScreenWidth() / mEngine.GetScreenHeight(), NEAR_PLANE, FAR_PLANE);
	mGameplayCamera->SetPosition(glm::vec3(0.0f, 4.9f, 3.0f));
	mGameplayCamera->SetTarget(glm::vec3(0.0f, 4.9f, 0.0f));
	mGameplayCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));

	mCamera = new GameEntity(	new Transformation(mGameplayCamera->GetPosition(), glm::vec3(0.0f), glm::vec3(0.0f)),
		nullptr);// new CubeRenderer(mEngine.GetShader("default")));
	mCamera->AddComponent(new ThirdPersonCameraComponent(static_cast<PerspectiveCamera*>(mGameplayCamera), mPlayer, 1.5f, 10.0f));
	mCamera->AddComponent(new CollisionComponent());
	if (mIsWaterEnabled)
	{
		mCamera->AddComponent(new OverWaterComponent(mWaterHeight));
	}

	mEngine.AddGameEntity(mCamera);

	//CreateSpecificCubes();

	//CreateHUD();

	//CreateWaterHudPlanes();
	
	
	if (mIsParticlesEnabled)
	{
		//CreateParticlesTest();
		CreateParticlesFire();
		//CreateParticlesSparkles();
	}
	

	if (mIsTextEnabled)
	{
		CreateTextTest();
	}
	
	if (mIsVegetationEnabled)
	{
		CreateTrees();
	}

	if (mIsPropsEnabled)
	{
		CreateProps();
	}

	if (mIsEnergyWallEnabled)
	{
		CreateEnergyWall();
	}

	if (mIsShadowEnabled)
	{
		//CreateShadowPlane();
	}

	//SKYBOX the last
	if (mIsSkyboxEnabled)
	{
		SkyBoxRenderer* skyboxRenderer = new SkyBoxRenderer(mEngine.GetShader("skybox"),
			static_cast<TextureCubemap*>(mEngine.GetTexture("cubemap")),
			1.0f);
		skyboxRenderer->SetLayer(IRenderer::LAYER_PARTICLES);
		skyboxRenderer->SetFogColor(mFogColor);

		GameEntity* skyBox = new GameEntity(
			new Transformation(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
			skyboxRenderer
			);
		skyBox->AddComponent(new RotationComponent(glm::vec3(0.0f, 1.0f, 0.0f), SKYBOX_ROTATION_SPEED));
		mEngine.AddGameEntity(skyBox);
	}
}

void DisableRenderPasses()
{
	if (mMapPass != nullptr)
	{
		mMapPass->SetEnable(false);
	}
}

void ApplyReflectionCameras(float yReflectionPlane, ICamera* camera, ICamera* cameraReflected)
{
	float distance = 2 * (camera->GetPosition().y - yReflectionPlane);
	glm::vec3 position = camera->GetPosition();
	position.y -= distance;
	cameraReflected->SetPosition(position);

	distance = 2 * (camera->GetTarget().y - yReflectionPlane);
	glm::vec3 target = camera->GetTarget();
	target.y -= distance;
	cameraReflected->SetTarget(target);
	cameraReflected->SetUp(camera->GetUp());
}

void ApplyRefractionCameras(ICamera* camera, ICamera* cameraRefracted)
{
	cameraRefracted->SetPosition(camera->GetPosition());
	cameraRefracted->SetTarget(camera->GetTarget());
	cameraRefracted->SetUp(camera->GetUp());
}

void CreateHudMapRenderPass()
{
	//HUD MAP RENDER PASS
	mMapCamera = new PerspectiveCamera(VIEW_ANGLE, mEngine.GetScreenWidth() / mEngine.GetScreenHeight(), NEAR_PLANE, FAR_PLANE);
	mMapCamera->SetPosition(glm::vec3(0.0f, 100.0f, 0.0f));
	mMapCamera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	mMapCamera->SetUp(glm::vec3(1.0f, 0.0f, 0.0f));
	mMapPass = new RenderPass(static_cast<ICamera*>(mMapCamera), IRenderer::LAYER_OTHER);
	
	IFrameBuffer* frameBuffer = new IFrameBuffer(static_cast<int>(mEngine.GetScreenWidth()), static_cast<int>(mEngine.GetScreenHeight()));
	frameBuffer->SetColorTextureAttachment(0, static_cast<Texture*>(mEngine.GetTexture("map")));
	frameBuffer->Init();
	mMapPass->SetFrameBufferOutput(frameBuffer);
	mMapPass->EnableFog(false);
	mEngine.AddRenderPass(mMapPass);
}

void CreateWaterRenderPass()
{
	float waterY = mWater->GetTransformation()->GetPosition().y;

	//WATER RENDER PASS	
	mReflectionWaterCamera = new PerspectiveCamera(VIEW_ANGLE, mEngine.GetScreenWidth() / mEngine.GetScreenHeight(), NEAR_PLANE, FAR_PLANE);
	ApplyReflectionCameras(waterY, mGameplayCamera, mReflectionWaterCamera);
	
	mWaterReflectionCameraEntity = new GameEntity(new Transformation(mReflectionWaterCamera->GetPosition(), glm::vec3(0.0f), glm::vec3(0.0f)),
		nullptr);// new CubeRenderer(mEngine.GetShader("default")));
	mEngine.AddGameEntity(mWaterReflectionCameraEntity);

	//REFLECTION
	IFrameBuffer* frameReflectionBuffer = new IFrameBuffer(static_cast<int>(mEngine.GetScreenWidth()), static_cast<int>(mEngine.GetScreenHeight()));
	Texture* reflectionTexture = static_cast<Texture*>(mEngine.GetTexture("reflection_water"));
	frameReflectionBuffer->SetColorTextureAttachment(0, reflectionTexture);
	frameReflectionBuffer->SetDepthAttachment(reflectionTexture->GetWidth(), reflectionTexture->GetHeight());

	frameReflectionBuffer->Init();
	
	RenderPass* reflectionWaterPass = new RenderPass(static_cast<ICamera*>(mReflectionWaterCamera), IRenderer::LAYER_TERRAIN | IRenderer::LAYER_OTHER | IRenderer::LAYER_PARTICLES);
	reflectionWaterPass->SetFrameBufferOutput(frameReflectionBuffer);
	reflectionWaterPass->EnableClipping(true);
	reflectionWaterPass->SetClippingPlaneNumber(GL_CLIP_DISTANCE0);
	reflectionWaterPass->SetClippingPlane(glm::vec4(0.0f, 1.0f, 0.0f, -waterY+0.2));
	reflectionWaterPass->EnableFog(true);

	mEngine.AddRenderPass(reflectionWaterPass);

	//REFRACTION	
	mRefractionWaterCamera = new PerspectiveCamera(VIEW_ANGLE, mEngine.GetScreenWidth() / mEngine.GetScreenHeight(), NEAR_PLANE, FAR_PLANE);
	ApplyRefractionCameras(mGameplayCamera, mRefractionWaterCamera);

	mWaterRefractionCameraEntity = new GameEntity(	new Transformation(mRefractionWaterCamera->GetPosition(), glm::vec3(0.0f), glm::vec3(0.0f)),
		nullptr);// new CubeRenderer(mEngine.GetShader("default")));
	mEngine.AddGameEntity(mWaterRefractionCameraEntity);

	IFrameBuffer* frameRefractionBuffer = new IFrameBuffer(static_cast<int>(mEngine.GetScreenWidth()), static_cast<int>(mEngine.GetScreenHeight()));
	Texture* refractionTexture = static_cast<Texture*>(mEngine.GetTexture("refraction_water"));
	Texture* refractionDepthTexture = static_cast<Texture*>(mEngine.GetTexture("refraction_depth_water"));
	frameRefractionBuffer->SetColorTextureAttachment(0, refractionTexture);
	frameRefractionBuffer->SetDepthTextureAttachment(refractionDepthTexture);
	frameRefractionBuffer->Init();

	RenderPass* refractionWaterPass = new RenderPass(static_cast<ICamera*>(mRefractionWaterCamera), IRenderer::LAYER_TERRAIN | IRenderer::LAYER_OTHER);
	refractionWaterPass->SetFrameBufferOutput(frameRefractionBuffer);
	refractionWaterPass->EnableClipping(true);
	refractionWaterPass->SetClippingPlaneNumber(GL_CLIP_DISTANCE0);
	//we need to add +2.0 to the water height, because when applying distorsion to the texture can get points 
	//out the refraction area drawed, and these points are blue (the color of the water plane), getting 
	//a bit more of the height, cutting over the water, we are painting more space in the water plane and then
	//the distorsion is not affecting, otherwise the border of the water has a bit of blue color (the default color of the plane)
	refractionWaterPass->SetClippingPlane(glm::vec4(0.0f, -1.0f, 0.0f, waterY + 2.0f));
	refractionWaterPass->EnableFog(false);

	mEngine.AddRenderPass(refractionWaterPass);
}

void CreateGUIRenderPass()
{
	//RENDER PASS GUI
	mGuiCamera = new OrthogonalCamera(mEngine.GetScreenWidth(), mEngine.GetScreenHeight(), NEAR_PLANE, FAR_PLANE);
	mGuiCamera->SetPosition(glm::vec3(0.0f, 0.0f, 40.0f));
	mGuiCamera->SetTarget(glm::vec3(0.0f, 0.0f, -50.0f));
	mGuiCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
	RenderPass *guiPass = new RenderPass(static_cast<ICamera*>(mGuiCamera), IRenderer::LAYER_GUI);
	mEngine.AddRenderPass(guiPass);
}

void CreateGameplayRenderPass()
{
	//RENDER PASS GAMEPLAY	
	RenderPass *gameplayPass = new RenderPass(static_cast<ICamera*>(mGameplayCamera), IRenderer::LAYER_OTHER | IRenderer::LAYER_WATER | IRenderer::LAYER_DEBUG);
	
	IFrameBuffer* frameBuffer = new IFrameBuffer(static_cast<int>(mEngine.GetScreenWidth()), static_cast<int>(mEngine.GetScreenHeight()));
	Texture* depthTexture = static_cast<Texture*>(mEngine.GetTexture("depth_texture"));
	frameBuffer->SetCopyDepthBufferToTexture(depthTexture, 0, 0, static_cast<int>(mEngine.GetScreenWidth()), static_cast<int>(mEngine.GetScreenHeight()));
	gameplayPass->SetFrameBufferOutput(frameBuffer);
	
	//IRenderer* renderer = new QuadRenderer(mEngine.GetShader("quad"), static_cast<Texture*>(mEngine.GetTexture("terrain_blendmap")), 10.0f, 10.0f);
	//gameplayPass->SetRenderer(renderer);
	mEngine.AddRenderPass(gameplayPass);
}

void CreateTerrainRenderPass()
{
	//RENDER PASS GAMEPLAY
	RenderPass *terrainPass = new RenderPass(static_cast<ICamera*>(mGameplayCamera), IRenderer::LAYER_TERRAIN);
	mEngine.AddRenderPass(terrainPass);
}

void CreateParticlesRenderPass()
{
	//RENDER PASS PARTICLES
	RenderPass *particlesPass = new RenderPass(static_cast<ICamera*>(mGameplayCamera), IRenderer::LAYER_PARTICLES);
	particlesPass->SetCalculateDistanceToCamera(true);
	mEngine.AddRenderPass(particlesPass);
}

void CreateRenderPasses()
{
	//CreateHudMapRenderPass();

	if (mIsWaterEnabled)
	{
		CreateWaterRenderPass();
	}

	CreateTerrainRenderPass();

	CreateGameplayRenderPass();

	CreateParticlesRenderPass();

	CreateGUIRenderPass();
}

void DeleteEntities()
{
	delete mEagleEyeCamera;
}

void UpdateInput(GLFWwindow* window)
{
	/*if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	mCamera->Move(200 * deltaTime);

	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
	mCamera->Move(-200 * deltaTime);*/

	//cameraDirection = glm::vec3(glm::rotate(glm::mat4(1.0f), -0.5f*delta * 180.0f / Pi, upVector)*glm::vec4(cameraDirection, 1.0));

	/*if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
	glm::vec3 orto = glm::cross(upVector, cameraDirection);
	cameraDirection = glm::vec3(glm::rotate(glm::mat4(1.0f), 0.5f*delta * 180.0f / Pi, orto)*glm::vec4(cameraDirection, 1.0));
	upVector = glm::vec3(glm::rotate(glm::mat4(1.0f), 0.5f*delta * 180.0f / Pi, orto)*glm::vec4(upVector, 1.0));
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
	glm::vec3 orto = glm::cross(upVector, cameraDirection);
	cameraDirection = glm::vec3(glm::rotate(glm::mat4(1.0f), -0.5f*delta * 180.0f / Pi, orto)*glm::vec4(cameraDirection, 1.0));
	upVector = glm::vec3(glm::rotate(glm::mat4(1.0f), -0.5f*delta * 180.0f / Pi, orto)*glm::vec4(upVector, 1.0));
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	upVector = glm::vec3(glm::rotate(glm::mat4(1.0f), -0.5f*delta * 180.0f / Pi, cameraDirection)*glm::vec4(upVector, 1.0));
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	upVector = glm::vec3(glm::rotate(glm::mat4(1.0f), 0.5f*delta * 180.0f / Pi, cameraDirection)*glm::vec4(upVector, 1.0));
	*/

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		PerspectiveCamera* gameplayCamera = static_cast<PerspectiveCamera*>(mGameplayCamera);
		PerspectiveCamera* eagleEyeCamera = static_cast<PerspectiveCamera*>(mEagleEyeCamera);
		std::swap(*gameplayCamera, *eagleEyeCamera);
		if (mIsGameplayCameraEnabled)
		{
			mCamera->RemoveComponent<ThirdPersonCameraComponent>();
		}
		else
		{
			mCamera->AddComponent(new ThirdPersonCameraComponent(static_cast<PerspectiveCamera*>(mGameplayCamera), mPlayer, 2.0f, 10.0f));
		}

		mIsGameplayCameraEnabled = !mIsGameplayCameraEnabled;
	}
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		mIsShadowEnabled = !mIsShadowEnabled;
		mEngine.SetCastingShadowsEnabled(mIsShadowEnabled);
	}

}

void UpdateWaterCameras()
{
	ApplyReflectionCameras(mWater->GetTransformation()->GetPosition().y, mGameplayCamera, mReflectionWaterCamera);
	mWaterReflectionCameraEntity->GetTransformation()->SetPosition(mReflectionWaterCamera->GetPosition());
	ApplyRefractionCameras(mGameplayCamera, mRefractionWaterCamera);
	mWaterRefractionCameraEntity->GetTransformation()->SetPosition(mGameplayCamera->GetPosition());
}

void UpdateEnergyWallCollisions(float elapsedTime)
{
	EnergyWallCollisionComponent* component = mPlayer->GetComponent<EnergyWallCollisionComponent>();
	if (component->IsColliding())
	{
		glm::vec3 collisionPoint = component->GetCollisionPoint();
		mEnergyWall->SetContactPoint(collisionPoint);
		mEnergyWall->SetLiveTime(mEnergyWall->GetMaxLiveTime());
		mEnergyWall->GetRenderer()->SetVisibility(true);
	}
	else if (!mEnergyWall->IsAlive())
	{
		mEnergyWall->SetContactPoint(glm::vec3(0.0f));
		mEnergyWall->GetRenderer()->SetVisibility(false);
	}
}

void UpdateStatitstics()
{
	int fps = static_cast<int>(mEngine.GetFPS());
	if (fps < MIN_FPS_ALLOWED)
	{
		mFPSText->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		mFPSText->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	mFPSText->UpdateText("FPS: " +std::to_string(fps));

	//mTestText->UpdateText("3D text " + std::to_string(fps));
}

void Update(float elapsedTime)
{
	if (mIsWaterEnabled)
	{
		UpdateWaterCameras();
	}
	if (mIsEnergyWallEnabled)
	{
		UpdateEnergyWallCollisions(elapsedTime);
	}
	if (mIsStatisticsVisible && mIsTextEnabled)
	{
		UpdateStatitstics();
	}
	if (mIsShadowEnabled)
	{
		mEngine.SetCastingShadowsTarget(mPlayer->GetTransformation()->GetPosition());
	}
}

void SetupConfiguration()
{
	switch (mConfiguration)
	{
	case DEBUG:
		mIsDebugModeEnabled = true;
		mIsWaterEnabled = false;
		mIsGameplayCameraEnabled = true;
		mIsFogEnabled = true;
		mIsVegetationEnabled = true;
		mIsPropsEnabled = true;
		mIsEnergyWallEnabled = false;
		mIsSkyboxEnabled = true;
		mIsTerrainFlat = true;
		mIsTextEnabled = true;
		mIsStatisticsVisible = true;
		mIsParticlesEnabled = true;
		mIsShadowEnabled = true;
		break;
	case SHADOWS:
		mIsDebugModeEnabled = true;
		mIsWaterEnabled = false;
		mIsGameplayCameraEnabled = true;
		mIsFogEnabled = false;
		mIsVegetationEnabled = false;
		mIsPropsEnabled = false;
		mIsEnergyWallEnabled = false;
		mIsSkyboxEnabled = true;
		mIsTerrainFlat = false;
		mIsTextEnabled = true;
		mIsStatisticsVisible = true;
		mIsShadowEnabled = true;
		mIsParticlesEnabled = false;
		break;
	case TEXT:
		mIsDebugModeEnabled = true;
		mIsWaterEnabled = false;
		mIsGameplayCameraEnabled = true;
		mIsFogEnabled = false;
		mIsVegetationEnabled = false;
		mIsPropsEnabled = false;
		mIsEnergyWallEnabled = false;
		mIsSkyboxEnabled = false;
		mIsTerrainFlat = true;
		mIsTextEnabled = true;
		mIsStatisticsVisible = true;
		mIsParticlesEnabled = false;
		mIsShadowEnabled = false;
		break;
	case ENERGY_WALL:
		mIsDebugModeEnabled = true;
		mIsWaterEnabled = false;
		mIsGameplayCameraEnabled = true;
		mIsFogEnabled = false;
		mIsVegetationEnabled = false;
		mIsPropsEnabled = false;
		mIsEnergyWallEnabled = true;
		mIsSkyboxEnabled = true;
		mIsTerrainFlat = false;
		mIsTextEnabled = true;
		mIsStatisticsVisible = true;
		mEnergyWallRadius = 22.0f;
		mIsParticlesEnabled = false;
		mIsShadowEnabled = false;
		mIsFullScreen = false;
		break;
	case PARTICLES:
		mIsDebugModeEnabled = true;
		mIsWaterEnabled = false;
		mIsGameplayCameraEnabled = true;
		mIsFogEnabled = false;
		mIsVegetationEnabled = false;
		mIsPropsEnabled = false;
		mIsEnergyWallEnabled = false;
		mIsSkyboxEnabled = true;
		mIsTerrainFlat = true;
		mIsTextEnabled = true;
		mIsStatisticsVisible = true;
		mIsParticlesEnabled = true;
		mIsShadowEnabled = false;
		mIsFullScreen = false;
		break;
	case PROPS:
		mIsDebugModeEnabled = true;
		mIsWaterEnabled = false;
		mIsGameplayCameraEnabled = true;
		mIsFogEnabled = false;
		mIsVegetationEnabled = false;
		mIsPropsEnabled = false;
		mIsEnergyWallEnabled = false;
		mIsSkyboxEnabled = false;
		mIsTerrainFlat = true;
		mIsTextEnabled = false;
		mIsStatisticsVisible = true;
		mIsParticlesEnabled = false;
		mIsShadowEnabled = false;
		mIsFullScreen = false;
		break; 
	case REFACTOR:
		mIsDebugModeEnabled = true;
		mIsWaterEnabled = false;
		mIsGameplayCameraEnabled = true;
		mIsFogEnabled = false;
		mIsVegetationEnabled = false;
		mIsPropsEnabled = false;
		mIsEnergyWallEnabled = false;
		mIsSkyboxEnabled = false;
		mIsTerrainFlat = true;
		mIsTextEnabled = false;
		mIsStatisticsVisible = false;
		mIsParticlesEnabled = false;
		mIsShadowEnabled = false;
		break;
	case RELEASE:
		mIsDebugModeEnabled = false;
		mIsWaterEnabled = true;
		mIsGameplayCameraEnabled = true;
		mIsFogEnabled = true;
		mIsVegetationEnabled = true;
		mIsPropsEnabled = true;
		mIsEnergyWallEnabled = true;
		mIsSkyboxEnabled = true;
		mIsTerrainFlat = false;
		mIsTextEnabled = true;
		mIsStatisticsVisible = true;
		mIsParticlesEnabled = true;
		mIsShadowEnabled = true;
		mIsFullScreen = true;
		break;
	}
}

void Initialize()
{
	SetupConfiguration();
	mEngine.RegisterInputHandler(std::bind(&UpdateInput, std::placeholders::_1));
	mEngine.RegisterUpdateHandler(std::bind(&Update, std::placeholders::_1));

	mEngine.SetCastingShadowsParameters(mSunLightDirection, 3);
	mEngine.SetCastingShadowsEnabled(true);
	
	mEngine.Init(mIsFullScreen);
}

int main(void)
{
	Initialize();

	mFogDensity = mIsFogEnabled ? 0.04f : 0.0f;

	CreateEntities();
	CreateRenderPasses();

	mEngine.Update();

	DeleteEntities();

	return 0;
}


/*
float hL = 0.0f;
float hR = 0.0f;
float hD = 0.0f;
float hU = 0.0f;

if (j > 0)
{
d = j - 1 + i * numVertexsSide;
hL = data[4 * d + 3];
}
if (j < numVertexsSide)
{
d = j + 1 + i * numVertexsSide;
hR = data[4 * d + 3];
}
if (i > 0)
{
d = j + (i - 1)* numVertexsSide;
hD = data[4 * d + 3];
}

if (i < numVertexsSide)
{
d = j + (i + 1)* numVertexsSide;
hU = data[4 * d + 3];
}
//glm::vec3 normal = glm::vec3(data[4 * d] / 255.0f, data[4 * d + 1] / 255.0f, data[4 * d + 2] / 255.0f);
//normals.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
//normals.push_back(normalize(normal * 2.0f + glm::vec3(-1.0f)));
normals.push_back(normalize(glm::vec3(hL - hR, hD - hU, 2.0f)));
*/