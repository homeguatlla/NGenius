#include "stdafx.h"
#include "MarsPlanet.h"
#include "../../NGenius.h"
#include "../ShooterGameConstants.h"
#include "../EngineConstants.h"

#include "../building/BuildingBuilder.h"

#include "../../resources/scene/GameScene.h"

#include "../../resources/materials/MaterialsLibrary.h"
#include "../../resources/materials/IMaterial.h"
#include "../../resources/materials/effects/MaterialEffectDiffuseTexture.h"
#include "../../resources/materials/effects/MaterialEffectNormalTexture.h"
#include "../../resources/materials/effects/MaterialEffectDirectionalLightProperties.h"
#include "../../resources/materials/effects/MaterialEffectFogProperties.h"
#include "../../resources/materials/effects/MaterialEffectShadowProperties.h"
#include "../../resources/materials/effects/MaterialEffectHeightMapTexture.h"
#include "../../resources/materials/effects/MaterialEffectTextureArray.h"
#include "../../resources/materials/effects/MaterialEffectClippingPlane.h"
#include "../../resources/materials/effects/MaterialEffectFloat.h"
#include "../../resources/materials/effects/MaterialEffectTextureCubemap.h"

#include "../../resources/textures/Texture.h"
#include "../../resources/textures/TextureArray.h"
#include "../../resources/textures/TextureCubemap.h"

#include "../../resources/renderers/IRenderer.h"
#include "../../resources/renderers/SkyBoxRenderer.h"
#include "../../resources/renderers/IndicesRenderer.h"
#include "../../resources/renderers/VerticesRenderer.h"
#include "../../resources/renderers/WireframeRenderer.h"
#include "../../resources/systems/renderSystem/RenderPass.h"

#include "../../resources/components/RotationComponent.h"
#include "../../resources/components/PhysicsComponent.h"
#include "../../resources/components/CollisionComponent.h"
#include "../../resources/components/SpacePartitionComponent.h"
#include "../../resources/components/DebugComponent.h"

#include "../../resources/systems/PhysicsSystem.h"

#include "../../resources/models/Model.h"


MarsPlanet::MarsPlanet(GameScene* scene, ICamera* gameplayCamera) :
	mEngine(&NGenius::GetInstance()),
	mScene(scene),
	mGameplayCamera(gameplayCamera)
{
	Create();
}

MarsPlanet::~MarsPlanet()
{
}

void MarsPlanet::Create()
{
	CreateTerrain();
	CreateSky();
	//CreateRocks();
	CreateCamp();
}

void MarsPlanet::CreateTerrain()
{
	//RENDER PASS TERRAIN
	RenderPass *terrainPass = new RenderPass(static_cast<ICamera*>(mGameplayCamera), IRenderer::LAYER_TERRAIN);
	terrainPass->EnableFog(true);
	mEngine->AddRenderPassAt(1, terrainPass, false);

	bool isFlat = true;

	float scale = isFlat ? 0.0f : TERRAIN_SCALE;

	IMaterial* material = mEngine->CreateMaterial("terrain", mEngine->GetShader("terrain"));
	material->AddEffect(new MaterialEffectDiffuseTexture(static_cast<Texture*>(mEngine->GetTexture("terrain_blendmap")), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f));
	material->AddEffect(new MaterialEffectDirectionalLightProperties());
	material->AddEffect(new MaterialEffectFogProperties());
	material->AddEffect(new MaterialEffectHeightMapTexture(static_cast<Texture*>(mEngine->GetTexture("terrain_heightmap")), 1.0f));
	material->AddEffect(new MaterialEffectTextureArray(static_cast<TextureArray*>(mEngine->GetTexture("terrain_mars_array"))));
	material->AddEffect(new MaterialEffectClippingPlane());
	material->AddEffect(new MaterialEffectShadowProperties(3));
	material->AddEffect(new MaterialEffectFloat(scale));

	mTerrain = new Terrain(new Transformation(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
		material,
		static_cast<Texture*>(mEngine->GetTexture("terrain_heightmap")),
		scale);

	mTerrain->SetFlat(isFlat);

	mScene->AddEntity(mTerrain);
	mEngine->SetTerrain(mTerrain);
}

void MarsPlanet::CreateSky()
{
	IMaterial* material = mEngine->CreateMaterial("skybox", mEngine->GetShader("skybox"));
	TextureCubemap* cubemap1 = static_cast<TextureCubemap*>(mEngine->GetTexture("day_cubemap"));
	TextureCubemap* cubemap2 = static_cast<TextureCubemap*>(mEngine->GetTexture("night_cubemap"));
	material->AddEffect(new MaterialEffectTextureCubemap(cubemap1, cubemap2, 0.0f));
	material->AddEffect(new MaterialEffectFogProperties());
	material->AddEffect(new MaterialEffectDirectionalLightProperties());

	SkyBoxRenderer* skyboxRenderer = new SkyBoxRenderer(mEngine->GetModel("skybox"), material);
	skyboxRenderer->SetLayer(IRenderer::LAYER_PARTICLES);

	mSkyBox = new GameEntity(
								new Transformation(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(200.0f)),
								skyboxRenderer
							);
	mSkyBox->AddComponent(new RotationComponent(glm::vec3(0.0f, 1.0f, 0.0f), EngineConstants::SKYBOX_ROTATION_SPEED));
	mScene->AddEntity(mSkyBox);
}

void MarsPlanet::CreateRocks()
{
	unsigned int numRocks = 400;
	float areaSize = 100.0f;
	std::vector<std::string> rockNames = {"rock2", "rock3", "rock5"};

	for (unsigned int i = 0; i < numRocks; ++i)
	{
		float randValue = (rand() % 1000) * areaSize / 1000.0f;
		float x = -areaSize * 0.5f + randValue;
		randValue = (rand() % 1000) * areaSize / 1000.0f;
		float z = -areaSize * 0.5f + randValue;
		float scale = 0.01f + (rand() % 10) /100.0f;
		float rotation = static_cast<float>(rand() % 360);
		float y = mEngine->GetHeight(glm::vec2(x, z));
		Transformation* transformation = new Transformation(glm::vec3(x, y, z), glm::vec3(0.0f, glm::radians(rotation), 0.0f), glm::vec3(scale));

		unsigned int index = rand() % rockNames.size();
		int maxCoef = index == 1 ? 2 : 6;
		float introductionCoef = (rand() % maxCoef) / 10.0f;
		
		std::string modelName = rockNames[index];
		GameEntity* rock = mEngine->CreateGameEntityFromModel(modelName, transformation, introductionCoef);
		if (rock != nullptr)
		{
			PhysicsComponent* component = rock->GetComponent<PhysicsComponent>();
			component->SetVelocity(MARS_GRAVITY_VALUE);
			mPropsEntityList.push_back(rock);
			mScene->AddEntity(rock);
		}
	}
}

void MarsPlanet::CreateCamp()
{
	BuildingBuilder builder(mScene);

	std::vector<std::vector<unsigned int>> matrix;
	std::vector<unsigned int> row;
	row.push_back(57);
	row.push_back(49);
	row.push_back(51);
	row.push_back(1);
	row.push_back(1);
	row.push_back(1);
	row.push_back(1);
	row.push_back(1);
	row.push_back(57);
	row.push_back(51);
	row.push_back(1);
	row.push_back(1);
	row.push_back(1);
	row.push_back(1);
	row.push_back(1);
	row.push_back(1);
	row.push_back(57);
	row.push_back(49);
	row.push_back(49);
	row.push_back(51);
	row.push_back(1);
	row.push_back(1);
	row.push_back(1);
	row.push_back(1);
	row.push_back(3);

	matrix.push_back(row);

	row.clear();
	row.push_back(60);
	row.push_back(48);
	row.push_back(54);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(56);
	row.push_back(48);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(60);
	row.push_back(48);
	row.push_back(52);
	row.push_back(54);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(1);

	matrix.push_back(row);

	row.clear();
	row.push_back(8);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(60);
	row.push_back(54);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(1);

	matrix.push_back(row);

	row.clear();
	row.push_back(8);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(8);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(57);
	row.push_back(49);
	row.push_back(49);
	row.push_back(51);

	matrix.push_back(row);

	row.clear();
	row.push_back(8);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(60);
	row.push_back(48);
	row.push_back(52);
	row.push_back(54);

	matrix.push_back(row);

	row.clear();
	row.push_back(8);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(9);
	row.push_back(1);
	row.push_back(1);
	row.push_back(1);
	row.push_back(1);
	row.push_back(3);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(56);
	row.push_back(50);
	row.push_back(0);

	matrix.push_back(row);

	row.clear();
	row.push_back(8);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(8);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(2);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(48);
	row.push_back(50);
	row.push_back(0);

	matrix.push_back(row);

	row.clear();
	row.push_back(8);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(8);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(56);
	row.push_back(50);
	row.push_back(0);

	matrix.push_back(row);

	row.clear();
	row.push_back(8);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(8);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(2);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(57);
	row.push_back(48);
	row.push_back(49);
	row.push_back(51);

	matrix.push_back(row);

	row.clear();
	row.push_back(8);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(12);
	row.push_back(4);
	row.push_back(4);
	row.push_back(4);
	row.push_back(4);
	row.push_back(6);
	row.push_back(4);
	row.push_back(4);
	row.push_back(0);
	row.push_back(0);
	row.push_back(4);
	row.push_back(4);
	row.push_back(4);
	row.push_back(60);
	row.push_back(52);
	row.push_back(52);
	row.push_back(54);

	matrix.push_back(row);

	row.clear();
	row.push_back(57);
	row.push_back(49);
	row.push_back(48);
	row.push_back(49);
	row.push_back(51);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(8);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(2);

	matrix.push_back(row);

	row.clear();
	row.push_back(60);
	row.push_back(48);
	row.push_back(13);
	row.push_back(7);
	row.push_back(50);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(16);
	row.push_back(16);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(57);
	row.push_back(49);
	row.push_back(49);
	row.push_back(49);
	row.push_back(14);

	matrix.push_back(row);

	row.clear();
	row.push_back(57);
	row.push_back(48);
	row.push_back(48);
	row.push_back(48);
	row.push_back(50);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(16);
	row.push_back(16);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(0);
	row.push_back(48);
	row.push_back(48);
	row.push_back(48);
	row.push_back(48);
	row.push_back(50);

	matrix.push_back(row);

	row.clear();
	row.push_back(60);
	row.push_back(52);
	row.push_back(54);
	row.push_back(52);
	row.push_back(54);
	row.push_back(4);
	row.push_back(4);
	row.push_back(4);
	row.push_back(12);
	row.push_back(4);
	row.push_back(4);
	row.push_back(4);
	row.push_back(4);
	row.push_back(4);
	row.push_back(4);
	row.push_back(4);
	row.push_back(4);
	row.push_back(4);
	row.push_back(4);
	row.push_back(4);
	row.push_back(60);
	row.push_back(52);
	row.push_back(52);
	row.push_back(52);
	row.push_back(54);

	matrix.push_back(row);

	builder.SetWallsMatrix(matrix);
	builder.SetCentre(glm::vec3(12.5f, 0.0f, 0.0f));
	builder.Build();
}

void MarsPlanet::CreateCampTest()
{
	BuildingBuilder builder(mScene);

	std::vector<std::vector<unsigned int>> matrix;
	std::vector<unsigned int> row;
	row.push_back(1);
	row.push_back(0);
	row.push_back(2);
	row.push_back(0);
	row.push_back(4);
	row.push_back(0);
	row.push_back(8);

	matrix.push_back(row);

	row.clear();
	row.push_back(0);
	matrix.push_back(row);

	row.clear();
	row.push_back(3);
	row.push_back(0);
	row.push_back(6);
	row.push_back(0);
	row.push_back(12);
	row.push_back(0);
	row.push_back(9);

	matrix.push_back(row);
	row.clear();
	row.push_back(0);
	matrix.push_back(row);

	row.clear();
	row.push_back(11);
	row.push_back(0);
	row.push_back(7);
	row.push_back(0);
	row.push_back(14);
	row.push_back(0);
	row.push_back(13);

	matrix.push_back(row);

	builder.SetWallsMatrix(matrix);
	builder.Build();
}