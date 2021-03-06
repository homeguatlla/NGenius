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
#include <memory>

#include "GameConstants.h"
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
#include "src/resources/entities/player/Player.h"
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

#include "src/resources/IGameEntity.h"
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
#include "src/resources/components/SpacePartitionComponent.h"
#include "src/resources/components/EnvironmentAffectedComponent.h"
#include "src/resources/components/EnvironmentModificatorComponent.h"
#include "src/resources/components/AnimationComponent.h"

#include "src/resources/command/ICommand.h"
#include "src/resources/command/commands/RiseTerrainCommand.h"
#include "src/resources/command/commands/ApplyForceToEntityCommand.h"
#include "src/resources/command/commands/EnableDisablePhysicsEngineCommand.h"

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


const float Pi = 3.141592f;


ICamera* mGameplayCamera;
ICamera* mEagleEyeCamera;
RenderPass *mGameplayPass; 
ThirdPersonCameraComponent* mThirdPersonCameraComponent;

RenderPass* mMapPass;
//Light* mSunLight;
Terrain* mTerrain;
Player* mPlayer;
Water* mWater;
IGameEntity* mCamera;
IGameEntity* mQuadTreeBox;
GameScene* mScene;

EnergyWall* mEnergyWall;

float mEnergyWallRadius = 44.0f;
glm::vec3 mEnergyWallPosition(0.0f, 0.0f, 0.0f);

const std::vector<std::string> texts = { "FPS: ", "Triangles: ", "Drawcalls: ", "GameEntities(GE): ", "GESpacePartition:", "GERendered:", "GEWithPhysics: ", "DayTime: "};
Text* mText[8];
IMaterial* materialText;

ICommand* mCurrentCommand = nullptr;

//camera bounding box
IGameEntity* mCameraAABBEntity;
IGameEntity* mCameraTargetEntity;

//for QUADTREE setup
float aabbSize = 2.0f;
const AABB mAABB(glm::vec3(-aabbSize, 0.0f, -aabbSize), glm::vec3(aabbSize, 0.0f, aabbSize));
GameEntityQuadTree mQuadTree(mAABB);
glm::vec3 mQuadMovingPosition(0.0f);
glm::vec3 mQuadMovingScale(1.0f);
IGameEntity* mQuadTreeMovedEntity;
std::vector<IGameEntity*> mQuadTreeEntities;


std::shared_ptr<NGenius> mEngine;
bool mCanUpdatePhysicsEngine = false;

double aleatori()
{
	int r = rand();
	return double(r % 100) / 100;
}

/*
void CreateTerrainNormals(vector<glm::vec3>& vertexs, int numVertexsSide)
{
	IShaderProgram* defaultShaderProgram = mEngine->GetShader("s_default");
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
	IRenderer* guiShadowRenderer = new GUIRenderer(	mEngine->GetShader("s_gui"),
														static_cast<Texture*>(mEngine->GetTexture("shadow_texture")),
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
	IRenderer* guiReflectionRenderer = new GUIRenderer(	mEngine->GetShader("s_gui"),
															static_cast<Texture*>(mEngine->GetTexture("reflection_water")),
															128.0f,
															128.0f
														);
	GameEntity* quadReflection = new GameEntity(	new Transformation(glm::vec3(420.0f, -300.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
		guiReflectionRenderer
												);
	mScene->AddGameEntity(quadReflection);

	IRenderer* guiRefractionRenderer = new GUIRenderer(mEngine->GetShader("s_gui"),
															static_cast<Texture*>(mEngine->GetTexture("refraction_depth_water")),
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
		ModelNormalMapRenderer* modelRenderer = new ModelNormalMapRenderer(mEngine->GetModel("cube"),
			mEngine->GetShader("s_model"),
			static_cast<Texture*>(mEngine->GetTexture("cube_diffuse")),
			static_cast<Texture*>(mEngine->GetTexture("cube_normalmap")),
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
/*
GameEntity* CreateModelWithLod(const glm::vec3& position, const glm::vec3& scale, const std::vector<std::string>& models, const std::vector<std::pair<float, bool>>& lod, IMaterial* material, IMaterial* materialNormalmap, bool isCullingEnabled)
{
	GameEntity* modelEntity = DBG_NEW GameEntity(
		DBG_NEW Transformation(position, glm::vec3(0.0f), scale),
		nullptr
		);

	modelEntity->AddComponent(DBG_NEW PhysicsComponent(true, PhysicsSystem::GRAVITY_VALUE));
	modelEntity->AddComponent(DBG_NEW CollisionComponent());
	modelEntity->AddComponent(DBG_NEW SpacePartitionComponent());
	IRenderer* boundingBoxRenderer = DBG_NEW WireframeRenderer(mEngine->GetModel("cube"), mEngine->GetMaterial(MaterialsLibrary::WIREFRAME_MATERIAL_NAME));
	modelEntity->AddComponent(DBG_NEW DebugComponent(boundingBoxRenderer));

	LODComponent* lodComponent = DBG_NEW LODComponent(mGameplayCamera);
	modelEntity->AddComponent(lodComponent);
	//WARNING!!! debemos a�adir el componente a la entity primero para que se le asigne el parent, a partir de ah�, agregar los dem�s lod que ya les asignar� el padre.

	for (unsigned int i = 0; i < models.size(); ++i)
	{
		IMaterial* m = material;
		if (i == 0 && materialNormalmap != nullptr)
		{
			m = materialNormalmap;
		}

		IRenderer* renderer = DBG_NEW VerticesRenderer(mEngine->GetModel(models[i]), m);
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
	IRenderer* renderer = DBG_NEW VerticesRenderer(model, material);

	GameEntity* modelEntity = DBG_NEW GameEntity(
												DBG_NEW Transformation(position, rotation, scale),
												renderer
											);

	modelEntity->AddComponent(DBG_NEW PhysicsComponent(true, PhysicsSystem::GRAVITY_VALUE));
	modelEntity->AddComponent(DBG_NEW CollisionComponent());
	modelEntity->AddComponent(DBG_NEW SpacePartitionComponent());

	IRenderer* boundingBoxRenderer = DBG_NEW WireframeRenderer(mEngine->GetModel("cube"), mEngine->GetMaterial(MaterialsLibrary::WIREFRAME_MATERIAL_NAME));
	modelEntity->AddComponent(DBG_NEW DebugComponent(boundingBoxRenderer));

	return modelEntity;
}

GameEntity* CreateQuadTreeBoxEntity(float size, glm::vec3 position, glm::vec3 color)
{
	IMaterial* material = mEngine->GetMaterial(MaterialsLibrary::QUADTREE_MATERIAL_NAME);
	material->AddEffect(DBG_NEW MaterialEffectFloat3(color));

	IRenderer* renderer = DBG_NEW WireframeRenderer(mEngine->GetModel("cube"), material);
	GameEntity* quad = DBG_NEW GameEntity(
		DBG_NEW Transformation(position, glm::vec3(0.0f), glm::vec3(size * 2.0f)),
		renderer
	);
	IRenderer* boundingBoxRenderer = DBG_NEW WireframeRenderer(mEngine->GetModel("cube"), mEngine->GetMaterial(MaterialsLibrary::WIREFRAME_MATERIAL_NAME));
	quad->AddComponent(DBG_NEW DebugComponent(boundingBoxRenderer));

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
	
	IMaterial* materialFoliage = mEngine->GetMaterial("tree_foliage");
	IMaterial* materialTrunk = mEngine->GetMaterial("tree_trunk");
	IMaterial* materialTrunkNormalmap = mEngine->GetMaterial("tree_trunk_normalmap");
	
	for (unsigned long i = 0; i < positions.size(); i++)
	{
		GameEntity* entity = CreateModelWithLod(positions[i], sizes[i], modelsFoliage, lod, materialFoliage, nullptr, true);
		mScene->AddEntity(entity);

		entity = CreateModelWithLod(positions[i], sizes[i], modelsTrunk, lod, materialTrunk, materialTrunkNormalmap, true);
		mScene->AddEntity(entity);
	}
}

void CreatePoints()
{
	glPointSize(5.0f);
	IMaterial* material = mEngine->GetMaterial("grass2_material");
	PointsPatch* pointsPatch = DBG_NEW PointsPatch(	DBG_NEW Transformation(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)), 
												material, mTerrain, mWaterHeight + 0.2f, mWaterHeight + 0.8f, 50.0f, 50.0f, 150.0f);

	EnvironmentAffectedComponent* environmentComponent = DBG_NEW EnvironmentAffectedComponent();
	environmentComponent->SetAffectedByWind(true);
	pointsPatch->AddComponent(environmentComponent);
	mScene->AddEntity(pointsPatch);
}

void CreateProps()
{
	int areaSize = 5;
	int numProps = 6;

	std::vector<std::string> models;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> rotations;
	std::vector<float> scales;

	//crear una estructura con los 4 datos, rotacion, escalado, traslacion y nombre y as� los tenemos siempre bien puestos
	models.push_back(std::string("barrel"));
	models.push_back(std::string("chest"));
	models.push_back(std::string("brazier"));
	models.push_back(std::string("barrel"));
	models.push_back(std::string("crate"));
	models.push_back(std::string("statue"));
	models.push_back(std::string("wood"));
	models.push_back(std::string("bridge"));

	//models.push_back(std::string("floor"));
	
	
	//positions.push_back(glm::vec3(5.0f, 0.0f, 0.0f));
	//positions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	//positions.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	//positions.push_back(glm::vec3(0.0f, 0.0f, 5.0f));
	//positions.push_back(glm::vec3(0.0f, 0.0f, -5.0f));
	//
	//int extraProps = 0;
	//numProps += extraProps;
	//for (int i = 0; i < extraProps; ++i)
	//{
	//	float x = static_cast<float>(rand() % 7 * (1 - 2 * (rand() % 2)));
	//	float z = static_cast<float>(rand() % 7 * (1 - 2 * (rand() % 2)));

	//	positions.push_back(glm::vec3(x, 0.0f, z));
	//}

	
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

	IMaterial* material = mEngine->GetMaterial("props_material");
	
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
		Model* model = mEngine->GetModel(modelName);

		GameEntity* entity = CreateModel(position, scale, rotation, model, material);
		mScene->AddEntity(entity);
	}
}

void CreateWater()
{
	//WATER
	if (mIsWaterEnabled)
	{
		IMaterial* material = mEngine->GetMaterial("water");
		mWater = DBG_NEW Water(		DBG_NEW Transformation(
													glm::vec3(4.0f, mWaterHeight, 4.5f), 
													glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), 
													glm::vec3(1.0f)),
								material,
								50.0f,
								50.0f,
								material->GetEffect<MaterialEffectWater>()->GetSpeed()
							);
		mScene->AddEntity(mWater);
	}
}

Particle* CreateParticle(bool canCollide, std::string& materialName, glm::vec3& gravity)
{
	IMaterial* material = mEngine->GetMaterial(materialName);
	Particle* particle = DBG_NEW Particle(DBG_NEW Transformation(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.1f)),
		mEngine->GetModel("particle_quad"),
		material,
		6.0f);
	PhysicsComponent* physicsComponent = DBG_NEW PhysicsComponent(false, gravity);
	particle->AddComponent(physicsComponent);

	if (canCollide)
	{
		particle->AddComponent(DBG_NEW CollisionComponent());
	}

	return particle;
}

void CreateParticlesFire()
{
	std::string materialName("smoke_particle");
	Particle* particle = CreateParticle(false, materialName, glm::vec3(0.0f));
	particle->SetLiveTime(2.0f);

	float x = 4.0f;
	float z = -1.7f;

	float height = mTerrain->GetHeight(glm::vec2(x, z)) + 0.7f;

	ParticlesEmitter* particlesEmitter = DBG_NEW ParticlesEmitter(	particle,
																DBG_NEW Transformation(glm::vec3(x, height, z), glm::vec3(0.0f), glm::vec3(0.1f)),
																nullptr,
																100);
	particlesEmitter->SetColorGradientValues(glm::vec4(1.0f, 1.0f, 0.25f, 1.0f), glm::vec4(1.0f, 0.2f, 0.1f, 0.4f));	
	particlesEmitter->SetScaleValues(0.3f, 0.01f);
	particlesEmitter->SetVelocity(glm::vec3(0.0f), glm::vec3(0.04f, 0.4f, 0.04f));
	particlesEmitter->SetSpawnArea(glm::vec3(-0.06f, 0.0f, -0.06f), glm::vec3(0.06f, 0.0f, 0.06f));
	mScene->AddEntity(particlesEmitter);
	mEngine->AddParticleEmitter(particlesEmitter);
}

void CreateEnergyWall()
{
	IMaterial* material = mEngine->GetMaterial("energy_wall");
	mEnergyWall = DBG_NEW EnergyWall(	DBG_NEW Transformation(mEnergyWallPosition, glm::vec3(0.0f), glm::vec3(mEnergyWallRadius)),
									material,
									mEngine->GetModel("sphere"),
									2.0f
								);
	mScene->AddEntity(mEnergyWall);
	mEngine->SetEnergyWall(mEnergyWallPosition, mEnergyWallRadius);
}

void CreateHUD()
{
	//QUAD
	IMaterial* material = mEngine->GetMaterial(MaterialsLibrary::GUI_MATERIAL_NAME);
	material->AddEffect(DBG_NEW MaterialEffectDiffuseTexture(mEngine->GetTexture("hud_map"), glm::vec3(1.0f), 1.0f));
	IRenderer* guiRenderer = DBG_NEW IndicesRenderer(mEngine->GetModel("gui_quad"), material);
	guiRenderer->SetLayer(IRenderer::LAYER_GUI);

	float x = mEngine->GetScreenWidth() * 0.5f * 0.90f;
	float y = -mEngine->GetScreenHeight() * 0.5f * 0.85f;
	GameEntity* quad = DBG_NEW GameEntity(	DBG_NEW Transformation(glm::vec3(x, y, 0.0f), 
										glm::vec3(0.0f), 
										glm::vec3(256.0f)),
										guiRenderer
									);
	mScene->AddEntity(quad);

	
	//IRenderer* mapRenderer = DBG_NEW GUIRenderer(mEngine->GetShader("s_gui"),
	//	static_cast<Texture*>(mEngine->GetTexture("map")),
	//	87.0f,
	//	73.0f
	//);
	//GameEntity* map = DBG_NEW GameEntity(DBG_NEW Transformation(glm::vec3(420.0f, -300.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
	//	mapRenderer
	//);
	//mScene->AddGameEntity(map);
}

void CreateParticlesTest()
{
	glm::vec3 gravity = PhysicsSystem::GRAVITY_VALUE;
	std::string materialName = "smoke_particle";
	Particle* particle = CreateParticle(false, materialName, glm::vec3(0.0f));
	particle->SetLiveTime(10.0f);

	glm::vec3 position(3.0f, 0.0f, 4.0f); 
	float height = mTerrain->GetHeight(glm::vec2(position.x, position.z)) + 0.01f;
	position.y = height + 0.3f;
	

	ParticlesEmitter* particlesEmitter = DBG_NEW ParticlesEmitter(	particle,
																DBG_NEW Transformation(position, glm::vec3(0.0f), glm::vec3(1.0f)),
																nullptr,
																10);
	particlesEmitter->SetColorGradientValues(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	particlesEmitter->SetScaleValues(1.4f + (rand() % 4) / 10.0f, 0.5f);
	particlesEmitter->SetVelocity(glm::vec3(0.1f, 0.0f, 0.0f), glm::vec3(0.3f, 0.0f, 0.0f));
	particlesEmitter->SetSpawnArea(glm::vec3(-3.f, 0.0f, -3.0f), glm::vec3(3.f, 0.0f, 3.0f));
	
	mScene->AddEntity(particlesEmitter);
	mEngine->AddParticleEmitter(particlesEmitter);	
	
	particle = CreateParticle(false, materialName, glm::vec3(0.0f));
	particle->SetLiveTime(1.0f);

	position = glm::vec3(4.0f, 0.0f, 4.0f);
	height = mTerrain->GetHeight(glm::vec2(position.x, position.z)) + 0.01f;
	position.y = height;
	particlesEmitter = DBG_NEW ParticlesEmitter(	particle,
												DBG_NEW Transformation(position, glm::vec3(0.0f), glm::vec3(0.1f)),
												nullptr,
												50);
	particlesEmitter->SetColorGradientValues(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	particlesEmitter->SetScaleValues(0.03f, 0.4f + (rand() % 4) / 10.0f);
	particlesEmitter->SetVelocity(glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f));
	mScene->AddEntity(particlesEmitter);
	mEngine->AddParticleEmitter(particlesEmitter);
}

void CreateTextTest()
{
	FontType* font = mEngine->GetFont("OCR A Extended");

	materialText = mEngine->GetMaterial(MaterialsLibrary::TEXT_MATERIAL_NAME);
	materialText->AddEffect(DBG_NEW MaterialEffectDiffuseTexture(font->GetTexture(), glm::vec3(1.0f), 1.0f));
	materialText->AddEffect(DBG_NEW MaterialEffectText(	glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
													glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
													0.4f,
													0.1f,
													0.0f,
													0.0f,
													glm::vec2(0.0f)));

	for (unsigned int i = 0; i < texts.size(); ++i)
	{
		mText[i] = DBG_NEW Text(
								DBG_NEW Transformation(	glm::vec3(-mEngine->GetScreenWidth() * 0.5f, mEngine->GetScreenHeight() * 0.5f - i * 20.0f, 0.0f),
								glm::vec3(0.0f),
								glm::vec3(0.70f)
							),
			materialText, font,
			texts[i], false, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1, 1, false);
		mScene->AddEntity(mText[i]);
	}
	//float x = 0.0f;
	//float z = 0.0f;
	//float height = mTerrain->GetHeight(glm::vec2(x, z)) + 1.0f;

	//IMaterial* material3D = mEngine->GetMaterial(MaterialsLibrary::TEXT3D_MATERIAL_NAME);
	//material3D->AddEffect(DBG_NEW MaterialEffectDiffuseTexture(font->GetTexture(), glm::vec3(1.0f), 1.0f));
	//material3D->AddEffect(DBG_NEW MaterialEffectText(	glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
	//												glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
	//												0.4f,
	//												0.1f,
	//												0.3f,
	//												0.6f,
	//												glm::vec2(0.0f)));

	//Text* mTestText = DBG_NEW Text(	DBG_NEW Transformation(glm::vec3(x, height, z), glm::vec3(0.0f), glm::vec3(0.01f)),
	//							material3D, font,
	//							"Origin", true, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 1, 1, false);
	//mScene->AddEntity(mTestText);

	//x = 10.0f;
	//z = 10.0f;
	//height = mTerrain->GetHeight(glm::vec2(x, z)) + 1.0f;

	//mTestText = DBG_NEW Text(DBG_NEW Transformation(glm::vec3(x, height, z), glm::vec3(0.0f), glm::vec3(.01f)),
	//	mEngine->GetShader("s_text"), mEngine->GetFont("OCR A Extended"),
	//	"Market", true, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), 1, 1, false);
	//mTestText->SetOutlineColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.8f));
	//mTestText->SetBorderParameters(0.5f, 0.1f, 0.5f, 0.4f);
	//mTestText->SetShadow(glm::vec2(0.002f, 0.002f));
	//mScene->AddGameEntity(mTestText);
}

void CreateTerrain()
{
	//TERRAIN GAME ENTITY
	//mSunCamera = DBG_NEW OrthogonalCamera(20.0f, 20.0f, -10.0f, 20.0f);
	//glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	//glm::vec3 direction = glm::vec3(camera->GetTarget() - glm::vec3(100000.0f, 100000.0f, 100000.0f));
	//glm::mat4 depthViewMatrix = glm::lookAt(direction, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	//glm::mat4 depthModelMatrix = glm::mat4(1.0);
	//glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;

	float scale = mIsTerrainFlat ? 0.0f : TERRAIN_SCALE;

	IMaterial* material = mEngine->GetMaterial("terrain");
	mTerrain = DBG_NEW Terrain(	DBG_NEW Transformation(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
							material,
							static_cast<Texture*>(mEngine->GetTexture("terrain_heightmap")),
							scale);

	mTerrain->SetFlat(mIsTerrainFlat);
			
	mScene->AddEntity(mTerrain);
	mEngine->SetTerrain(mTerrain);

	//TERRAIN NORMALS ENTITY
	//CreateTerrainNormals(vertexs, numVertexsSide);
}

void CreatePlayer()
{
	//TODO texto color std::cout << "\033[1;31mbold red text\033[0m normal text\n";
	//PLAYER
	IMaterial* material = mEngine->GetMaterial("player");

	InputComponent* inputComponent = DBG_NEW InputComponent();
	inputComponent->AddConverter(DBG_NEW KeyToEventBind(GLFW_KEY_W, DBG_NEW ForwardEvent()));
	inputComponent->AddConverter(DBG_NEW KeyToEventBind(GLFW_KEY_S, DBG_NEW BackwardEvent()));
	inputComponent->AddConverter(DBG_NEW MouseToEventBind(-1, DBG_NEW TurnEvent()));
	inputComponent->AddConverter(DBG_NEW KeyToEventBind(GLFW_KEY_SPACE, DBG_NEW JumpEvent()));
	inputComponent->AddConverter(DBG_NEW MouseToEventBind(GLFW_MOUSE_BUTTON_MIDDLE, DBG_NEW ZoomEvent()));

	Model* model = mEngine->GetModel("farmer");
	//IRenderer* renderer = DBG_NEW VerticesRenderer(model, material);
	IRenderer* renderer = DBG_NEW IndicesRenderer(model, material);

	mPlayer = DBG_NEW Player(	DBG_NEW Transformation(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f), glm::vec3(0.18f)),
							renderer,
							inputComponent,
							DBG_NEW CharacterComponent(),
							DBG_NEW PhysicsComponent(false, PhysicsSystem::GRAVITY_VALUE),
							DBG_NEW CollisionComponent(),
							PLAYER_RUN_SPEED,
							PLAYER_TURN_SPEED,
							PLAYER_UPWARDS_HEIGHT
						);
	mPlayer->AddComponent(DBG_NEW EnergyWallCollisionComponent());
	IRenderer* boundingBoxRenderer = DBG_NEW WireframeRenderer(mEngine->GetModel("cube"), mEngine->GetMaterial(MaterialsLibrary::WIREFRAME_MATERIAL_NAME));
	mPlayer->AddComponent(DBG_NEW DebugComponent(boundingBoxRenderer));
	mPlayer->AddComponent(DBG_NEW EnvironmentModificatorComponent());
	AnimationComponent* animationComponent = DBG_NEW AnimationComponent();
	animationComponent->AddAnimation(mEngine->GetAnimation("animation_0"));
	mPlayer->AddComponent(animationComponent);
	mScene->AddEntity(mPlayer);
}

void CreateGameCameraEntity()
{
	InputComponent* inputComponent = DBG_NEW InputComponent();
	inputComponent->AddConverter(DBG_NEW MouseToEventBind(GLFW_MOUSE_BUTTON_MIDDLE, DBG_NEW ZoomEvent()));
	inputComponent->AddConverter(DBG_NEW MouseToEventBind(-1, DBG_NEW PitchEvent()));

	mCamera = DBG_NEW GameEntity(DBG_NEW Transformation(mGameplayCamera->GetPosition(), glm::vec3(0.0f), glm::vec3(0.0f)),
		nullptr);// DBG_NEW CubeRenderer(mEngine->GetShader("s_default")));

	glm::vec3 targetOffset(0.0f, 0.5f, 0.0f); //head
	mThirdPersonCameraComponent = DBG_NEW ThirdPersonCameraComponent(	static_cast<PerspectiveCamera*>(mGameplayCamera), 
																	mPlayer, 
																	targetOffset, 
																	4.0f, 
																	PLAYER_PITCH, 
																	PLAYER_PITCH_SPEED, 
																	PLAYER_ZOOM_SPEED);
	mCamera->AddComponent(mThirdPersonCameraComponent);
	mCamera->AddComponent(DBG_NEW CollisionComponent());

	mCamera->AddComponent(DBG_NEW CharacterComponent());
	mCamera->AddComponent(inputComponent);
	
	if (mIsWaterEnabled)
	{
		mCamera->AddComponent(DBG_NEW OverWaterComponent(mWaterHeight));
	}

	mScene->AddEntity(mCamera);
}

void CreateCameras()
{
	float screenWidth = static_cast<float>(mEngine->GetScreenWidth());
	float screenHeight = static_cast<float>(mEngine->GetScreenHeight());
	float aspectRatio = screenWidth / screenHeight;

	//CAMERA
	//mEagleEyeCamera = DBG_NEW PerspectiveCamera(VIEW_ANGLE, aspectRatio, NEAR_PLANE, FAR_PLANE);
	float factor = 1.0f;
	if (mConfiguration != QUADTREE && mConfiguration != QUADTREE_WITH_CAMERA)
	{
		factor = 20.0f;
	}
	else
	{
		factor = 50.0f;
	}
	mEagleEyeCamera = DBG_NEW OrthogonalCamera("eagle_camera", screenWidth/factor, screenHeight/factor, NEAR_PLANE, FAR_PLANE);
	mEagleEyeCamera->SetPosition(glm::vec3(0.0f, 15.0f, 0.0f));
	mEagleEyeCamera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	mEagleEyeCamera->SetUp(glm::vec3(1.0f, 0.0f, 0.0f));

	mEngine->AddCamera(mEagleEyeCamera);

	mGameplayCamera = DBG_NEW PerspectiveCamera("gameplay_camera", VIEW_ANGLE, aspectRatio, NEAR_PLANE, FAR_PLANE);
	mGameplayCamera->SetPosition(glm::vec3(0.0f, 5.0f, 5.0f));
	mGameplayCamera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	mGameplayCamera->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
	mEngine->AddCamera(mGameplayCamera);

	
	//if (mConfiguration == QUADTREE_WITH_CAMERA)
	//{
	//	mCameraTest = DBG_NEW PerspectiveCamera("gameplay_test", VIEW_ANGLE, aspectRatio, NEAR_PLANE, FAR_PLANE);
	//	mCameraTest->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	//	mCameraTest->SetTarget(glm::vec3(0.0f, 0.0f, 10.0f));
	//	mCameraTest->SetUp(glm::vec3(0.0f, 1.0f, 0.0f));
	//	mEngine->AddCamera(mCameraTest);
	//}
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
	
	//GameEntity* entity = CreateQuadTreeBoxEntity(1.3f, glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 1.0f));
	//mQuadTree.AddGameEntity(entity);
	//mQuadTreeEntities.push_back(entity);

	//entity = CreateQuadTreeBoxEntity(0.05f, glm::vec3(1.7f, 0.0f, 1.7f), glm::vec3(0.0f, 1.0f, 1.0f));
	//mQuadTree.AddGameEntity(entity);
	//mQuadTreeEntities.push_back(entity);

	//entity = CreateQuadTreeBoxEntity(0.1f, glm::vec3(-1.5f, 0.0f, -1.5f), glm::vec3(0.0f, 1.0f, 1.0f));
	//mQuadTree.AddGameEntity(entity);
	//mQuadTreeEntities.push_back(entity);

	//entity = CreateQuadTreeBoxEntity(0.8f, glm::vec3(-0.9f, 0.0f, 1.1f), glm::vec3(0.0f, 1.0f, 1.0f));
	//mQuadTree.AddGameEntity(entity);
	//mQuadTreeEntities.push_back(entity);

	//entity = CreateQuadTreeBoxEntity(0.2f, glm::vec3(-0.9f, 0.0f, 0.3f), glm::vec3(0.0f, 1.0f, 1.0f));
	//mQuadTree.AddGameEntity(entity);
	//mQuadTreeEntities.push_back(entity);
	
	
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
	mScene = mEngine->CreateGameScene("mainScene");
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

void CreateHudMapRenderPass()
{
	//HUD MAP RENDER PASS
	ICamera* camera = DBG_NEW PerspectiveCamera("map_camera", VIEW_ANGLE, mEngine->GetScreenWidth() / mEngine->GetScreenHeight(), NEAR_PLANE, FAR_PLANE);
	camera->SetPosition(glm::vec3(0.0f, 100.0f, 0.0f));
	camera->SetTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	camera->SetUp(glm::vec3(1.0f, 0.0f, 0.0f));
	mMapPass = DBG_NEW RenderPass("map_hud_render_pass", camera, IRenderer::LAYER_OTHER);
	mEngine->AddCamera(camera);
	
	IFrameBuffer* frameBuffer = DBG_NEW IFrameBuffer(static_cast<int>(mEngine->GetScreenWidth()), static_cast<int>(mEngine->GetScreenHeight()));
	frameBuffer->SetColorTextureAttachment(0, static_cast<Texture*>(mEngine->GetTexture("map")));
	frameBuffer->Init();
	mMapPass->SetFrameBufferOutput(frameBuffer);
	mEngine->AddRenderPass(mMapPass, false);
}

void CreateGameplayRenderPass()
{
	int screenWidth = static_cast<int>(mEngine->GetScreenWidth());
	int screenHeight = static_cast<int>(mEngine->GetScreenHeight());
	//RENDER PASS GAMEPLAY	
	ICamera* camera = mEngine->GetCamera("gameplay_camera");
	mGameplayPass = DBG_NEW RenderPass("gameplay_render_pass", camera, IRenderer::LAYER_OTHER | IRenderer::LAYER_WATER | IRenderer::LAYER_DEBUG);
	mGameplayPass->SetAcceptSpacePartitionOnly(true);

	IFrameBuffer* frameBuffer = DBG_NEW IFrameBuffer(screenWidth, screenHeight);
	Texture* depthTexture = static_cast<Texture*>(mEngine->GetTexture("depth_texture"));
	frameBuffer->SetCopyBufferToTexture(depthTexture, 0, 0, screenWidth, screenHeight);

	mGameplayPass->SetFrameBufferOutput(frameBuffer);
	mGameplayPass->EnableFog(true);
	
	//IMaterial* material = mEngine->GetMaterial("shadow");
	//material->AddEffect(DBG_NEW MaterialEffectDiffuseTexture(mEngine->GetTexture("tree_foliage_diffuse"), glm::vec3(0.0f), 1.0f));
	//mGameplayPass->SetMaterial(material);

	mEngine->AddRenderPass(mGameplayPass, false);
}

void CreateParticlesRenderPass()
{
	//RENDER PASS PARTICLES
	ICamera* camera = mEngine->GetCamera("gameplay_camera");
	RenderPass *particlesPass = DBG_NEW RenderPass("particles_render_pass", camera, IRenderer::LAYER_PARTICLES);
	particlesPass->SetCalculateDistanceToCamera(true);
	particlesPass->EnableFog(true);
	mEngine->AddRenderPass(particlesPass, false);
}

void CreateTransparentRenderPass()
{
	//RENDER PASS TRANSPARENT
	ICamera* camera = mEngine->GetCamera("gameplay_camera");
	RenderPass *transparentPass = DBG_NEW RenderPass("transparent_render_pass", camera, IRenderer::LAYER_TRANSPARENT);
	transparentPass->EnableFog(true);
	mEngine->AddRenderPass(transparentPass, false);
}

void CreateSubSystems()
{
	//CreateHudMapRenderPass();

	//CreateTerrainRenderPass();

	//CreateGameplayRenderPass();

	//CreateTransparentRenderPass();

	//CreateParticlesRenderPass();

	//CreateGUIRenderPass();
}

void UpdateEnergyWallCollisions(float elapsedTime)
{
	if (mPlayer != nullptr)
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
}

void UpdateStatitstics()
{
	const Statistics* statistics = mEngine->GetStatistics();

	if (materialText != nullptr)
	{
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
	
	//glm::vec3 position(0.0f);
	//glm::vec3 rotation(0.0f);
	//glm::vec3 scale(1.0f);

	//rotation = mPlayer->GetTransformation()->GetRotation();
	//glm::mat4 matrix(1.0f);
	//matrix = glm::rotate(matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	//matrix = glm::rotate(matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	//matrix = glm::rotate(matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	//glm::vec3 targetPos = mPlayer->GetTransformation()->GetPosition();
	//glm::vec3 forward = matrix * glm::vec4(0.0f, 0.0f, 10.0f, 0.0f);
	//forward.y = 0.0f;
	//mCameraTargetEntity->GetTransformation()->SetPosition(targetPos + forward);
	//
	//mCameraTest->SetTarget(targetPos + forward);
	//mCameraTest->SetPosition(targetPos);
	
	AABB aabb = mGameplayCamera->GetAABB();// mCameraTest->GetAABB();
	mCameraAABBEntity->GetTransformation()->SetPosition(aabb.GetCenter());
	mCameraAABBEntity->GetTransformation()->SetScale(aabb.GetSize());
}

void UpdateInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		mEngine->SaveToFile();
	}

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
		mEngine->SetCastingShadowsEnabled(mIsShadowEnabled);
	}
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		if (mCurrentCommand == nullptr)
		{
			mCurrentCommand = DBG_NEW RiseTerrainCommand(mTerrain);
		}
	} 
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		mIsSpacePartitionEnabled = !mIsSpacePartitionEnabled;
		mEngine->SetIsSpacePartitionEnabled(mIsSpacePartitionEnabled);
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
	if (mConfiguration == QUADTREE_WITH_CAMERA)
	{
		UpdateCameraAABB();
	}
}

*/

void UpdateInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		if (mCurrentCommand == nullptr)
		{
			std::shared_ptr<IGameEntity> entity = mEngine->GetGameEntity(std::string("box"));
			mCurrentCommand = DBG_NEW ApplyForceToEntityCommand(entity);
			mCurrentCommand->Execute();
			delete mCurrentCommand;
			mCurrentCommand = nullptr;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
	{
		if (mCurrentCommand == nullptr)
		{
			mCanUpdatePhysicsEngine = true;
			mCurrentCommand = DBG_NEW EnableDisablePhysicsEngineCommand(mEngine, mCanUpdatePhysicsEngine);
			mCurrentCommand->Execute();
			delete mCurrentCommand;
			mCurrentCommand = nullptr;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
	{
		if (mCurrentCommand == nullptr)
		{
			mCanUpdatePhysicsEngine = false;
			mCurrentCommand = DBG_NEW EnableDisablePhysicsEngineCommand(mEngine, mCanUpdatePhysicsEngine);
			mCurrentCommand->Execute();
			delete mCurrentCommand;
			mCurrentCommand = nullptr;
		}
	}
}

int main(int argc, char* argv[])
{
	std::string filename = "data/levels/test.xml";

	if (argc > 1)
	{
		filename = std::string(argv[1]);
	}

	mEngine = std::make_shared<NGenius>("Demo", SCREEN_WIDTH, SCREEN_HEIGHT);
	mEngine->SetFilename(filename);
	mEngine->RegisterInputHandler(std::bind(&UpdateInput, std::placeholders::_1));
	mEngine->Create();

	mCurrentCommand = DBG_NEW EnableDisablePhysicsEngineCommand(mEngine, false);
	mCurrentCommand->Execute();
	delete mCurrentCommand;
	mCurrentCommand = nullptr;

	mEngine->Run();

	mEngine.reset();

	_CrtDumpMemoryLeaks();

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