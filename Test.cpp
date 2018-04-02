
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

#include "GameConstants.h"
#include "src/NGenius.h"
#include "src/resources/systems/PhysicsSystem.h"
#include "src/resources/systems/renderSystem/RenderSystem.h"

#include "src/TerrainGrid.h"

#include "src/resources/renderers/IRenderer.h"
#include "src/resources/renderers/VertexsRenderer.h"
#include "src/resources/renderers/IndexesRenderer.h"
#include "src/resources/renderers/SkyBoxRenderer.h"
#include "src/resources/renderers/WireframeRenderer.h"

#include "src/resources/systems/renderSystem/RenderPass.h"
#include "src/resources/systems/renderSystem/IFrameBuffer.h"

#include "src/resources/textures/PNGLoader.h"
#include "src/resources/textures/TextureGenerator.h"

#include "src/resources/shaders/IShaderProgram.h"

#include "src/resources/textures/Texture.h"
#include "src/resources/textures/TextureArray.h"
#include "src/resources/textures/TextureCubemap.h"

#include "src/resources/font/FontType.h"

#include "src/resources/models/Model.h"

#include "src/resources/materials/IMaterial.h"
#include "src/resources/materials/effects/MaterialEffectDiffuseTexture.h"
#include "src/resources/materials/effects/MaterialEffectNormalTexture.h"
#include "src/resources/materials/effects/MaterialEffectLightProperties.h"
#include "src/resources/materials/effects/MaterialEffectFogProperties.h"
#include "src/resources/materials/effects/MaterialEffectShadowProperties.h"
#include "src/resources/materials/effects/MaterialEffectHeightMapTexture.h"
#include "src/resources/materials/effects/MaterialEffectTextureArray.h"
#include "src/resources/materials/effects/MaterialEffectClippingPlane.h"
#include "src/resources/materials/effects/MaterialEffectTextureCubemap.h"
#include "src/resources/materials/effects/MaterialEffectText.h"
#include "src/resources/materials/effects/MaterialEffectFloat.h"
#include "src/resources/materials/effects/MaterialEffectWater.h"
#include "src/resources/materials/effects/MaterialEffectParticle.h"
#include "src/resources/materials/effects/MaterialEffectDepthTexture.h"
#include "src/resources/materials/effects/MaterialEffectFloat2.h"
#include "src/resources/materials/effects/MaterialEffectFloat3.h"

#include "src/resources/entities/Terrain.h"
#include "src/resources/entities/Player.h"
#include "src/resources/entities/Text.h"
#include "src/resources/entities/Water.h"
#include "src/resources/entities/Particle.h"
#include "src/resources/entities/ParticlesEmitter.h"
#include "src/resources/entities/EnergyWall.h"

#include "src/statistics/Statistics.h"

#include "src/resources/camera/ICamera.h"
#include "src/resources/camera/PerspectiveCamera.h"
#include "src/resources/camera/OrthogonalCamera.h"

#include "src/resources/GameEntity.h"
#include "src/resources/Transformation.h"

#include "src/resources/components/PhysicsComponent.h"
#include "src/resources/components/CollisionComponent.h"
#include "src/resources/components/EnergyWallCollisionComponent.h"
#include "src/resources/components/ThirdPersonCameraComponent.h"
#include "src/resources/components/CameraComponent.h"
#include "src/resources/components/RotationComponent.h"
#include "src/resources/components/LODComponent.h"
#include "src/resources/components/BillboardComponent.h"
#include "src/resources/components/OverWaterComponent.h"
#include "src/resources/components/CharacterComponent.h"
#include "src/resources/components/DebugComponent.h"

#include "src/resources/command/ICommand.h"
#include "src/resources/command/commands/RiseTerrainCommand.h"

#include "src/resources/events/characterControllerEvents/ForwardEvent.h"
#include "src/resources/events/characterControllerEvents/BackwardEvent.h"
#include "src/resources/events/characterControllerEvents/ZoomEvent.h"
#include "src/resources/events/characterControllerEvents/JumpEvent.h"
#include "src/resources/events/characterControllerEvents/TurnEvent.h"
#include "src/resources/events/characterControllerEvents/PitchEvent.h"

#include "src/input/IInputListener.h"
#include "src/input/bindings/KeyToEventBind.h"
#include "src/input/bindings/MouseToEventBind.h"



using namespace glm;
using namespace std;

#define and &&
#define or ||

enum Configuration
{
	DEBUG,
	TEXT,
	ENERGY_WALL,
	SHADOWS,
	PARTICLES,
	PROPS,
	COLLISIONS,
	FLAT,
	RELEASE
};

Configuration mConfiguration = DEBUG;

int movx[] = { 1, 1, 0, -1, -1, -1, 0, 1 };
int movy[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int despi[] = { 1, 1, -1, -1 };
int despj[] = { 1, -1, 1, -1 };
int muevei[] = { 1, 0, -1, 0 };
int muevej[] = { 0, 1, 0, -1 };


const float Pi = 3.141592f;

float mWaterHeight = 8.2f * TERRAIN_SCALE / 15.0f;

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
ICamera* mMapCamera;
ICamera* mEagleEyeCamera;
ThirdPersonCameraComponent* mThirdPersonCameraComponent;

RenderPass* mMapPass;
//Light* mSunLight;
glm::vec3 mSunLightDirection(-100000.0f, 100000.0f, -100000.0f);
Terrain* mTerrain;
Player* mPlayer;
Water* mWater;
GameEntity* mCamera;

EnergyWall* mEnergyWall;
Text* mText[5];
IMaterial* materialText;

float mFogDensity = 0.04f;
const float mFogGradient = 1.5f;
glm::vec3 mFogColor = vec3(89.0f, 120.0f, 143.0f) / 255.0f;
//red glm::vec3 mFogColor = vec3(218.0f, 74.0f, 43.0f) / 255.0f; 
float mEnergyWallRadius = 22.0f;
glm::vec3 mEnergyWallPosition(0.0f, 0.0f, 0.0f);

std::vector<std::string> texts = { "FPS: ", "Triangles: ", "Drawcalls: ", "GameEntities: ", "GEWithPhysics: "};

ICommand* mCurrentCommand = nullptr;

double aleatori()
{
	int r = rand();
	return double(r % 100) / 100;
}

/*
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
			vertexs[index].y = height / 255.0f * mTerrainHeightScale;

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
*/

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	mEngine.OnMouseScroll(GLFW_MOUSE_BUTTON_MIDDLE, static_cast<float>(yOffset));
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	mEngine.OnKey(key, action);
	//std::cout << "key = " << key << " action = " << action << "\n";
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	mEngine.OnMouseButton(button, action, mods);
}

void MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	mEngine.OnMouseCursorPos(xpos, ypos);
	//std::cout << "cursor X = " << xpos  << "\n";
}

GameEntity* CreateModelWithLod(const glm::vec3& position, const glm::vec3& scale, const std::vector<std::string>& models, const std::vector<float>& distances, IMaterial* material, IMaterial* materialNormalmap)
{
	GameEntity* modelEntity = new GameEntity(
		new Transformation(position, glm::vec3(0.0f), scale),
		nullptr
		);

	modelEntity->AddComponent(new PhysicsComponent(true, PhysicsSystem::GRAVITY_VALUE));
	modelEntity->AddComponent(new CollisionComponent());
	IRenderer* boundingBoxRenderer = new WireframeRenderer(mEngine.GetModel("cube"), mEngine.GetMaterial(RenderSystem::WIREFRAME_MATERIAL_NAME));
	modelEntity->AddComponent(new DebugComponent(boundingBoxRenderer));

	LODComponent* lodComponent = new LODComponent(mGameplayCamera);
	modelEntity->AddComponent(lodComponent);
	//WARNING!!! debemos añadir el componente a la entity primero para que se le asigne el parent, a partir de ahí, agregar los demás lod que ya les asignará el padre.

	for (unsigned int i = 0; i < models.size(); ++i)
	{
		IMaterial* m = material;
		if (i == 0 && materialNormalmap != nullptr)
		{
			m = materialNormalmap;
		}

		IRenderer* renderer = new VertexsRenderer(mEngine.GetModel(models[i]), m);
		lodComponent->AddLevelOfDetail(renderer, distances[i]);
	}

	return modelEntity;
}

GameEntity* CreateModel(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, Model* model, IMaterial* material)
{
	IRenderer* renderer = new VertexsRenderer(model, material);

	GameEntity* modelEntity = new GameEntity(
												new Transformation(position, rotation, scale),
												renderer
											);

	modelEntity->AddComponent(new PhysicsComponent(true, PhysicsSystem::GRAVITY_VALUE));
	modelEntity->AddComponent(new CollisionComponent());

	IRenderer* boundingBoxRenderer = new WireframeRenderer(mEngine.GetModel("cube"), mEngine.GetMaterial(RenderSystem::WIREFRAME_MATERIAL_NAME));
	modelEntity->AddComponent(new DebugComponent(boundingBoxRenderer));

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
	
	IMaterial* materialFoliage = mEngine.CreateMaterial("tree_foliage", mEngine.GetShader("model"));
	materialFoliage->AddEffect(new MaterialEffectDiffuseTexture(static_cast<Texture*>(mEngine.GetTexture("tree_foliage_diffuse")), glm::vec3(1.0f, 1.0f, 1.0f), 1));
	materialFoliage->AddEffect(new MaterialEffectLightProperties(glm::vec3(100000.0f, 100000.0f, 100000.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	materialFoliage->AddEffect(new MaterialEffectFogProperties(mFogColor, mFogDensity, mFogGradient));
	materialFoliage->AddEffect(new MaterialEffectShadowProperties());

	IMaterial* materialTrunk = mEngine.CreateMaterial("tree_trunk", mEngine.GetShader("model"));
	materialTrunk->AddEffect(new MaterialEffectDiffuseTexture(static_cast<Texture*>(mEngine.GetTexture("tree_trunk_diffuse")), glm::vec3(1.0f, 1.0f, 1.0f), 1));
	materialTrunk->AddEffect(new MaterialEffectLightProperties(glm::vec3(100000.0f, 100000.0f, 100000.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	materialTrunk->AddEffect(new MaterialEffectFogProperties(mFogColor, mFogDensity, mFogGradient));
	materialTrunk->AddEffect(new MaterialEffectShadowProperties());

	IMaterial* materialTrunkNormalmap = mEngine.CreateMaterial("tree_trunk_normalmap", mEngine.GetShader("normalmap"));
	materialTrunkNormalmap->AddEffect(new MaterialEffectDiffuseTexture(static_cast<Texture*>(mEngine.GetTexture("tree_trunk_diffuse")), glm::vec3(1.0f, 1.0f, 1.0f), 1));
	materialTrunkNormalmap->AddEffect(new MaterialEffectLightProperties(glm::vec3(100000.0f, 100000.0f, 100000.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	materialTrunkNormalmap->AddEffect(new MaterialEffectFogProperties(mFogColor, mFogDensity, mFogGradient));
	materialTrunkNormalmap->AddEffect(new MaterialEffectNormalTexture(static_cast<Texture*>(mEngine.GetTexture("tree_trunk_normalmap")), 1));
	materialTrunkNormalmap->AddEffect(new MaterialEffectShadowProperties());

	for (unsigned long i = 0; i < positions.size(); i++)
	{
		GameEntity* entity = CreateModelWithLod(positions[i], sizes[i], modelsFoliage, distances, materialFoliage, nullptr);
		mEngine.AddGameEntity(entity);
		
		entity = CreateModelWithLod(positions[i], sizes[i], modelsTrunk, distances, materialTrunk, materialTrunkNormalmap);
		mEngine.AddGameEntity(entity);
	}
}

void CreateProps()
{
	int areaSize = 5;
	int numProps = 6;

	std::vector<std::string> models;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> rotations;

	models.push_back(std::string("barrel"));
	models.push_back(std::string("chest"));
	models.push_back(std::string("brazier"));
	models.push_back(std::string("barrel"));
	models.push_back(std::string("crate"));
	models.push_back(std::string("statue"));
	//models.push_back(std::string("floor"));
	


	positions.push_back(glm::vec3(0.8f, 0.0f, -2.3f));
	positions.push_back(glm::vec3(0.4f, 0.0f, -2.0f));
	positions.push_back(glm::vec3(1.0f, 0.0f, -1.7f));
	positions.push_back(glm::vec3(1.1f, 0.0f, -2.3f));
	positions.push_back(glm::vec3(2.5f, 0.0f, -2.7f));
	positions.push_back(glm::vec3(-2.0f, 0.0f, 2.0f));
	positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));	

	rotations.push_back(glm::vec3(0.0f));
	rotations.push_back(glm::vec3(0.0f));
	rotations.push_back(glm::vec3(0.0f));
	rotations.push_back(glm::vec3(1.5f, 0.0f, 0.0f));
	rotations.push_back(glm::vec3(0.0f));
	rotations.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	rotations.push_back(glm::vec3(0.0f));

	std::string textureName("MedievalDungeonPropsAtlas02_diffuse");
	std::string textureNormalName("MedievalDungeonPropsAtlas02_normalmap");

	Texture* texture = static_cast<Texture*>(mEngine.GetTexture(textureName));
	Texture* normal = static_cast<Texture*>(mEngine.GetTexture(textureNormalName));

	IMaterial* material = mEngine.CreateMaterial("model", mEngine.GetShader("normalmap"));
	material->AddEffect(new MaterialEffectDiffuseTexture(texture, glm::vec3(1.0f, 1.0f, 1.0f), 1));
	material->AddEffect(new MaterialEffectLightProperties(glm::vec3(100000.0f, 100000.0f, 100000.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	material->AddEffect(new MaterialEffectFogProperties(mFogColor, mFogDensity, mFogGradient));
	material->AddEffect(new MaterialEffectNormalTexture(normal, 1));
	material->AddEffect(new MaterialEffectShadowProperties());

	for (int i = 0; i < numProps; i++)
	{
		float x = static_cast<float>(-areaSize / 2 + 2 * rand() % areaSize);
		float z = static_cast<float>(-areaSize / 2 + 2 * rand() % areaSize);

		x = positions[i % positions.size()].x;
		z = positions[i % positions.size()].z;

		float height = mTerrain->GetHeight(glm::vec2(x, z)) - 0.1f;
		//if (height > mWaterHeight + 0.2f)
		{
			glm::vec3 position(x, height, z);
			glm::vec3 scale(0.3f);
			glm::vec3 rotation(rotations[i % rotations.size()]);
			
			std::string modelName = models[i % models.size()];
			Model* model = mEngine.GetModel(modelName);

			GameEntity* entity = CreateModel(position, scale, rotation, model, material);
			mEngine.AddGameEntity(entity);
		}
	}
}

void CreateWater()
{
	//WATER
	if (mIsWaterEnabled)
	{
		float waterSpeed = 0.02f;
		IMaterial* material = mEngine.CreateMaterial("water", mEngine.GetShader("water"));
		material->AddEffect(new MaterialEffectFogProperties(mFogColor, mFogDensity, mFogGradient));
		material->AddEffect(new MaterialEffectWater(
														mEngine.GetTexture("reflection_water"),
														mEngine.GetTexture("refraction_water"),
														mEngine.GetTexture("distorsion_water"),
														mEngine.GetTexture("normal_water"),
														mEngine.GetTexture("refraction_depth_water"),
														waterSpeed,
														glm::vec4(0.0f, 0.3f, 0.8f, 0.0f)
													));
		material->AddEffect(new MaterialEffectLightProperties(glm::vec3(100000.0f, 100000.0f, 100000.0f), glm::vec3(1.0f, 1.0f, 1.0f)));

		mWater = new Water(		new Transformation(
													glm::vec3(4.0f, mWaterHeight, 4.5f), 
													glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), 
													glm::vec3(1.0f)),
								material,
								50.0f,
								50.0f,
								waterSpeed
							);
		mEngine.AddGameEntity(mWater);
	}
}

Particle* CreateParticle(bool canCollide, Texture* texture, glm::vec3& gravity)
{
	IMaterial* material = mEngine.CreateMaterial("particle", mEngine.GetShader("particle"));
	material->AddEffect(new MaterialEffectParticle(	texture, 
													mEngine.GetTexture("depth_texture"), 
													glm::vec2(mEngine.GetScreenWidth(), mEngine.GetScreenHeight()), 
													1.0f)
						);

	Particle* particle = new Particle(new Transformation(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.1f)),
		mEngine.GetModel("particle_quad"),
		material,
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

	ParticlesEmitter* particlesEmitter = new ParticlesEmitter(	particle,
																new Transformation(glm::vec3(x, height, z), glm::vec3(0.0f), glm::vec3(0.1f)),
																nullptr,
																150);
	particlesEmitter->SetColorGradientValues(glm::vec4(1.0f, 1.0f, 0.25f, 1.0f), glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));	
	particlesEmitter->SetScaleValues(0.05f, 0.005f);
	particlesEmitter->SetVelocity(glm::vec3(0.0f), glm::vec3(0.02f, 0.2f, 0.02f));
	particlesEmitter->SetSpawnArea(glm::vec3(-0.02f, 0.0f, -0.02f), glm::vec3(0.03f, 0.0f, 0.03f));
	mEngine.AddGameEntity(particlesEmitter);
	mEngine.AddParticleEmitter(particlesEmitter);
}

void CreateEnergyWall()
{
	IMaterial* material = mEngine.CreateMaterial("energy_wall", mEngine.GetShader("energy_wall"));
	material->AddEffect(new MaterialEffectDiffuseTexture(mEngine.GetTexture("yellow_grid"), glm::vec3(0.0f), 50.0f));
	material->AddEffect(new MaterialEffectDepthTexture(mEngine.GetTexture("depth_texture"), 1.0f));
	material->AddEffect(new MaterialEffectFloat2(glm::vec2(mEngine.GetScreenWidth(), mEngine.GetScreenHeight())));

	mEnergyWall = new EnergyWall(	new Transformation(mEnergyWallPosition, glm::vec3(0.0f), glm::vec3(mEnergyWallRadius)),
									material,
									mEngine.GetModel("sphere"),
									2.0f
								);
	mEngine.AddGameEntity(mEnergyWall);
	mEngine.SetEnergyWall(mEnergyWallPosition, mEnergyWallRadius);
}

void CreateHUD()
{
	//QUAD
	IMaterial* material = mEngine.CreateMaterial("gui", mEngine.GetShader("gui"));
	material->AddEffect(new MaterialEffectDiffuseTexture(mEngine.GetTexture("hud_map"), glm::vec3(1.0f), 1.0f));
	IRenderer* guiRenderer = new IndexesRenderer(mEngine.GetModel("gui_quad"), material);
	guiRenderer->SetLayer(IRenderer::LAYER_GUI);

	float x = mEngine.GetScreenWidth() * 0.5f * 0.90f;
	float y = -mEngine.GetScreenHeight() * 0.5f * 0.85f;
	GameEntity* quad = new GameEntity(	new Transformation(glm::vec3(x, y, 0.0f), 
										glm::vec3(0.0f), 
										glm::vec3(256.0f)),
										guiRenderer
									);
	mEngine.AddGameEntity(quad);

	/*
	IRenderer* mapRenderer = new GUIRenderer(mEngine.GetShader("gui"),
		static_cast<Texture*>(mEngine.GetTexture("map")),
		87.0f,
		73.0f
	);
	GameEntity* map = new GameEntity(new Transformation(glm::vec3(420.0f, -300.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
		mapRenderer
	);
	mEngine.AddGameEntity(map);*/
}

/*

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
*/

void CreateTextTest()
{
	FontType* font = mEngine.GetFont("OCR A Extended");

	materialText = mEngine.CreateMaterial("text", mEngine.GetShader("text"));
	materialText->AddEffect(new MaterialEffectDiffuseTexture(font->GetTexture(), glm::vec3(1.0f), 1.0f));
	materialText->AddEffect(new MaterialEffectText(	glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
													glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
													0.4f,
													0.1f,
													0.0f,
													0.0f,
													glm::vec2(0.0f)));

	for (unsigned int i = 0; i < texts.size(); ++i)
	{
		mText[i] = new Text(
			new Transformation(	glm::vec3(-mEngine.GetScreenWidth() * 0.5f, mEngine.GetScreenHeight() * 0.5f - i * 20.0f, 0.0f),
								glm::vec3(0.0f),
								glm::vec3(0.70f)
							),
			materialText, font,
			texts[i], false, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, false);
		mEngine.AddGameEntity(mText[i]);
	}
	float x = 0.0f;
	float z = 0.0f;
	float height = mTerrain->GetHeight(glm::vec2(x, z)) + 1.0f;

	IMaterial* material3D = mEngine.CreateMaterial("text3D", mEngine.GetShader("text"));
	material3D->AddEffect(new MaterialEffectDiffuseTexture(font->GetTexture(), glm::vec3(1.0f), 1.0f));
	material3D->AddEffect(new MaterialEffectText(	glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
													glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
													0.4f,
													0.1f,
													0.3f,
													0.6f,
													glm::vec2(0.0f)));

	Text* mTestText = new Text(	new Transformation(glm::vec3(x, height, z), glm::vec3(0.0f), glm::vec3(0.01f)),
								material3D, font,
								"Origin", true, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 1, 1, false);
	mEngine.AddGameEntity(mTestText);
	/*
	x = 10.0f;
	z = 10.0f;
	height = mTerrain->GetHeight(glm::vec2(x, z)) + 1.0f;

	mTestText = new Text(new Transformation(glm::vec3(x, height, z), glm::vec3(0.0f), glm::vec3(.01f)),
		mEngine.GetShader("text"), mEngine.GetFont("OCR A Extended"),
		"Market", true, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 1, 1, false);
	mTestText->SetOutlineColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
	mTestText->SetBorderParameters(0.5f, 0.1f, 0.5f, 0.4f);
	mTestText->SetShadow(glm::vec2(0.002f, 0.002f));
	mEngine.AddGameEntity(mTestText);*/
}

void CreateTerrain()
{
	//TERRAIN GAME ENTITY
	//mSunCamera = new OrthogonalCamera(20.0f, 20.0f, -10.0f, 20.0f);
	//glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	//glm::vec3 direction = glm::vec3(camera->GetTarget() - glm::vec3(100000.0f, 100000.0f, 100000.0f));
	//glm::mat4 depthViewMatrix = glm::lookAt(direction, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	//glm::mat4 depthModelMatrix = glm::mat4(1.0);
	//glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	float scale = mIsTerrainFlat ? 0.0f : TERRAIN_SCALE;

	IMaterial* material = mEngine.CreateMaterial("terrain", mEngine.GetShader("terrain"));
	material->AddEffect(new MaterialEffectDiffuseTexture(static_cast<Texture*>(mEngine.GetTexture("terrain_blendmap")), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f));
	material->AddEffect(new MaterialEffectLightProperties(glm::vec3(100000.0f, 100000.0f, 100000.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	material->AddEffect(new MaterialEffectFogProperties(mFogColor, mFogDensity, mFogGradient));
	material->AddEffect(new MaterialEffectHeightMapTexture(static_cast<Texture*>(mEngine.GetTexture("terrain_heightmap")), 1.0f));
	material->AddEffect(new MaterialEffectTextureArray(static_cast<TextureArray*>(mEngine.GetTexture("terrain_array"))));
	material->AddEffect(new MaterialEffectClippingPlane());
	material->AddEffect(new MaterialEffectShadowProperties());
	material->AddEffect(new MaterialEffectFloat(scale));
	
	mTerrain = new Terrain(	new Transformation(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
							material,
							static_cast<Texture*>(mEngine.GetTexture("terrain_heightmap")),
							scale);

	mTerrain->SetFlat(mIsTerrainFlat);
			
	mEngine.AddGameEntity(mTerrain);	
	mEngine.SetTerrain(mTerrain);

	//TERRAIN NORMALS ENTITY
	//CreateTerrainNormals(vertexs, numVertexsSide);
}

void CreatePlayer()
{
	//PLAYER
	IMaterial* material = mEngine.CreateMaterial("player", mEngine.GetShader("normalmap"));
	material->AddEffect(new MaterialEffectDiffuseTexture(static_cast<Texture*>(mEngine.GetTexture("enano_diffuse")), glm::vec3(1.0f, 1.0f, 1.0f), 1));
	material->AddEffect(new MaterialEffectNormalTexture(static_cast<Texture*>(mEngine.GetTexture("enano_normalmap")), 1.0f));
	material->AddEffect(new MaterialEffectLightProperties(glm::vec3(100000.0f, 100000.0f, 100000.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	material->AddEffect(new MaterialEffectFogProperties(mFogColor, mFogDensity, mFogGradient));
	material->AddEffect(new MaterialEffectShadowProperties());
	
	InputComponent* inputComponent = new InputComponent();
	inputComponent->AddConverter(new KeyToEventBind(GLFW_KEY_W, new ForwardEvent()));
	inputComponent->AddConverter(new KeyToEventBind(GLFW_KEY_S, new BackwardEvent()));
	inputComponent->AddConverter(new MouseToEventBind(-1, new TurnEvent()));
	inputComponent->AddConverter(new KeyToEventBind(GLFW_KEY_SPACE, new JumpEvent()));
	inputComponent->AddConverter(new MouseToEventBind(GLFW_MOUSE_BUTTON_MIDDLE, new ZoomEvent()));

	Model* model = mEngine.GetModel("enano");
	IRenderer* renderer = new VertexsRenderer(model, material);

	mPlayer = new Player(	new Transformation(glm::vec3(0.0f, 4.9f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.03f)),
							renderer,
							inputComponent,
							new CharacterComponent(),
							new PhysicsComponent(false, PhysicsSystem::GRAVITY_VALUE),
							new CollisionComponent(),
							PLAYER_RUN_SPEED,
							PLAYER_TURN_SPEED,
							PLAYER_UPWARDS_HEIGHT
						);
	mPlayer->AddComponent(new EnergyWallCollisionComponent());
	IRenderer* boundingBoxRenderer = new WireframeRenderer(mEngine.GetModel("cube"), mEngine.GetMaterial(RenderSystem::WIREFRAME_MATERIAL_NAME));
	mPlayer->AddComponent(new DebugComponent(boundingBoxRenderer));
	mEngine.AddGameEntity(mPlayer);
}

void CreateGameCameraEntity()
{
	InputComponent* inputComponent = new InputComponent();
	inputComponent->AddConverter(new MouseToEventBind(GLFW_MOUSE_BUTTON_MIDDLE, new ZoomEvent()));
	inputComponent->AddConverter(new MouseToEventBind(-1, new PitchEvent()));

	mCamera = new GameEntity(new Transformation(mGameplayCamera->GetPosition(), glm::vec3(0.0f), glm::vec3(0.0f)),
		nullptr);// new CubeRenderer(mEngine.GetShader("default")));

	glm::vec3 targetOffset(0.0f, 0.5f, 0.0f); //head
	mThirdPersonCameraComponent = new ThirdPersonCameraComponent(	static_cast<PerspectiveCamera*>(mGameplayCamera), 
																	mPlayer, 
																	targetOffset, 
																	1.5f, 
																	PLAYER_PITCH, 
																	PLAYER_PITCH_SPEED, 
																	PLAYER_ZOOM_SPEED);
	mCamera->AddComponent(mThirdPersonCameraComponent);
	mCamera->AddComponent(new CollisionComponent());

	mCamera->AddComponent(new CharacterComponent());
	mCamera->AddComponent(inputComponent);
	
	if (mIsWaterEnabled)
	{
		mCamera->AddComponent(new OverWaterComponent(mWaterHeight));
	}

	mEngine.AddGameEntity(mCamera);
}

void CreateCube()
{
	//SKYBOX the last
	if (mIsSkyboxEnabled)
	{
		IMaterial* material = mEngine.CreateMaterial("skybox", mEngine.GetShader("skybox"));
		material->AddEffect(new MaterialEffectTextureCubemap(static_cast<TextureCubemap*>(mEngine.GetTexture("cubemap"))));
		material->AddEffect(new MaterialEffectFogProperties(mFogColor, mFogDensity, mFogGradient));

		SkyBoxRenderer* skyboxRenderer = new SkyBoxRenderer(mEngine.GetModel("skybox"), material);
		skyboxRenderer->SetLayer(IRenderer::LAYER_PARTICLES);
		
		GameEntity* skyBox = new GameEntity(
			new Transformation(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(50.0f)),
			skyboxRenderer
		);
		skyBox->AddComponent(new RotationComponent(glm::vec3(0.0f, 1.0f, 0.0f), SKYBOX_ROTATION_SPEED));
		mEngine.AddGameEntity(skyBox);
	}
}

void CreateCameras()
{
	float screenWidth = static_cast<float>(mEngine.GetScreenWidth());
	float screenHeight = static_cast<float>(mEngine.GetScreenHeight());
	float aspectRatio = screenWidth / screenHeight;

	//CAMERA
	mEagleEyeCamera = new PerspectiveCamera(VIEW_ANGLE, aspectRatio, NEAR_PLANE, FAR_PLANE);
	mEagleEyeCamera->SetPosition(glm::vec3(0.0f, 15.0f, 15.0f));
	mEagleEyeCamera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	mEagleEyeCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));

	mGameplayCamera = new PerspectiveCamera(VIEW_ANGLE, aspectRatio, NEAR_PLANE, FAR_PLANE);
	mGameplayCamera->SetPosition(glm::vec3(0.0f, 8.0f, 2.0f));
	mGameplayCamera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	mGameplayCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
}

void CreateEntities()
{
	const Texture* texture = static_cast<const Texture*>(mEngine.CreateDepthTexture("depth_texture", glm::vec2(mEngine.GetScreenWidth(), mEngine.GetScreenHeight())));

	//CreateHUD();

	CreatePlayer();

	CreateGameCameraEntity();

	CreateTerrain();

	CreateWater();

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

	if (mIsParticlesEnabled)
	{
		//CreateParticlesTest();
		CreateParticlesFire();
		//CreateParticlesSparkles();
	}

	if (mIsEnergyWallEnabled)
	{
		CreateEnergyWall();
	}

	if (mIsSkyboxEnabled)
	{
		CreateCube();
	}
}

/*
void CreateEntities2()
{
	const Texture* texture = static_cast<const Texture*>(mEngine.CreateDepthTexture("depth_texture", glm::vec2(mEngine.GetScreenWidth(), mEngine.GetScreenHeight())));

	//LIGHT GAME ENTITY
	mSunLight = new Light(mSunLightDirection, glm::vec3(1, 1, 1), nullptr);// new CubeRenderer(mEngine.GetShader("default")));
	mSunLight->AddComponent(new VerticalInputComponent(mEngine.GetGLWindow()));
	mSunLight->GetTransformation()->SetScale(glm::vec3(0.05f));

	mEngine.AddGameEntity(mSunLight);

	//CreateSpecificCubes();

	//CreateHUD();

	//CreateWaterHudPlanes();

	if (mIsShadowEnabled)
	{
		//CreateShadowPlane();
	}
}
*/

void DisableRenderPasses()
{
	if (mMapPass != nullptr)
	{
		mMapPass->SetEnable(false);
	}
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
	
	//IMaterial* material = mEngine.GetMaterial("shadow");
	//material->AddEffect(new MaterialEffectDiffuseTexture(mEngine.GetTexture("tree_foliage_diffuse"), glm::vec3(0.0f), 1.0f));
	//gameplayPass->SetMaterial(material);

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

void CreateSubSystems()
{
	//CreateHudMapRenderPass();

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
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		PerspectiveCamera* gameplayCamera = static_cast<PerspectiveCamera*>(mGameplayCamera);
		PerspectiveCamera* eagleEyeCamera = static_cast<PerspectiveCamera*>(mEagleEyeCamera);
		std::swap(*gameplayCamera, *eagleEyeCamera);
		if (mIsGameplayCameraEnabled)
		{
			if (mCamera->HasComponent<ThirdPersonCameraComponent>())
			{
				mCamera->RemoveComponent<ThirdPersonCameraComponent>();
			}
		}
		else
		{
			glm::vec3 targetOffset(0.0f, 0.5f, 0.0f); //head
			mThirdPersonCameraComponent = new ThirdPersonCameraComponent(	static_cast<PerspectiveCamera*>(mGameplayCamera), 
																			mPlayer, 
																			targetOffset, 
																			1.5f, 
																			PLAYER_PITCH, 
																			PLAYER_PITCH_SPEED, 
																			PLAYER_ZOOM_SPEED);
			mCamera->AddComponent(mThirdPersonCameraComponent);
		}

		mIsGameplayCameraEnabled = !mIsGameplayCameraEnabled;
	}
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		mIsShadowEnabled = !mIsShadowEnabled;
		mEngine.SetCastingShadowsEnabled(mIsShadowEnabled);
	}
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		if (mCurrentCommand == nullptr)
		{
			mCurrentCommand = new RiseTerrainCommand(mTerrain);
		}
	}

	if (mCurrentCommand != nullptr)
	{
		mCurrentCommand->Execute();
	}
}

void UpdateEnergyWallCollisions(float elapsedTime)
{
	EnergyWallCollisionComponent* component = mPlayer->GetComponent<EnergyWallCollisionComponent>();
	if (component->IsColliding())
	{
		glm::vec3 collisionPoint = component->GetCollisionPoint();
		mEnergyWall->SetContactPoint(collisionPoint);
		mEnergyWall->SetLiveTime(mEnergyWall->GetMaxLiveTime());
		//mEnergyWall->GetRenderer()->SetVisibility(true);
	}
	else if (!mEnergyWall->IsAlive())
	{
		mEnergyWall->SetContactPoint(glm::vec3(0.0f));
		//mEnergyWall->GetRenderer()->SetVisibility(false);
	}
}

void UpdateStatitstics()
{
	const Statistics* statistics = mEngine.GetStatistics();

	MaterialEffectText* effect = materialText->GetEffect<MaterialEffectText>();
	int fps = static_cast<int>(statistics->GetNumberFPS());
	if (fps < MIN_FPS_ALLOWED)
	{
		effect->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		effect->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	mText[0]->UpdateText(texts[0] + std::to_string(fps));
	mText[1]->UpdateText(texts[1] + std::to_string(statistics->GetNumberTrianglesRendered()));
	mText[2]->UpdateText(texts[2] + std::to_string(statistics->GetNumberDrawCalls()));
	mText[3]->UpdateText(texts[3] + std::to_string(statistics->GetNumberGameEntities()));
	mText[4]->UpdateText(texts[4] + std::to_string(statistics->GetNumberGameEntitiesWithPhysics()));
}

void UpdateCommand(float elapsedTime)
{
	if (mCurrentCommand != nullptr)
	{
		if (mCurrentCommand->HasFinished())
		{
			delete mCurrentCommand;
			mCurrentCommand = nullptr;
		}
		else
		{
			mCurrentCommand->Update(elapsedTime);
		}
	}
}

void Update(float elapsedTime)
{
	UpdateCommand(elapsedTime);
	
	if (mIsEnergyWallEnabled)
	{
		UpdateEnergyWallCollisions(elapsedTime);
	}
	if (mIsStatisticsVisible && mIsTextEnabled)
	{
		UpdateStatitstics();
	}
}

void SetupConfiguration()
{
	switch (mConfiguration)
	{
	case DEBUG:
		mIsDebugModeEnabled = true;
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
		break;
	case SHADOWS:
		mIsDebugModeEnabled = true;
		mIsWaterEnabled = false;
		mIsGameplayCameraEnabled = true;
		mIsFogEnabled = false;
		mIsVegetationEnabled = false;
		mIsPropsEnabled = false;
		mIsEnergyWallEnabled = false;
		mIsSkyboxEnabled = false;
		mIsTerrainFlat = false;
		mIsTextEnabled = false;
		mIsStatisticsVisible = false;
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
		mIsPropsEnabled = true;
		mIsEnergyWallEnabled = false;
		mIsSkyboxEnabled = false;
		mIsTerrainFlat = false;
		mIsTextEnabled = false;
		mIsStatisticsVisible = true;
		mIsParticlesEnabled = false;
		mIsShadowEnabled = false;
		mIsFullScreen = false;
		break; 
	case COLLISIONS:
		mIsDebugModeEnabled = true;
		mIsWaterEnabled = false;
		mIsGameplayCameraEnabled = true;
		mIsFogEnabled = false;
		mIsVegetationEnabled = false;
		mIsPropsEnabled = true;
		mIsEnergyWallEnabled = false;
		mIsSkyboxEnabled = false;
		mIsTerrainFlat = false;
		mIsTextEnabled = false;
		mIsStatisticsVisible = false;
		mIsParticlesEnabled = false;
		mIsShadowEnabled = true;
		break;
	case FLAT:
		mIsDebugModeEnabled = true;
		mIsWaterEnabled = true;
		mIsGameplayCameraEnabled = true;
		mIsFogEnabled = false;
		mIsVegetationEnabled = false;
		mIsPropsEnabled = false;
		mIsEnergyWallEnabled = false;
		mIsSkyboxEnabled = false;
		mIsTerrainFlat = false;
		mIsTextEnabled = false;
		mIsStatisticsVisible = true;
		mIsParticlesEnabled = false;
		mIsShadowEnabled = false;
		mIsFullScreen = false;
		break;
	case RELEASE:
		mIsDebugModeEnabled = true;
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

	mEngine.Init(mIsFullScreen);

	CreateCameras();

	mEngine.SetCastingShadowsParameters(mSunLightDirection, 3);
	mEngine.SetCastingShadowsEnabled(mIsShadowEnabled);

	mEngine.SetWaterEnabled(mIsWaterEnabled);
	mEngine.SetWaterParameters(mGameplayCamera, mWaterHeight);

	mEngine.SetDebugModeEnabled(mIsDebugModeEnabled);

	glfwSetScrollCallback(mEngine.GetGLWindow(), &ScrollCallback);
	glfwSetKeyCallback(mEngine.GetGLWindow(), &KeyCallback);
	glfwSetMouseButtonCallback(mEngine.GetGLWindow(), &MouseButtonCallback);
	glfwSetCursorPosCallback(mEngine.GetGLWindow(), &MouseCursorPosCallback);

	mEngine.PostInit();

	mFogDensity = mIsFogEnabled ? mFogDensity : 0.0f;
}

int main(void)
{
	Initialize();

	CreateEntities();
	CreateSubSystems();

	mEngine.SetCastingShadowsTarget(mPlayer);

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