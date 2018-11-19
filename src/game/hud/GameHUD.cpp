#include "stdafx.h"
#include "GameHUD.h"
#include "ItemsListHUD.h"
#include "ItemHUD.h"

#include "../../NGenius.h"
#include "../../resources/GameEntity.h"
#include "../../resources/materials/IMaterial.h"
#include "../../resources/materials/MaterialsLibrary.h"
#include "../../resources/materials/effects/MaterialEffectDiffuseTexture.h"

#include "../../resources/renderers/IRenderer.h"
#include "../../resources/renderers/IndicesRenderer.h"
#include "../../resources/scene/GameScene.h"

#include "../ShooterGameConstants.h"


GameHUD::GameHUD(GameScene* scene)
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

	factorX = NGenius::GetInstance().GetScreenWidth() / 1024.0f * factorX;

	mItemsList = new  ItemsListHUD(
		scene, 
		glm::vec2(NGenius::GetInstance().GetScreenWidth() * factorX, -NGenius::GetInstance().GetScreenHeight() * factorY),
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

