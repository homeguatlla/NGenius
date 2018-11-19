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
#include <utility>

#include "EngineConstants.h"
#include "src/game/ShooterGameConstants.h"

#include "src/Singleton.h"
#include "src/NGenius.h"
#include "src/resources/systems/PhysicsSystem.h"
#include "src/resources/systems/renderSystem/RenderSystem.h"

#include "src/TerrainGrid.h"

#include "src/resources/renderers/IRenderer.h"
#include "src/resources/renderers/VerticesRenderer.h"
#include "src/resources/renderers/IndicesRenderer.h"
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
#include "src/resources/models/animation/AnimatedModel.h"

#include "src/resources/materials/MaterialsLibrary.h"
#include "src/resources/materials/IMaterial.h"
#include "src/resources/materials/effects/MaterialEffectDiffuseTexture.h"
#include "src/resources/materials/effects/MaterialEffectNormalTexture.h"
#include "src/resources/materials/effects/MaterialEffectDirectionalLightProperties.h"
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
#include "src/resources/materials/effects/MaterialEffectFloat3Array.h"
#include "src/resources/materials/effects/MaterialEffectMatrix4Array.h"

#include "src/resources/entities/Terrain.h"
#include "src/resources/entities/Character.h"
#include "src/resources/entities/Text.h"
#include "src/resources/entities/Water.h"
#include "src/resources/entities/Particle.h"
#include "src/resources/entities/ParticlesEmitter.h"
#include "src/resources/entities/EnergyWall.h"
#include "src/resources/entities/PointsPatch.h"

#include "src/resources/scene/GameScene.h"
#include "src/resources/scene/quadtree/GameEntityQuadTree.h"

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
#include "src/resources/components/GameEventsComponent.h"
#include "src/resources/components/DebugComponent.h"
#include "src/resources/components/SpacePartitionComponent.h"
#include "src/resources/components/EnvironmentAffectedComponent.h"
#include "src/resources/components/EnvironmentModificatorComponent.h"
#include "src/resources/components/AnimationComponent.h"

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



#include "src/utils/Log.h"

#include "src/game/ShooterGame.h"


using namespace glm;
using namespace std;

#define and &&
#define or ||

enum Configuration
{
	TEST,
	DEBUG,
	TEXT,
	ENERGY_WALL,
	SHADOWS,
	PARTICLES,
	PROPS,
	COLLISIONS,
	FLAT,
	QUADTREE,
	QUADTREE_WITH_CAMERA,
	RELEASE
};

Configuration mConfiguration = RELEASE;

int movx[] = { 1, 1, 0, -1, -1, -1, 0, 1 };
int movy[] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int despi[] = { 1, 1, -1, -1 };
int despj[] = { 1, -1, 1, -1 };
int muevei[] = { 1, 0, -1, 0 };
int muevej[] = { 0, 1, 0, -1 };

const float WaterFloor = 8.2f * EngineConstants::TERRAIN_SCALE / 15.0f;
float mWaterHeight = WaterFloor;

bool mIsDebugModeEnabled = false;
bool mIsWaterEnabled = true;
bool mIsGameplayCameraEnabled = true;
bool mIsFogEnabled = true;
bool mIsVegetationEnabled = false;
bool mIsEnergyWallEnabled = true;
bool mIsSkyboxEnabled = true;
bool mIsTerrainFlat = false;
bool mIsTextEnabled = false;
bool mIsStatisticsVisible = false;
bool mIsShadowEnabled = false;
bool mIsParticlesEnabled = false;
bool mIsFullScreen = false;
bool mIsPropsEnabled = false;

bool mIsSpacePartitionEnabled = false;

typedef Singleton<NGenius> SNGenius;


ICamera* mGameplayCamera;
ICamera* mEagleEyeCamera;
RenderPass *mGameplayPass; 
ThirdPersonCameraComponent* mThirdPersonCameraComponent;

RenderPass* mMapPass;
//Light* mSunLight;
Terrain* mTerrain;
Character* mPlayer;
Water* mWater;
GameEntity* mCamera;
GameEntity* mQuadTreeBox;
GameScene* mScene;

EnergyWall* mEnergyWall;

float mEnergyWallRadius = 44.0f;
glm::vec3 mEnergyWallPosition(0.0f, 0.0f, 0.0f);

const std::vector<std::string> texts = { "FPS: ", "Triangles: ", "Drawcalls: ", "GameEntities(GE): ", "GESpacePartition:", "GERendered:", "GEWithPhysics: ", "DayTime: "};
Text* mText[8];
IMaterial* materialText;

ICommand* mCurrentCommand = nullptr;

//camera bounding box
GameEntity* mCameraAABBEntity;
GameEntity* mCameraTargetEntity;

//for QUADTREE setup
float aabbSize = 2.0f;
const AABB mAABB(glm::vec3(-aabbSize, 0.0f, -aabbSize), glm::vec3(aabbSize, 0.0f, aabbSize));
GameEntityQuadTree mQuadTree(mAABB);
glm::vec3 mQuadMovingPosition(0.0f);
glm::vec3 mQuadMovingScale(1.0f);
GameEntity* mQuadTreeMovedEntity;
std::vector<GameEntity*> mQuadTreeEntities;


bool mIsShooterGameRunning = true;
ShooterGame mGame;



double aleatori()
{
	int r = rand();
	return double(r % 100) / 100;
}

/*
void CreateTerrainNormals(vector<glm::vec3>& vertexs, int numVertexsSide)
{
	IShaderProgram* defaultShaderProgram = SNGenius::GetInstance().GetShader("default");
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

	mScene->AddGameEntity(entity);
}

void CreateShadowPlane()
{
	//QUAD
	IRenderer* guiShadowRenderer = new GUIRenderer(	SNGenius::GetInstance().GetShader("gui"),
														static_cast<Texture*>(SNGenius::GetInstance().GetTexture("shadow_texture")),
														10.0f,
														10.0f
													);
	GameEntity* quadShadow = new GameEntity(	new Transformation(glm::vec3(0.0f, -300.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
												guiShadowRenderer
											);
	mScene->AddGameEntity(quadShadow);
}

void CreateWaterHudPlanes()
{
	//QUAD
	IRenderer* guiReflectionRenderer = new GUIRenderer(	SNGenius::GetInstance().GetShader("gui"),
															static_cast<Texture*>(SNGenius::GetInstance().GetTexture("reflection_water")),
															128.0f,
															128.0f
														);
	GameEntity* quadReflection = new GameEntity(	new Transformation(glm::vec3(420.0f, -300.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
		guiReflectionRenderer
												);
	mScene->AddGameEntity(quadReflection);

	IRenderer* guiRefractionRenderer = new GUIRenderer(SNGenius::GetInstance().GetShader("gui"),
															static_cast<Texture*>(SNGenius::GetInstance().GetTexture("refraction_depth_water")),
															128.0f,
															128.0f
														);
	GameEntity* guiRefraction = new GameEntity(	new Transformation(glm::vec3(-320.0f, -300.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
													guiRefractionRenderer
												);
	mScene->AddGameEntity(guiRefraction);
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
		ModelNormalMapRenderer* modelRenderer = new ModelNormalMapRenderer(SNGenius::GetInstance().GetModel("cube"),
			SNGenius::GetInstance().GetShader("model"),
			static_cast<Texture*>(SNGenius::GetInstance().GetTexture("cube_diffuse")),
			static_cast<Texture*>(SNGenius::GetInstance().GetTexture("cube_normalmap")),
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
		mScene->AddGameEntity(modelEntity);
	}
}
*/

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	SNGenius::GetInstance().OnMouseScroll(GLFW_MOUSE_BUTTON_MIDDLE, static_cast<float>(yOffset));
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	SNGenius::GetInstance().OnKey(key, action);
	//std::cout << "key = " << key << " action = " << action << "\n";
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	SNGenius::GetInstance().OnMouseButton(button, action, mods);
}

void MouseCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	SNGenius::GetInstance().OnMouseCursorPos(xpos, ypos);
	//std::cout << "cursor X = " << xpos  << "\n";
}

GameEntity* CreateModelWithLod(const glm::vec3& position, const glm::vec3& scale, const std::vector<std::string>& models, const std::vector<std::pair<float, bool>>& lod, IMaterial* material, IMaterial* materialNormalmap, bool isCullingEnabled)
{
	GameEntity* modelEntity = new GameEntity(
		new Transformation(position, glm::vec3(0.0f), scale),
		nullptr
		);

	modelEntity->AddComponent(new PhysicsComponent(true, SNGenius::GetInstance().GetGravity()));
	modelEntity->AddComponent(new CollisionComponent());
	modelEntity->AddComponent(new SpacePartitionComponent());
	IRenderer* boundingBoxRenderer = new WireframeRenderer(SNGenius::GetInstance().GetModel("cube"), SNGenius::GetInstance().GetMaterial(MaterialsLibrary::WIREFRAME_MATERIAL_NAME));
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

		IRenderer* renderer = new VerticesRenderer(SNGenius::GetInstance().GetModel(models[i]), m);
		lodComponent->AddLevelOfDetail(renderer, lod[i].first);
		if (lod[i].second)
		{
			renderer->SetBillboard(true);
			renderer->SetTransparency(true);
		}
		renderer->SetCullingEnabled(isCullingEnabled);
	}

	return modelEntity;
}

GameEntity* CreateModel(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, Model* model, IMaterial* material)
{
	IRenderer* renderer = new VerticesRenderer(model, material);

	GameEntity* modelEntity = new GameEntity(
												new Transformation(position, rotation, scale),
												renderer
											);

	modelEntity->AddComponent(new PhysicsComponent(true, SNGenius::GetInstance().GetGravity()));
	modelEntity->AddComponent(new CollisionComponent());
	modelEntity->AddComponent(new SpacePartitionComponent());

	IRenderer* boundingBoxRenderer = new WireframeRenderer(SNGenius::GetInstance().GetModel("cube"), SNGenius::GetInstance().GetMaterial(MaterialsLibrary::WIREFRAME_MATERIAL_NAME));
	modelEntity->AddComponent(new DebugComponent(boundingBoxRenderer));

	return modelEntity;
}

GameEntity* CreateQuadTreeBoxEntity(float size, glm::vec3 position, glm::vec3 color)
{
	IMaterial* material = SNGenius::GetInstance().CreateMaterial("quadtree", SNGenius::GetInstance().GetShader("default"));
	material->AddEffect(new MaterialEffectFloat3(color));

	IRenderer* renderer = new WireframeRenderer(SNGenius::GetInstance().GetModel("cube"), material);
	GameEntity* quad = new GameEntity(
		new Transformation(position, glm::vec3(0.0f), glm::vec3(size * 2.0f)),
		renderer
	);
	IRenderer* boundingBoxRenderer = new WireframeRenderer(SNGenius::GetInstance().GetModel("cube"), SNGenius::GetInstance().GetMaterial(MaterialsLibrary::WIREFRAME_MATERIAL_NAME));
	quad->AddComponent(new DebugComponent(boundingBoxRenderer));

	mScene->AddEntity(quad);
	return quad;
}

void CreateTrees()
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> sizes;

	float areaSize = 100.0f;
	int numTrees = 200;
	for (int i = 0; i < numTrees; i++)
	{
		float randValue = (rand() % 1000) * areaSize / 1000.0f;
		float x = -areaSize * 0.5f + randValue;
		randValue = (rand() % 1000) * areaSize / 1000.0f;
		float z = -areaSize * 0.5f + randValue;
		float height = mTerrain->GetHeight(glm::vec2(x, z));
		
		if (mConfiguration == QUADTREE_WITH_CAMERA || (height > mWaterHeight + 0.2f))
		{
			positions.push_back(glm::vec3(x, height, z));
			float scale = 1.0f;// (rand() % 5) / 200.0f + 0.02f;
			sizes.push_back(glm::vec3(scale));
		}
	}

	std::vector<std::string> modelsFoliage;
	modelsFoliage.push_back("tree_foliage_0");
	modelsFoliage.push_back("tree_foliage_1");
	modelsFoliage.push_back("tree_foliage_2");

	std::vector<std::pair<float, bool>> lod;
	lod.push_back(std::pair<float, bool>(500.0f, false));
	lod.push_back(std::pair<float, bool>(800.0f, false));
	lod.push_back(std::pair<float, bool>(1000.0f, false));

	std::vector<std::string> modelsTrunk;
	modelsTrunk.push_back("tree_trunk_0");
	modelsTrunk.push_back("tree_trunk_1");
	modelsTrunk.push_back("tree_trunk_2");
	
	IMaterial* materialFoliage = SNGenius::GetInstance().CreateMaterial("tree_foliage", SNGenius::GetInstance().GetShader("model"));
	materialFoliage->AddEffect(new MaterialEffectDiffuseTexture(static_cast<Texture*>(SNGenius::GetInstance().GetTexture("tree_foliage_diffuse")), glm::vec3(1.0f, 1.0f, 1.0f), 1));
	materialFoliage->AddEffect(new MaterialEffectDirectionalLightProperties());
	materialFoliage->AddEffect(new MaterialEffectFogProperties());
	materialFoliage->AddEffect(new MaterialEffectShadowProperties(1));
	materialFoliage->AddEffect(new MaterialEffectClippingPlane());

	IMaterial* materialTrunk = SNGenius::GetInstance().CreateMaterial("tree_trunk", SNGenius::GetInstance().GetShader("model"));
	materialTrunk->AddEffect(new MaterialEffectDiffuseTexture(static_cast<Texture*>(SNGenius::GetInstance().GetTexture("tree_trunk_diffuse")), glm::vec3(1.0f, 1.0f, 1.0f), 1));
	materialTrunk->AddEffect(new MaterialEffectDirectionalLightProperties());
	materialTrunk->AddEffect(new MaterialEffectFogProperties());
	materialTrunk->AddEffect(new MaterialEffectShadowProperties(1));
	materialTrunk->AddEffect(new MaterialEffectClippingPlane());

	IMaterial* materialTrunkNormalmap = SNGenius::GetInstance().CreateMaterial("tree_trunk_normalmap", SNGenius::GetInstance().GetShader("normalmap"));
	materialTrunkNormalmap->AddEffect(new MaterialEffectDiffuseTexture(static_cast<Texture*>(SNGenius::GetInstance().GetTexture("tree_trunk_diffuse")), glm::vec3(1.0f, 1.0f, 1.0f), 1));
	materialTrunkNormalmap->AddEffect(new MaterialEffectDirectionalLightProperties());
	materialTrunkNormalmap->AddEffect(new MaterialEffectFogProperties());
	materialTrunkNormalmap->AddEffect(new MaterialEffectNormalTexture(static_cast<Texture*>(SNGenius::GetInstance().GetTexture("tree_trunk_normalmap")), 1));
	materialTrunkNormalmap->AddEffect(new MaterialEffectShadowProperties(1));
	materialTrunkNormalmap->AddEffect(new MaterialEffectClippingPlane());

	for (unsigned long i = 0; i < positions.size(); i++)
	{
		GameEntity* entity = CreateModelWithLod(positions[i], sizes[i], modelsFoliage, lod, materialFoliage, nullptr, true);
		mScene->AddEntity(entity);

		entity = CreateModelWithLod(positions[i], sizes[i], modelsTrunk, lod, materialTrunk, materialTrunkNormalmap, true);
		mScene->AddEntity(entity);
	}
}

void CreateGrass()
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> sizes;

	float areaSize = 10.0f;
	int numGrass = 2000;
	for (int i = 0; i < numGrass; i++)
	{
		float x = -areaSize * 0.5f + (rand() % 1000) * (areaSize / 1000.0f);
		float z = -areaSize * 0.5f + (rand() % 1000) * (areaSize / 1000.0f);
		float height = mTerrain->GetHeight(glm::vec2(x, z));

		if (mConfiguration == QUADTREE_WITH_CAMERA || (height > mWaterHeight - 0.1f))
		{
			positions.push_back(glm::vec3(x, height, z));
			float scale = 0.1f + (rand() % 5) / 20.0f;// (rand() % 5) / 20.0f + 0.02f;
			sizes.push_back(glm::vec3(scale*2.0f, scale, scale));
		}
	}

	IMaterial* materialGrass = SNGenius::GetInstance().CreateMaterial("grass1", SNGenius::GetInstance().GetShader("model"));
	materialGrass->AddEffect(new MaterialEffectDiffuseTexture(static_cast<Texture*>(SNGenius::GetInstance().GetTexture("grass1_diffuse")), glm::vec3(1.0f, 1.0f, 1.0f), 1));
	materialGrass->AddEffect(new MaterialEffectDirectionalLightProperties());
	materialGrass->AddEffect(new MaterialEffectFogProperties());
	materialGrass->AddEffect(new MaterialEffectShadowProperties(0));

	std::vector<std::pair<float, bool>> lod;
	lod.push_back(std::pair<float, bool>(25.0f, false));
	lod.push_back(std::pair<float, bool>(100.0f, true));

	std::vector<std::string> models;
	models.push_back("grass1");
	models.push_back("grass2"); // modelo simplificado

	for (unsigned long i = 0; i < positions.size(); i++)
	{
		GameEntity* entity = CreateModel(positions[i], sizes[i], glm::vec3(0.0f), SNGenius::GetInstance().GetModel("grass2"), materialGrass);
		//entity->GetRenderer()->SetBillboard(true);
		entity->GetRenderer()->SetTransparency(true);
		entity->GetRenderer()->SetCullingEnabled(false);
		//GameEntity* entity = CreateModelWithLod(positions[i], sizes[i], models, lod, materialGrass, nullptr, false);
		mScene->AddEntity(entity);
	}
}

void CreatePoints()
{
	glPointSize(5.0f);
	IShaderProgram* shader = SNGenius::GetInstance().GetShader("grass");
	IMaterial* material = SNGenius::GetInstance().CreateMaterial("grass2_material", shader);

	material->AddEffect(new MaterialEffectFogProperties());
	material->AddEffect(new MaterialEffectFloat2(glm::vec2(4.0f, 4.0f)));
	material->AddEffect(new MaterialEffectShadowProperties(0));
	material->AddEffect(new MaterialEffectFloat(0.0));
	material->AddEffect(new MaterialEffectDirectionalLightProperties());
	material->AddEffect(new MaterialEffectFloat3Array());
	material->AddEffect(new MaterialEffectParticle(static_cast<Texture*>(SNGenius::GetInstance().GetTexture("grass2")),
		SNGenius::GetInstance().GetTexture("depth_texture"),
		glm::vec2(SNGenius::GetInstance().GetScreenWidth(), SNGenius::GetInstance().GetScreenHeight()),
		1.0f)
	);

	Texture* windTexture = static_cast<Texture*>(SNGenius::GetInstance().GetTexture("wind_texture"));
	material->AddEffect(new MaterialEffectNormalTexture(windTexture, 1.0f));
	material->AddEffect(new MaterialEffectClippingPlane());

	PointsPatch* pointsPatch = new PointsPatch(	new Transformation(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)), 
												material, mTerrain, mWaterHeight + 0.2f, mWaterHeight + 0.8f, 50.0f, 50.0f, 150.0f);

	EnvironmentAffectedComponent* environmentComponent = new EnvironmentAffectedComponent();
	environmentComponent->SetAffectedByWind(true);
	pointsPatch->AddComponent(environmentComponent);
	mScene->AddEntity(pointsPatch);
	/*
	material = SNGenius::GetInstance().CreateMaterial("grass3_material", shader);
	material->AddEffect(new MaterialEffectFogProperties(mFogColor, mFogDensity, mFogGradient));
	material->AddEffect(new MaterialEffectFloat2(glm::vec2(4.0f, 4.0f)));
	material->AddEffect(new MaterialEffectShadowProperties(0));
	material->AddEffect(new MaterialEffectFloat(0.0));
	material->AddEffect(new MaterialEffectFloat3Array());
	material->AddEffect(new MaterialEffectParticle(static_cast<Texture*>(SNGenius::GetInstance().GetTexture("grass3")),
		SNGenius::GetInstance().GetTexture("depth_texture"),
		glm::vec2(SNGenius::GetInstance().GetScreenWidth(), SNGenius::GetInstance().GetScreenHeight()),
		1.0f)
	);

	material->AddEffect(new MaterialEffectNormalTexture(windTexture, 1.0f));
	material->AddEffect(new MaterialEffectClippingPlane());

	pointsPatch = new PointsPatch(	new Transformation(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
									material, mTerrain, mWaterHeight - 0.1f, mWaterHeight + 0.2f, 50.0f, 50.0f, 100.0f);
	environmentComponent = new EnvironmentAffectedComponent();
	environmentComponent->SetAffectedByWind(true);
	pointsPatch->AddComponent(environmentComponent);
	mScene->AddEntity(pointsPatch);*/
}

void CreateProps()
{
	int areaSize = 5;
	int numProps = 6;

	std::vector<std::string> models;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> rotations;
	std::vector<float> scales;

	//crear una estructura con los 4 datos, rotacion, escalado, traslacion y nombre y así los tenemos siempre bien puestos
	models.push_back(std::string("barrel"));
	models.push_back(std::string("chest"));
	models.push_back(std::string("brazier"));
	models.push_back(std::string("barrel"));
	models.push_back(std::string("crate"));
	models.push_back(std::string("statue"));
	models.push_back(std::string("wood"));
	models.push_back(std::string("bridge"));

	//models.push_back(std::string("floor"));
	
	/*
	positions.push_back(glm::vec3(5.0f, 0.0f, 0.0f));
	positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	positions.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	positions.push_back(glm::vec3(0.0f, 0.0f, 5.0f));
	positions.push_back(glm::vec3(0.0f, 0.0f, -5.0f));*/
	/*
	int extraProps = 0;
	numProps += extraProps;
	for (int i = 0; i < extraProps; ++i)
	{
		float x = static_cast<float>(rand() % 7 * (1 - 2 * (rand() % 2)));
		float z = static_cast<float>(rand() % 7 * (1 - 2 * (rand() % 2)));

		positions.push_back(glm::vec3(x, 0.0f, z));
	}*/

	
	positions.push_back(glm::vec3(1.8f, 10.0f, -2.3f));
	positions.push_back(glm::vec3(0.4f, 10.0f, -2.0f));
	positions.push_back(glm::vec3(4.0f, 10.0f, -1.7f));
	positions.push_back(glm::vec3(3.1f, 10.0f, -4.3f));
	positions.push_back(glm::vec3(5.5f, 10.0f, -2.7f));
	positions.push_back(glm::vec3(-2.0f, 10.0f, 2.0f));
	positions.push_back(glm::vec3(0.0f, 10.0f, 0.0f));	

	positions.push_back(glm::vec3(10.0f, 10.0f, 0.0f));
	positions.push_back(glm::vec3(0.0f, 10.0f, 10.0f));

	rotations.push_back(glm::vec3(0.0f));
	rotations.push_back(glm::vec3(0.0f));
	rotations.push_back(glm::vec3(0.0f));
	rotations.push_back(glm::vec3(1.8f, 0.0f, 0.0f));
	rotations.push_back(glm::vec3(0.0f));
	rotations.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	rotations.push_back(glm::vec3(0.0f));
	rotations.push_back(glm::vec3(0.0f));
	rotations.push_back(glm::vec3(0.0f));

	scales.push_back(1.0f);
	scales.push_back(1.0f);
	scales.push_back(0.7f);//brazer
	scales.push_back(1.0f);
	scales.push_back(1.0f);
	scales.push_back(1.0f);
	scales.push_back(1.0f);
	scales.push_back(1.0f);
	scales.push_back(1.0f);
	scales.push_back(1.0f);


	std::string textureName("MedievalDungeonPropsAtlas02_diffuse");
	std::string textureNormalName("MedievalDungeonPropsAtlas02_normalmap");

	//std::string textureName("barrel_diffuse");
	//std::string textureNormalName("barrel_normalmap");

	Texture* texture = static_cast<Texture*>(SNGenius::GetInstance().GetTexture(textureName));
	Texture* normal = static_cast<Texture*>(SNGenius::GetInstance().GetTexture(textureNormalName));

	IMaterial* material = SNGenius::GetInstance().GetMaterial(MaterialsLibrary::MODEL_MATERIAL_NAME);
	material->AddEffect(new MaterialEffectDiffuseTexture(texture, glm::vec3(1.0f, 1.0f, 1.0f), 1));
	material->AddEffect(new MaterialEffectDirectionalLightProperties());
	material->AddEffect(new MaterialEffectFogProperties());
	material->AddEffect(new MaterialEffectNormalTexture(normal, 1));
	material->AddEffect(new MaterialEffectShadowProperties(1));
	material->AddEffect(new MaterialEffectClippingPlane());

	for (int i = 0; i < numProps; i++)
	{
		float x = static_cast<float>(-areaSize / 2 + 2 * rand() % areaSize);
		float z = static_cast<float>(-areaSize / 2 + 2 * rand() % areaSize);

		x = positions[i % positions.size()].x;
		z = positions[i % positions.size()].z;

		float height = mTerrain->GetHeight(glm::vec2(x, z));
		glm::vec3 position(x, height, z);
		glm::vec3 scale(scales[i % scales.size()]);
		glm::vec3 rotation(rotations[i % rotations.size()]);
			
		std::string modelName = models[i % models.size()];
		Model* model = SNGenius::GetInstance().GetModel(modelName);

		GameEntity* entity = CreateModel(position, scale, rotation, model, material);
		mScene->AddEntity(entity);
	}
}

void CreateWater()
{
	//WATER
	if (mIsWaterEnabled)
	{
		float waterSpeed = 0.02f;
		glm::vec4 waterColor(0.0f, 0.3f, 0.8f, 0.0f);

		IMaterial* material = SNGenius::GetInstance().CreateMaterial("water", SNGenius::GetInstance().GetShader("water"));
		material->AddEffect(new MaterialEffectFogProperties());
		material->AddEffect(new MaterialEffectWater(
														SNGenius::GetInstance().GetTexture("reflection_water"),
														SNGenius::GetInstance().GetTexture("refraction_water"),
														SNGenius::GetInstance().GetTexture("distorsion_water"),
														SNGenius::GetInstance().GetTexture("normal_water"),
														SNGenius::GetInstance().GetTexture("refraction_depth_water"),
														waterSpeed,
														waterColor
													));
		material->AddEffect(new MaterialEffectDirectionalLightProperties());

		mWater = new Water(		new Transformation(
													glm::vec3(4.0f, mWaterHeight, 4.5f), 
													glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), 
													glm::vec3(1.0f)),
								material,
								50.0f,
								50.0f,
								waterSpeed
							);
		mScene->AddEntity(mWater);
	}
}

Particle* CreateParticle(bool canCollide, Texture* texture, glm::vec3& gravity)
{
	IMaterial* material = SNGenius::GetInstance().CreateMaterial("particle", SNGenius::GetInstance().GetShader("particle"));
	material->AddEffect(new MaterialEffectParticle(	texture, 
													SNGenius::GetInstance().GetTexture("depth_texture"), 
													glm::vec2(SNGenius::GetInstance().GetScreenWidth(), SNGenius::GetInstance().GetScreenHeight()), 
													1.0f)
						);
	material->AddEffect(new MaterialEffectClippingPlane());

	Particle* particle = new Particle(new Transformation(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.1f)),
		SNGenius::GetInstance().GetModel("particle_quad"),
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
	Particle* particle = CreateParticle(false, static_cast<Texture*>(SNGenius::GetInstance().GetTexture("smoke")), glm::vec3(0.0f));
	particle->SetLiveTime(2.0f);

	float x = 4.0f;
	float z = -1.7f;

	float height = mTerrain->GetHeight(glm::vec2(x, z)) + 0.7f;

	ParticlesEmitter* particlesEmitter = new ParticlesEmitter(	particle,
																new Transformation(glm::vec3(x, height, z), glm::vec3(0.0f), glm::vec3(0.1f)),
																nullptr,
																100);
	particlesEmitter->SetColorGradientValues(glm::vec4(1.0f, 1.0f, 0.25f, 1.0f), glm::vec4(1.0f, 0.2f, 0.1f, 0.4f));	
	particlesEmitter->SetScaleValues(0.3f, 0.01f);
	particlesEmitter->SetVelocity(glm::vec3(0.0f), glm::vec3(0.04f, 0.4f, 0.04f));
	particlesEmitter->SetSpawnArea(glm::vec3(-0.06f, 0.0f, -0.06f), glm::vec3(0.06f, 0.0f, 0.06f));
	mScene->AddEntity(particlesEmitter);
	SNGenius::GetInstance().AddParticleEmitter(particlesEmitter);
}

void CreateEnergyWall()
{
	IMaterial* material = SNGenius::GetInstance().CreateMaterial("energy_wall", SNGenius::GetInstance().GetShader("energy_wall"));
	material->AddEffect(new MaterialEffectDiffuseTexture(SNGenius::GetInstance().GetTexture("yellow_grid"), glm::vec3(0.0f), 50.0f));
	material->AddEffect(new MaterialEffectDepthTexture(SNGenius::GetInstance().GetTexture("depth_texture"), 1.0f));
	material->AddEffect(new MaterialEffectFloat2(glm::vec2(SNGenius::GetInstance().GetScreenWidth(), SNGenius::GetInstance().GetScreenHeight())));

	mEnergyWall = new EnergyWall(	new Transformation(mEnergyWallPosition, glm::vec3(0.0f), glm::vec3(mEnergyWallRadius)),
									material,
									SNGenius::GetInstance().GetModel("sphere"),
									2.0f
								);
	mScene->AddEntity(mEnergyWall);
	SNGenius::GetInstance().SetEnergyWall(mEnergyWallPosition, mEnergyWallRadius);
}

void CreateHUD()
{
	//QUAD
	IMaterial* material = SNGenius::GetInstance().GetMaterial(MaterialsLibrary::GUI_MATERIAL_NAME);
	material->AddEffect(new MaterialEffectDiffuseTexture(SNGenius::GetInstance().GetTexture("hud_map"), glm::vec3(1.0f), 1.0f));
	IRenderer* guiRenderer = new IndicesRenderer(SNGenius::GetInstance().GetModel("gui_quad"), material);
	guiRenderer->SetLayer(IRenderer::LAYER_GUI);

	float x = SNGenius::GetInstance().GetScreenWidth() * 0.5f * 0.90f;
	float y = -SNGenius::GetInstance().GetScreenHeight() * 0.5f * 0.85f;
	GameEntity* quad = new GameEntity(	new Transformation(glm::vec3(x, y, 0.0f), 
										glm::vec3(0.0f), 
										glm::vec3(256.0f)),
										guiRenderer
									);
	mScene->AddEntity(quad);

	/*
	IRenderer* mapRenderer = new GUIRenderer(SNGenius::GetInstance().GetShader("gui"),
		static_cast<Texture*>(SNGenius::GetInstance().GetTexture("map")),
		87.0f,
		73.0f
	);
	GameEntity* map = new GameEntity(new Transformation(glm::vec3(420.0f, -300.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
		mapRenderer
	);
	mScene->AddGameEntity(map);*/
}

void CreateParticlesTest()
{
	glm::vec3 gravity = SNGenius::GetInstance().GetGravity();

	Particle* particle = CreateParticle(false, static_cast<Texture*>(SNGenius::GetInstance().GetTexture("smoke")), glm::vec3(0.0f));
	particle->SetLiveTime(10.0f);

	glm::vec3 position(3.0f, 0.0f, 4.0f); 
	float height = mTerrain->GetHeight(glm::vec2(position.x, position.z)) + 0.01f;
	position.y = height + 0.3f;
	

	ParticlesEmitter* particlesEmitter = new ParticlesEmitter(	particle,
																new Transformation(position, glm::vec3(0.0f), glm::vec3(1.0f)),
																nullptr,
																10);
	particlesEmitter->SetColorGradientValues(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	particlesEmitter->SetScaleValues(1.4f + (rand() % 4) / 10.0f, 0.5f);
	particlesEmitter->SetVelocity(glm::vec3(0.1f, 0.0f, 0.0f), glm::vec3(0.3f, 0.0f, 0.0f));
	particlesEmitter->SetSpawnArea(glm::vec3(-3.f, 0.0f, -3.0f), glm::vec3(3.f, 0.0f, 3.0f));
	
	mScene->AddEntity(particlesEmitter);
	SNGenius::GetInstance().AddParticleEmitter(particlesEmitter);	
	
	particle = CreateParticle(false, static_cast<Texture*>(SNGenius::GetInstance().GetTexture("smoke")), glm::vec3(0.0f));
	particle->SetLiveTime(1.0f);

	position = glm::vec3(4.0f, 0.0f, 4.0f);
	height = mTerrain->GetHeight(glm::vec2(position.x, position.z)) + 0.01f;
	position.y = height;
	particlesEmitter = new ParticlesEmitter(	particle,
												new Transformation(position, glm::vec3(0.0f), glm::vec3(0.1f)),
												nullptr,
												50);
	particlesEmitter->SetColorGradientValues(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	particlesEmitter->SetScaleValues(0.03f, 0.4f + (rand() % 4) / 10.0f);
	particlesEmitter->SetVelocity(glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f));
	mScene->AddEntity(particlesEmitter);
	SNGenius::GetInstance().AddParticleEmitter(particlesEmitter);
}

void CreateTextTest()
{
	FontType* font = SNGenius::GetInstance().GetFont("OCR A Extended");

	materialText = SNGenius::GetInstance().GetMaterial(MaterialsLibrary::TEXT_MATERIAL_NAME);
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
								new Transformation(	glm::vec3(-SNGenius::GetInstance().GetScreenWidth() * 0.5f, SNGenius::GetInstance().GetScreenHeight() * 0.5f - i * 20.0f, 0.0f),
								glm::vec3(0.0f),
								glm::vec3(0.70f)
							),
			materialText, font,
			texts[i], false, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, false);
		mScene->AddEntity(mText[i]);
	}
	/*float x = 0.0f;
	float z = 0.0f;
	float height = mTerrain->GetHeight(glm::vec2(x, z)) + 1.0f;

	IMaterial* material3D = SNGenius::GetInstance().GetMaterial(MaterialsLibrary::TEXT3D_MATERIAL_NAME);
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
	mScene->AddEntity(mTestText);*/
	/*
	x = 10.0f;
	z = 10.0f;
	height = mTerrain->GetHeight(glm::vec2(x, z)) + 1.0f;

	mTestText = new Text(new Transformation(glm::vec3(x, height, z), glm::vec3(0.0f), glm::vec3(.01f)),
		SNGenius::GetInstance().GetShader("text"), SNGenius::GetInstance().GetFont("OCR A Extended"),
		"Market", true, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 1, 1, false);
	mTestText->SetOutlineColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
	mTestText->SetBorderParameters(0.5f, 0.1f, 0.5f, 0.4f);
	mTestText->SetShadow(glm::vec2(0.002f, 0.002f));
	mScene->AddGameEntity(mTestText);*/
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

	float scale = mIsTerrainFlat ? 0.0f : EngineConstants::TERRAIN_SCALE;

	IMaterial* material = SNGenius::GetInstance().CreateMaterial("terrain", SNGenius::GetInstance().GetShader("terrain"));
	material->AddEffect(new MaterialEffectDiffuseTexture(static_cast<Texture*>(SNGenius::GetInstance().GetTexture("terrain_blendmap")), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f));
	material->AddEffect(new MaterialEffectDirectionalLightProperties());
	material->AddEffect(new MaterialEffectFogProperties());
	material->AddEffect(new MaterialEffectHeightMapTexture(static_cast<Texture*>(SNGenius::GetInstance().GetTexture("terrain_heightmap")), 1.0f));
	material->AddEffect(new MaterialEffectTextureArray(static_cast<TextureArray*>(SNGenius::GetInstance().GetTexture("terrain_array"))));
	material->AddEffect(new MaterialEffectClippingPlane());
	material->AddEffect(new MaterialEffectShadowProperties(3));
	material->AddEffect(new MaterialEffectFloat(scale));
	
	mTerrain = new Terrain(	new Transformation(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
							material,
							static_cast<Texture*>(SNGenius::GetInstance().GetTexture("terrain_heightmap")),
							scale);

	mTerrain->SetFlat(mIsTerrainFlat);
			
	mScene->AddEntity(mTerrain);
	SNGenius::GetInstance().SetTerrain(mTerrain);

	//TERRAIN NORMALS ENTITY
	//CreateTerrainNormals(vertexs, numVertexsSide);
}

void CreatePlayer()
{
	//TODO texto color std::cout << "\033[1;31mbold red text\033[0m normal text\n";
	//PLAYER
	IMaterial* material = SNGenius::GetInstance().CreateMaterial("player", SNGenius::GetInstance().GetShader("animated_model"));
	material->AddEffect(new MaterialEffectDiffuseTexture(static_cast<Texture*>(SNGenius::GetInstance().GetTexture("material_farmer_diffuse")), glm::vec3(1.0f, 1.0f, 1.0f), 1));
	//material->AddEffect(new MaterialEffectNormalTexture(static_cast<Texture*>(SNGenius::GetInstance().GetTexture("material_farmer_normalmap")), 1.0f));
	material->AddEffect(new MaterialEffectDirectionalLightProperties());
	material->AddEffect(new MaterialEffectFogProperties());
	material->AddEffect(new MaterialEffectShadowProperties(3));
	material->AddEffect(new MaterialEffectClippingPlane());
	material->AddEffect(new MaterialEffectMatrix4Array());

	InputComponent* inputComponent = new InputComponent();
	inputComponent->AddConverter(new KeyToEventBind(GLFW_KEY_W, new ForwardEvent()));
	inputComponent->AddConverter(new KeyToEventBind(GLFW_KEY_S, new BackwardEvent()));
	inputComponent->AddConverter(new MouseToEventBind(-1, new TurnEvent()));
	inputComponent->AddConverter(new KeyToEventBind(GLFW_KEY_SPACE, new JumpEvent()));
	inputComponent->AddConverter(new MouseToEventBind(GLFW_MOUSE_BUTTON_MIDDLE, new ZoomEvent()));

	Model* model = SNGenius::GetInstance().GetModel("farmer");
	//IRenderer* renderer = new VerticesRenderer(model, material);
	IRenderer* renderer = new IndicesRenderer(model, material);

	mPlayer = new Character(	new Transformation(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), glm::vec3(0.18f)),
							renderer,
							inputComponent,
							new GameEventsComponent(),
							new PhysicsComponent(false, SNGenius::GetInstance().GetGravity()),
							new CollisionComponent(),
		EngineConstants::PLAYER_RUN_SPEED,
		EngineConstants::PLAYER_TURN_SPEED,
		EngineConstants::PLAYER_UPWARDS_HEIGHT
						);
	mPlayer->AddComponent(new EnergyWallCollisionComponent());
	IRenderer* boundingBoxRenderer = new WireframeRenderer(SNGenius::GetInstance().GetModel("cube"), SNGenius::GetInstance().GetMaterial(MaterialsLibrary::WIREFRAME_MATERIAL_NAME));
	mPlayer->AddComponent(new DebugComponent(boundingBoxRenderer));
	mPlayer->AddComponent(new EnvironmentModificatorComponent());
	AnimationComponent* animationComponent = new AnimationComponent();
	animationComponent->AddAnimation(SNGenius::GetInstance().GetAnimation("animation_0"));
	mPlayer->AddComponent(animationComponent);
	mScene->AddEntity(mPlayer);
}

void CreateGameCameraEntity()
{
	InputComponent* inputComponent = new InputComponent();
	inputComponent->AddConverter(new MouseToEventBind(GLFW_MOUSE_BUTTON_MIDDLE, new ZoomEvent()));
	inputComponent->AddConverter(new MouseToEventBind(-1, new PitchEvent()));

	mCamera = new GameEntity(new Transformation(mGameplayCamera->GetPosition(), glm::vec3(0.0f), glm::vec3(0.0f)),
		nullptr);// new CubeRenderer(SNGenius::GetInstance().GetShader("default")));

	glm::vec3 targetOffset(0.0f, 0.5f, 0.0f); //head
	mThirdPersonCameraComponent = new ThirdPersonCameraComponent(	static_cast<PerspectiveCamera*>(mGameplayCamera), 
																	mPlayer, 
																	targetOffset, 
																	4.0f, 
																	EngineConstants::PLAYER_PITCH,
																	EngineConstants::PLAYER_PITCH_SPEED,
																	EngineConstants::PLAYER_ZOOM_SPEED);
	mCamera->AddComponent(mThirdPersonCameraComponent);
	mCamera->AddComponent(new CollisionComponent());

	mCamera->AddComponent(new GameEventsComponent());
	mCamera->AddComponent(inputComponent);
	
	if (mIsWaterEnabled)
	{
		mCamera->AddComponent(new OverWaterComponent(mWaterHeight));
	}

	mScene->AddEntity(mCamera);
}

void CreateSkybox()
{
	//SKYBOX the last
	if (mIsSkyboxEnabled)
	{
		IMaterial* material = SNGenius::GetInstance().CreateMaterial("skybox", SNGenius::GetInstance().GetShader("skybox"));
		TextureCubemap* cubemap1 = static_cast<TextureCubemap*>(SNGenius::GetInstance().GetTexture("day_cubemap"));
		TextureCubemap* cubemap2 = static_cast<TextureCubemap*>(SNGenius::GetInstance().GetTexture("night_cubemap"));
		material->AddEffect(new MaterialEffectTextureCubemap(cubemap1, cubemap2, 0.0f));
		material->AddEffect(new MaterialEffectFogProperties());
		material->AddEffect(new MaterialEffectDirectionalLightProperties());

		SkyBoxRenderer* skyboxRenderer = new SkyBoxRenderer(SNGenius::GetInstance().GetModel("skybox"), material);
		skyboxRenderer->SetLayer(IRenderer::LAYER_PARTICLES);
		
		GameEntity* skyBox = new GameEntity(
			new Transformation(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(100.0f)),
			skyboxRenderer
		);
		skyBox->AddComponent(new RotationComponent(glm::vec3(0.0f, 1.0f, 0.0f), EngineConstants::SKYBOX_ROTATION_SPEED));
		mScene->AddEntity(skyBox);
	}
}

void CreateCameras()
{
	float screenWidth = static_cast<float>(SNGenius::GetInstance().GetScreenWidth());
	float screenHeight = static_cast<float>(SNGenius::GetInstance().GetScreenHeight());
	float aspectRatio = screenWidth / screenHeight;

	//CAMERA
	//mEagleEyeCamera = new PerspectiveCamera(VIEW_ANGLE, aspectRatio, NEAR_PLANE, FAR_PLANE);
	float factor = 1.0f;
	if (mConfiguration != QUADTREE && mConfiguration != QUADTREE_WITH_CAMERA)
	{
		factor = 20.0f;
	}
	else
	{
		factor = 50.0f;
	}
	mEagleEyeCamera = new OrthogonalCamera("eagle_camera", screenWidth/factor, screenHeight/factor, EngineConstants::NEAR_PLANE, EngineConstants::FAR_PLANE);
	mEagleEyeCamera->SetPosition(glm::vec3(0.0f, 15.0f, 0.0f));
	mEagleEyeCamera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	mEagleEyeCamera->SetUp(glm::vec3(1.0f, 0.0f, 0.0f));

	SNGenius::GetInstance().AddCamera(mEagleEyeCamera);
	mGameplayCamera = SNGenius::GetInstance().GetCamera(EngineConstants::GAMEPLAY_CAMERA);
	/*
	mGameplayCamera = new PerspectiveCamera("gameplay_camera", EngineConstants::VIEW_ANGLE, aspectRatio, EngineConstants::NEAR_PLANE, EngineConstants::FAR_PLANE);
	mGameplayCamera->SetPosition(glm::vec3(0.0f, 5.0f, 5.0f));
	mGameplayCamera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	mGameplayCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
	SNGenius::GetInstance().AddCamera(mGameplayCamera);
	*/
	/*
	if (mConfiguration == QUADTREE_WITH_CAMERA)
	{
		mCameraTest = new PerspectiveCamera("gameplay_test", VIEW_ANGLE, aspectRatio, NEAR_PLANE, FAR_PLANE);
		mCameraTest->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		mCameraTest->SetTarget(glm::vec3(0.0f, 0.0f, 10.0f));
		mCameraTest->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
		SNGenius::GetInstance().AddCamera(mCameraTest);
	}*/
}

void CreateQuadTreeGrid(int levels, int maxWidth, const glm::vec3 color)
{
	int numQuads = static_cast<int>(pow(2, levels - 1));
	float size = static_cast<float>(maxWidth) / static_cast<float>(numQuads);
	glm::vec3 origin(-size * (numQuads - 1), 0.0f, -size * (numQuads - 1));

	for (int x = 0; x < numQuads; ++x)
	{
		for (int z = 0; z < numQuads; ++z)
		{
			glm::vec3 position(x * size * 2.0f, 0.0f, z * size * 2.0f);

			CreateQuadTreeBoxEntity(size, origin + position, color);
		}
	}
}

void CreateQuads()
{
	mQuadTreeBox = CreateQuadTreeBoxEntity(aabbSize, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	/*
	GameEntity* entity = CreateQuadTreeBoxEntity(1.3f, glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 1.0f));
	mQuadTree.AddGameEntity(entity);
	mQuadTreeEntities.push_back(entity);

	entity = CreateQuadTreeBoxEntity(0.05f, glm::vec3(1.7f, 0.0f, 1.7f), glm::vec3(0.0f, 1.0f, 1.0f));
	mQuadTree.AddGameEntity(entity);
	mQuadTreeEntities.push_back(entity);

	entity = CreateQuadTreeBoxEntity(0.1f, glm::vec3(-1.5f, 0.0f, -1.5f), glm::vec3(0.0f, 1.0f, 1.0f));
	mQuadTree.AddGameEntity(entity);
	mQuadTreeEntities.push_back(entity);

	entity = CreateQuadTreeBoxEntity(0.8f, glm::vec3(-0.9f, 0.0f, 1.1f), glm::vec3(0.0f, 1.0f, 1.0f));
	mQuadTree.AddGameEntity(entity);
	mQuadTreeEntities.push_back(entity);

	entity = CreateQuadTreeBoxEntity(0.2f, glm::vec3(-0.9f, 0.0f, 0.3f), glm::vec3(0.0f, 1.0f, 1.0f));
	mQuadTree.AddGameEntity(entity);
	mQuadTreeEntities.push_back(entity);
	*/
	
	int maxBoxes = 20;
	for (int i = 0; i < maxBoxes; ++i)
	{
		float size = (rand() % 100) * 0.005f + 0.02f;
		glm::vec3 position((rand()%400)* 0.01f - 2.0f, 0.0f, (rand() % 400)* 0.01f - 2.0f);
		GameEntity* entity = CreateQuadTreeBoxEntity(size, position, glm::vec3(0.0f, 1.0f, 1.0f));
		mQuadTree.AddGameEntity(entity);
		mQuadTreeEntities.push_back(entity);
	}

	GameEntity* entity = CreateQuadTreeBoxEntity(1.9f, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 1.0f));
	mQuadTree.AddGameEntity(entity);
	mQuadTreeEntities.push_back(entity);
	

	//quad reference for move and scale, different color no debug component
	mQuadTreeMovedEntity = CreateQuadTreeBoxEntity(mQuadMovingScale.x, mQuadMovingPosition, glm::vec3(1.0f, 1.0f, 0.0f));
	//mQuadTree.AddGameEntity(mQuadTreeMovedEntity);
	mQuadTreeMovedEntity->RemoveComponent<DebugComponent>();
}

void CreateCameraAABBEntity()
{
	AABB aabb = mGameplayCamera->GetAABB();
	mCameraAABBEntity = CreateQuadTreeBoxEntity(1.0f, aabb.GetCenter(), glm::vec3(1.0f, 1.0f, 0.0f));
	mCameraAABBEntity->RemoveComponent<DebugComponent>();

	mCameraTargetEntity = CreateQuadTreeBoxEntity(0.3f, glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
}

void CreateEntities()
{
	//const Texture* texture = static_cast<const Texture*>(SNGenius::GetInstance().CreateDepthTexture("depth_texture", glm::vec2(SNGenius::GetInstance().GetScreenWidth(), SNGenius::GetInstance().GetScreenHeight())));
	const Texture* texture = static_cast<const Texture*>(SNGenius::GetInstance().GetTexture("depth_texture"));

	//CreateHUD();

	CreatePlayer();

	CreateGameCameraEntity();

	CreateTerrain();

	CreateWater();

	if (mIsVegetationEnabled)
	{
		CreateTrees();
		//CreateGrass();
		CreatePoints();
	}

	if (mIsPropsEnabled)
	{
		CreateProps();
	}

	if (mIsParticlesEnabled)
	{
		//CreateParticlesTest();
		CreateParticlesFire();
	}

	if (mIsEnergyWallEnabled)
	{
		CreateEnergyWall();
	}

	if (mIsSkyboxEnabled)
	{
		CreateSkybox();
	}

	if (mConfiguration == QUADTREE)
	{
		CreateQuadTreeGrid(2, static_cast<int>(aabbSize), glm::vec3(0.0f, 1.0f, 0.0f));
		CreateQuads();
	}

	if (mConfiguration == QUADTREE_WITH_CAMERA)
	{
		CreateCameraAABBEntity();
	}
}

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
	ICamera* camera = new PerspectiveCamera("map_camera", EngineConstants::VIEW_ANGLE, SNGenius::GetInstance().GetScreenWidth() / SNGenius::GetInstance().GetScreenHeight(), EngineConstants::NEAR_PLANE, EngineConstants::FAR_PLANE);
	camera->SetPosition(glm::vec3(0.0f, 100.0f, 0.0f));
	camera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	camera->SetUp(glm::vec3(1.0f, 0.0f, 0.0f));
	mMapPass = new RenderPass(camera, IRenderer::LAYER_OTHER);
	SNGenius::GetInstance().AddCamera(camera);
	
	IFrameBuffer* frameBuffer = new IFrameBuffer(static_cast<int>(SNGenius::GetInstance().GetScreenWidth()), static_cast<int>(SNGenius::GetInstance().GetScreenHeight()));
	frameBuffer->SetColorTextureAttachment(0, static_cast<Texture*>(SNGenius::GetInstance().GetTexture("map")));
	frameBuffer->Init();
	mMapPass->SetFrameBufferOutput(frameBuffer);
	SNGenius::GetInstance().AddRenderPass(mMapPass, false);
}

void CreateTerrainRenderPass()
{
	//RENDER PASS GAMEPLAY
	RenderPass *terrainPass = new RenderPass(static_cast<ICamera*>(mGameplayCamera), IRenderer::LAYER_TERRAIN);
	terrainPass->EnableFog(true);
	SNGenius::GetInstance().AddRenderPassAt(1, terrainPass, false);
}

void CreateSubSystems()
{
	//CreateHudMapRenderPass();

	CreateTerrainRenderPass();
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
	const Statistics* statistics = SNGenius::GetInstance().GetStatistics();

	MaterialEffectText* effect = materialText->GetEffect<MaterialEffectText>();
	int fps = static_cast<int>(statistics->GetNumberFPS());
	if (fps < EngineConstants::MIN_FPS_ALLOWED)
	{
		effect->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}
	else
	{
		effect->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}

	unsigned int numGameEntities = statistics->GetNumberGameEntities();
	unsigned int numGameEntitiesInsideSpacePartition = statistics->GetNumberGameEntitiesInsideSpacePartition();

	mText[0]->UpdateText(texts[0] + std::to_string(fps));
	mText[1]->UpdateText(texts[1] + std::to_string(statistics->GetNumberTrianglesRendered()));
	mText[2]->UpdateText(texts[2] + std::to_string(statistics->GetNumberDrawCalls()));
	mText[3]->UpdateText(texts[3] + std::to_string(numGameEntities));
	mText[4]->UpdateText(texts[4] + std::to_string(numGameEntitiesInsideSpacePartition));
	mText[5]->UpdateText(texts[5] + std::to_string(statistics->GetNumberRenderers()));
	mText[6]->UpdateText(texts[6] + std::to_string(statistics->GetNumberGameEntitiesWithPhysics()));
	mText[7]->UpdateText(texts[7] + std::to_string(statistics->GetDayTime()));

	//std::cout << "entities rendered: " << statistics->GetNumberRenderers() << "\n";
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

void UpdateQuadTreeBox()
{
	
	glm::vec3 position = mQuadTreeMovedEntity->GetTransformation()->GetPosition();
	glm::vec3 scale = mQuadTreeMovedEntity->GetTransformation()->GetScale();

	//mQuadTree.RemoveGameEntity(mQuadTreeMovedEntity);

	scale.x = mQuadMovingScale.x * 2.0f;
	scale.y = mQuadMovingScale.x * 2.0f;
	scale.z = mQuadMovingScale.x * 2.0f;

	mQuadTreeMovedEntity->GetTransformation()->SetScale(scale);
	mQuadTreeMovedEntity->GetTransformation()->SetPosition(mQuadMovingPosition);

	//mQuadTree.AddGameEntity(mQuadTreeMovedEntity);
	Log(Log::LOG_INFO) << "num elements in quadtree: " << mQuadTree.GetNumEntities() << "\n";
	
	for (GameEntity* entity : mQuadTreeEntities)
	{
		DebugComponent* debugComponent = entity->GetComponent<DebugComponent>();
		if (debugComponent != nullptr)
		{
			debugComponent->SetEnabled(true);
		}
	}

	std::vector<GameEntity*> result;
	mQuadTree.Query(mQuadTreeMovedEntity->GetRenderer()->GetAABB(), result);

	for (GameEntity* entity : result)
	{
		DebugComponent* debugComponent = entity->GetComponent<DebugComponent>();
		if (debugComponent != nullptr)
		{
			debugComponent->SetEnabled(false);
		}
	}
}

void UpdateCameraAABB()
{
	/*
	glm::vec3 position(0.0f);
	glm::vec3 rotation(0.0f);
	glm::vec3 scale(1.0f);

	rotation = mPlayer->GetTransformation()->GetRotation();
	glm::mat4 matrix(1.0f);
	matrix = glm::rotate(matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec3 targetPos = mPlayer->GetTransformation()->GetPosition();
	glm::vec3 forward = matrix * glm::vec4(0.0f, 0.0f, 10.0f, 0.0f);
	forward.y = 0.0f;
	mCameraTargetEntity->GetTransformation()->SetPosition(targetPos + forward);
	
	mCameraTest->SetTarget(targetPos + forward);
	mCameraTest->SetPosition(targetPos);*/
	
	AABB aabb = mGameplayCamera->GetAABB();// mCameraTest->GetAABB();
	mCameraAABBEntity->GetTransformation()->SetPosition(aabb.GetCenter());
	mCameraAABBEntity->GetTransformation()->SetScale(aabb.GetSize());
}

void UpdateInput(GLFWwindow* window)
{
	if (!mIsShooterGameRunning)
	{
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			if (!mIsGameplayCameraEnabled)
			{
				mGameplayPass->SetCamera(mGameplayCamera);
			}
			else
			{
				mGameplayPass->SetCamera(mEagleEyeCamera);
			}

			mIsGameplayCameraEnabled = !mIsGameplayCameraEnabled;
		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			mIsShadowEnabled = !mIsShadowEnabled;
			SNGenius::GetInstance().SetCastingShadowsEnabled(mIsShadowEnabled);
		}
		else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			if (mCurrentCommand == nullptr)
			{
				mCurrentCommand = new RiseTerrainCommand(mTerrain);
			}
		}
		else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			mIsSpacePartitionEnabled = !mIsSpacePartitionEnabled;
			SNGenius::GetInstance().SetIsSpacePartitionEnabled(mIsSpacePartitionEnabled);
		}

		if (mConfiguration == QUADTREE)
		{
			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			{
				mQuadMovingPosition.x += 0.01f;
				UpdateQuadTreeBox();
			}
			else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				mQuadMovingPosition.x -= 0.01f;
				UpdateQuadTreeBox();
			}
			else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			{
				mQuadMovingPosition.z += 0.01f;
				UpdateQuadTreeBox();
			}
			else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			{
				mQuadMovingPosition.z -= 0.01f;
				UpdateQuadTreeBox();
			}
			else if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
			{
				mQuadMovingScale.x += 0.01f;
				UpdateQuadTreeBox();
			}
			else if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
			{
				mQuadMovingScale.x -= 0.01f;
				UpdateQuadTreeBox();
			}
		}

		if (mCurrentCommand != nullptr)
		{
			mCurrentCommand->Execute();
		}
	}
}

void Update(float elapsedTime)
{
	if (mIsShooterGameRunning)
	{
		mGame.Update(elapsedTime);
	}
	else
	{
		UpdateCommand(elapsedTime);

		if (mIsEnergyWallEnabled)
		{
			UpdateEnergyWallCollisions(elapsedTime);
		}
		if (mConfiguration == QUADTREE_WITH_CAMERA)
		{
			UpdateCameraAABB();
		}
	}

	/*if (mIsStatisticsVisible && mIsTextEnabled)
	{
		UpdateStatitstics();
	}*/
}

void Init(NGenius* engine)
{
	mGame.Init(engine);
}

void Start(NGenius* engine)
{
	if (mIsShooterGameRunning)
	{
		mGame.Start(engine);
		mScene = SNGenius::GetInstance().GetGameScene(GAME_SCENE_NAME);
	}
	else
	{
		mScene = SNGenius::GetInstance().CreateGameScene("mainScene");
		CreateSubSystems();
		CreateEntities();		

		//green island
		SNGenius::GetInstance().AddSunLightFrame(1200.0f, 90.0f, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(255.0f, 255.0f, 255.0f) / 255.0f, 0.004f, 1.5f, "day_cubemap");
		SNGenius::GetInstance().AddSunLightFrame(1800.0f, 135.0f, glm::vec3(0.93f, 0.64f, 0.78f), glm::vec3(218.0f, 74.0f, 43.0f) / 255.0f, 0.04f, 1.5f, "day_cubemap");
		SNGenius::GetInstance().AddSunLightFrame(2400.0f, 270.0f, glm::vec3(0.86f, 0.64f, 0.93f), glm::vec3(0.0f), 0.004f, 1.5f, "night_cubemap");
		SNGenius::GetInstance().AddSunLightFrame(600.0f, 45.0f, glm::vec3(0.36f, 0.73f, 0.82f), glm::vec3(93.0f, 188.0f, 210.0f) / 255.0f, 0.08f, 1.5f, "day_cubemap");


		SNGenius::GetInstance().SetCastingShadowsTarget(mPlayer);
	}
	
	/*
	if (mIsTextEnabled)
	{
		CreateTextTest();
	}*/
}

void SetupConfiguration()
{
	switch (mConfiguration)
	{
	case TEST:
		mIsDebugModeEnabled = true;
		mIsWaterEnabled = false;
		mIsGameplayCameraEnabled = true;
		mIsFogEnabled = true;
		mIsVegetationEnabled = false;
		mIsPropsEnabled = false;
		mIsEnergyWallEnabled = false;
		mIsSkyboxEnabled = false;
		mIsTerrainFlat = true;
		//mWaterHeight = 0.0f;
		mIsTextEnabled = true;
		mIsStatisticsVisible = true;
		mIsParticlesEnabled = false;
		mIsShadowEnabled = true;
		break;
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
		mIsWaterEnabled = false;
		mIsGameplayCameraEnabled = true;
		mIsFogEnabled = true;
		mIsVegetationEnabled = false;
		mIsPropsEnabled = false;
		mIsEnergyWallEnabled = false;
		mIsSkyboxEnabled = false;
		mIsTerrainFlat = true;
		mIsTextEnabled = false;
		mIsStatisticsVisible = false;
		mIsParticlesEnabled = false;
		mIsShadowEnabled = false;
		mIsFullScreen = false;
		mWaterHeight = 0.0f;
		break;
	case QUADTREE:
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
	case QUADTREE_WITH_CAMERA:
		mIsDebugModeEnabled = true;
		mIsWaterEnabled = false;
		mIsGameplayCameraEnabled = true;
		mIsFogEnabled = false;
		mIsVegetationEnabled = true;
		mIsPropsEnabled = true;
		mIsEnergyWallEnabled = false;
		mIsSkyboxEnabled = false;
		mIsTerrainFlat = true;
		mIsTextEnabled = true;
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
		mIsParticlesEnabled = false;
		mIsShadowEnabled = true;
		mIsFullScreen = true;
		break;
	}
}

void Initialize()
{
	SetupConfiguration();

	SNGenius::GetInstance().Init("Demo", EngineConstants::SCREEN_WIDTH, EngineConstants::SCREEN_HEIGHT, mIsFullScreen);
	SNGenius::GetInstance().RegisterInputHandler(std::bind(&UpdateInput, std::placeholders::_1));
	SNGenius::GetInstance().RegisterUpdateHandler(std::bind(&Update, std::placeholders::_1));
	SNGenius::GetInstance().RegisterStartHandler(std::bind(&Start, std::placeholders::_1));
	SNGenius::GetInstance().RegisterInitHandler(std::bind(&Init, std::placeholders::_1));

	CreateCameras();

	SNGenius::GetInstance().SetCastingShadowsEnabled(mIsShadowEnabled);

	SNGenius::GetInstance().SetWaterEnabled(mIsWaterEnabled);
	SNGenius::GetInstance().SetWaterParameters(mGameplayCamera, mWaterHeight);

	SNGenius::GetInstance().SetDebugModeEnabled(mIsDebugModeEnabled);

	glfwSetScrollCallback(SNGenius::GetInstance().GetGLWindow(), &ScrollCallback);
	glfwSetKeyCallback(SNGenius::GetInstance().GetGLWindow(), &KeyCallback);
	glfwSetMouseButtonCallback(SNGenius::GetInstance().GetGLWindow(), &MouseButtonCallback);
	glfwSetCursorPosCallback(SNGenius::GetInstance().GetGLWindow(), &MouseCursorPosCallback);

	SNGenius::GetInstance().SetFogEnabled(mIsFogEnabled);

	SNGenius::GetInstance().SetGUIEnabled(true);
	SNGenius::GetInstance().SetGameplayEnabled(true);
	SNGenius::GetInstance().SetParticlesEnabled(true);
	SNGenius::GetInstance().SetTransparentEnabled(true);

	SNGenius::GetInstance().Start();
}

int main(void)
{
	Initialize();

	SNGenius::GetInstance().Run();

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