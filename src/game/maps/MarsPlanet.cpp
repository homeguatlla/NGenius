#include "stdafx.h"
#include "MarsPlanet.h"
#include "../ShooterGameConstants.h"

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

#include "../../resources/textures/Texture.h"
#include "../../resources/textures/TextureArray.h"

#include "../../resources/renderers/IRenderer.h"
#include "../../resources/systems/renderSystem/RenderPass.h"


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
}


void MarsPlanet::CreateTerrain()
{
	//RENDER PASS GAMEPLAY
	RenderPass *terrainPass = new RenderPass(static_cast<ICamera*>(mGameplayCamera), IRenderer::LAYER_TERRAIN);
	terrainPass->EnableFog(true);
	mEngine.AddRenderPass(terrainPass, false);

	float scale = TERRAIN_SCALE;

	IMaterial* material = mEngine.CreateMaterial("terrain", mEngine.GetShader("terrain"));
	material->AddEffect(new MaterialEffectDiffuseTexture(static_cast<Texture*>(mEngine.GetTexture("terrain_blendmap")), glm::vec3(1.0f, 1.0f, 1.0f), 50.0f));
	material->AddEffect(new MaterialEffectDirectionalLightProperties());
	material->AddEffect(new MaterialEffectFogProperties());
	material->AddEffect(new MaterialEffectHeightMapTexture(static_cast<Texture*>(mEngine.GetTexture("terrain_heightmap")), 1.0f));
	material->AddEffect(new MaterialEffectTextureArray(static_cast<TextureArray*>(mEngine.GetTexture("terrain_array"))));
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