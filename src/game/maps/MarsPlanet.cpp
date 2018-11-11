#include "stdafx.h"
#include "MarsPlanet.h"
#include "../ShooterGameConstants.h"
#include "../GameConstants.h"

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
#include "../../resources/systems/renderSystem/RenderPass.h"

#include "../../resources/components/RotationComponent.h"

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
}


void MarsPlanet::CreateTerrain()
{
	//RENDER PASS TERRAIN
	RenderPass *terrainPass = new RenderPass(static_cast<ICamera*>(mGameplayCamera), IRenderer::LAYER_TERRAIN);
	terrainPass->EnableFog(true);
	mEngine.AddRenderPass(terrainPass, false);

	float scale = TERRAIN_SCALE;

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

	mTerrain->SetFlat(false);

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