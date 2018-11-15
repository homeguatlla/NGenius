#include "stdafx.h"
#include "GameHUD.h"
#include "ItemsListHUD.h"
#include "ItemHUD.h"

#include "../../resources/GameEntity.h"
#include "../../resources/materials/IMaterial.h"
#include "../../resources/materials/MaterialsLibrary.h"
#include "../../resources/materials/effects/MaterialEffectDiffuseTexture.h"

#include "../../resources/renderers/IRenderer.h"
#include "../../resources/renderers/IndicesRenderer.h"
#include "../../resources/scene/GameScene.h"

#include "../ShooterGameConstants.h"


GameHUD::GameHUD(NGenius& engine, GameScene* scene) : mEngine(engine)
{
	Create(scene);
}

GameHUD::~GameHUD()
{
	
}

void GameHUD::SetVisibility(bool visible)
{
	mItemsList->SetVisibility(visible);
}

void GameHUD::Create(GameScene* scene)
{
	//para resolucion 1024 x 768
	float factorX = 0.15f;
	float factorY = 0.43f;

	factorX = mEngine.GetScreenWidth() / 1024.0f * factorX;

	mItemsList = new  ItemsListHUD(
		mEngine, 
		scene, 
		glm::vec2(mEngine.GetScreenWidth() * factorX, -mEngine.GetScreenHeight() * factorY),
		NUM_ITEMS_INVENTORY
	);
}

void GameHUD::Update(float elapsedTime)
{
	mItemsList->Update(elapsedTime);
}

ItemsListHUD* GameHUD::GetItemsListHUD()
{
	return mItemsList;
}

