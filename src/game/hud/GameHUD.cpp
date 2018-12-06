#include "stdafx.h"
#include "GameHUD.h"
#include "ItemsListHUD.h"
#include "ItemHUD.h"
#include "HealthHUD.h"

#include "../../NGenius.h"
#include "../../resources/GameEntity.h"
#include "../../resources/materials/IMaterial.h"
#include "../../resources/materials/MaterialsLibrary.h"
#include "../../resources/materials/effects/MaterialEffectDiffuseTexture.h"

#include "../../resources/renderers/IRenderer.h"
#include "../../resources/renderers/IndicesRenderer.h"
#include "../../resources/scene/GameScene.h"

#include "../ShooterGameConstants.h"


GameHUD::GameHUD(GameScene* scene) :
	mItemsList(nullptr),
	mHealthHUD(nullptr)
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
	mItemsList = new  ItemsListHUD(
		scene, 
		glm::vec2(665.0f, 700.0f),
		NUM_ITEMS_INVENTORY
	);
	
	mHealthHUD = new HealthHUD(scene, glm::vec2(100.0f, 700.0f));
}

void GameHUD::Update(float elapsedTime)
{
}

ItemsListHUD* GameHUD::GetItemsListHUD()
{
	return mItemsList;
}

