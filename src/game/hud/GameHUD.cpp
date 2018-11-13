#include "stdafx.h"
#include "GameHUD.h"
#include "ItemsListHUD.h"

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
	mItemsList = new  ItemsListHUD(mEngine, scene, glm::vec2(0.15f, 0.43f));
	//mGameHudGameEntity->SetEnabled(true);
	
}

void GameHUD::Update(float elapsedTime)
{
	mItemsList->Update(elapsedTime);
}

ItemsListHUD* GameHUD::GetItemsListHUD()
{
	return mItemsList;
}

