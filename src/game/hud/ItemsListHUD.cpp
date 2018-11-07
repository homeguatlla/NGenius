#include "stdafx.h"
#include "ItemsListHUD.h"
#include "../../resources/GameEntity.h"
#include "../../resources/materials/IMaterial.h"
#include "../../resources/materials/MaterialsLibrary.h"
#include "../../resources/materials/effects/MaterialEffectDiffuseTexture.h"

#include "../../resources/renderers/IRenderer.h"
#include "../../resources/renderers/IndicesRenderer.h"
#include "../../resources/scene/GameScene.h"

#include "../ShooterGameConstants.h"

#include <string>

const std::string ITEM_QUAD_TEXTURE("item_quad_base");
const std::string ITEM_QUAD_SELECTED_TEXTURE("item_quad_selected");

const std::string MATERIAL_ITEM("item_quad_base_material");
const std::string MATERIAL_ITEM_SELECTED("item_quad_selected_material");

const int ITEM_SIZE = 64;
const int NUM_ITEMS = 5;
const float OFFSET_BETWEEN_ITEMS = 0.07f;

ItemsListHUD::ItemsListHUD(NGenius& engine, GameScene* scene, const glm::vec2& screenCoord) :
	mSelectedItemEntity(nullptr),
	mScreenCoord(screenCoord),
	mSelectedItem(0)
{
	Create(engine, scene);
}


ItemsListHUD::~ItemsListHUD()
{
}

void ItemsListHUD::Show()
{
}

void ItemsListHUD::Create(NGenius& engine, GameScene* scene)
{
	CreateItems(engine, scene);
	CreateSelectedItem(engine, scene);
}

void ItemsListHUD::CreateItems(NGenius& engine, GameScene* scene)
{
	for (int i = 0; i < NUM_ITEMS; ++i)
	{
		glm::vec2 screenCoords(mScreenCoord.x + OFFSET_BETWEEN_ITEMS * i, mScreenCoord.y);
		CreateItem(engine, scene, screenCoords, MATERIAL_ITEM, ITEM_QUAD_TEXTURE);
	}
}

void ItemsListHUD::CreateSelectedItem(NGenius& engine, GameScene* scene)
{
	mSelectedItemEntity = CreateItemBase(engine, scene, mScreenCoord, MATERIAL_ITEM_SELECTED, ITEM_QUAD_SELECTED_TEXTURE);
	scene->AddEntity(mSelectedItemEntity);
}

GameEntity* ItemsListHUD::CreateItemBase(NGenius& engine, GameScene* scene, const glm::vec2& screenCoord, const std::string& materialName, const std::string& textureName)
{
	IMaterial* material = engine.GetMaterial(materialName);
	if (material == nullptr)
	{
		material = engine.CreateMaterial(materialName, engine.GetShader("gui"));
	}
	material->AddEffect(new MaterialEffectDiffuseTexture(engine.GetTexture(textureName), glm::vec3(1.0f), 1.0f));
	IRenderer* guiRenderer = new IndicesRenderer(engine.GetModel(GUI_QUAD_MODEL), material);
	guiRenderer->SetLayer(IRenderer::LAYER_GUI);

	float x = engine.GetScreenWidth() * screenCoord.x;
	float y = -engine.GetScreenHeight() * screenCoord.y;
	GameEntity* itemGameEntity = new GameEntity(new Transformation(glm::vec3(x, y, 0.0f),
		glm::vec3(0.0f),
		glm::vec3(ITEM_SIZE)),
		guiRenderer
	);
	return itemGameEntity;
}

void ItemsListHUD::CreateItem(NGenius& engine, GameScene* scene, const glm::vec2& screenCoord, const std::string& materialName, const std::string& textureName)
{
	GameEntity* itemGameEntity = CreateItemBase(engine, scene, screenCoord, materialName, textureName);
	mItemsListEntity.push_back(itemGameEntity);
	scene->AddEntity(itemGameEntity);
}