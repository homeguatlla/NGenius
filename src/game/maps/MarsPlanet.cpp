#include "stdafx.h"
#include "MarsPlanet.h"
#include "../ShooterGameConstants.h"
#include "../EngineConstants.h"

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


MarsPlanet::MarsPlanet(NGenius& engine, GameScene* scene, ICamera* gameplayCamera) :
	mEngine(engine),
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
	CreateRocks();
}

void MarsPlanet::CreateTerrain()
{
	//RENDER PASS TERRAIN
	RenderPass *terrainPass = new RenderPass(static_cast<ICamera*>(mGameplayCamera), IRenderer::LAYER_TERRAIN);
	terrainPass->EnableFog(true);
	mEngine.AddRenderPassAt(1, terrainPass, false);

	bool isFlat = false;

	float scale = isFlat ? 0.0f : TERRAIN_SCALE;

	IMaterial* material = mEngine.CreateMaterial("terrain", mEngine.GetShader("terrain"));
	material->AddEffect(new MaterialEffectDiffuseTexture(static_cast<Texture*>(mEngine.GetTexture("terrain_blendmap")), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f));
	material->AddEffect(new MaterialEffectDirectionalLightProperties());
	material->AddEffect(new MaterialEffectFogProperties());
	material->AddEffect(new MaterialEffectHeightMapTexture(static_cast<Texture*>(mEngine.GetTexture("terrain_heightmap")), 1.0f));
	material->AddEffect(new MaterialEffectTextureArray(static_cast<TextureArray*>(mEngine.GetTexture("terrain_mars_array"))));
	material->AddEffect(new MaterialEffectClippingPlane());
	material->AddEffect(new MaterialEffectShadowProperties(3));
	material->AddEffect(new MaterialEffectFloat(scale));

	mTerrain = new Terrain(new Transformation(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)),
		material,
		static_cast<Texture*>(mEngine.GetTexture("terrain_heightmap")),
		scale);

	mTerrain->SetFlat(isFlat);

	mScene->AddEntity(mTerrain);
	mEngine.SetTerrain(mTerrain);
}

void MarsPlanet::CreateSky()
{
	IMaterial* material = mEngine.CreateMaterial("skybox", mEngine.GetShader("skybox"));
	TextureCubemap* cubemap1 = static_cast<TextureCubemap*>(mEngine.GetTexture("day_cubemap"));
	TextureCubemap* cubemap2 = static_cast<TextureCubemap*>(mEngine.GetTexture("night_cubemap"));
	material->AddEffect(new MaterialEffectTextureCubemap(cubemap1, cubemap2, 0.0f));
	material->AddEffect(new MaterialEffectFogProperties());
	material->AddEffect(new MaterialEffectDirectionalLightProperties());

	SkyBoxRenderer* skyboxRenderer = new SkyBoxRenderer(mEngine.GetModel("skybox"), material);
	skyboxRenderer->SetLayer(IRenderer::LAYER_PARTICLES);

	mSkyBox = new GameEntity(
								new Transformation(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(100.0f)),
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
		float scale = 0.01f + (rand() % 5) /100.0f;
		float rotation = rand() % 360;
		Transformation* transformation = new Transformation(glm::vec3(x, 10.0f, z), glm::vec3(0.0f, glm::radians(rotation), 0.0f), glm::vec3(scale));

		unsigned int index = rand() % rockNames.size();
		int maxCoef = index == 1 ? 2 : 6;
		float introductionCoef = (rand() % maxCoef) / 10.0f;
		
		std::string modelName = rockNames[index];
		GameEntity* rock = CreateGameEntityFromModel(modelName, transformation, introductionCoef);
		if (rock != nullptr)
		{
			mPropsEntityList.push_back(rock);
			mScene->AddEntity(rock);
		}
	}
}

GameEntity* MarsPlanet::CreateGameEntityFromModel(const std::string& modelName, Transformation* transformation, float introductionCoef)
{
	Model* model = mEngine.GetModel(modelName);
	if (model != nullptr)
	{
		IMaterial* material = mEngine.GetMaterial(model->GetMaterialName());
		if (material == nullptr)
		{
			std::string shaderName = model->HasNormalmap() ? "normalmap" : "model";

			material = mEngine.CreateMaterial(model->GetMaterialName(), mEngine.GetShader(shaderName));
		}
		material->AddEffect(new MaterialEffectDiffuseTexture(
			static_cast<Texture*>(mEngine.GetTexture(model->GetMaterialName() + "_diffuse")),
			glm::vec3(1.0f, 1.0f, 1.0f), 
			1
		));
		material->AddEffect(new MaterialEffectDirectionalLightProperties());
		material->AddEffect(new MaterialEffectFogProperties());
		if (model->HasNormalmap())
		{
			material->AddEffect(new MaterialEffectNormalTexture(
				static_cast<Texture*>(mEngine.GetTexture(model->GetMaterialName() + "_normalmap")),
				1
			));
		}
		material->AddEffect(new MaterialEffectShadowProperties(1));
		material->AddEffect(new MaterialEffectClippingPlane());


		IRenderer* renderer = new IndicesRenderer(model, material);

		GameEntity* modelEntity = new GameEntity(transformation, renderer);

		modelEntity->AddComponent(new PhysicsComponent(true, PhysicsSystem::GRAVITY_VALUE, introductionCoef));
		modelEntity->AddComponent(new CollisionComponent());
		modelEntity->AddComponent(new SpacePartitionComponent());

		IRenderer* boundingBoxRenderer = new WireframeRenderer(mEngine.GetModel("cube"), mEngine.GetMaterial(MaterialsLibrary::WIREFRAME_MATERIAL_NAME));
		modelEntity->AddComponent(new DebugComponent(boundingBoxRenderer));

		return modelEntity;
	}

	return nullptr;
}