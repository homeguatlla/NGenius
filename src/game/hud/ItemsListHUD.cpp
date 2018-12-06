#include "stdafx.h"
#include "ItemsListHUD.h"

#include <GLFW/glfw3.h>

#include "../../NGenius.h"
#include "../../resources/GameEntity.h"
#include "../../resources/materials/IMaterial.h"
#include "../../resources/materials/MaterialsLibrary.h"
#include "../../resources/materials/effects/MaterialEffectDiffuseTexture.h"

#include "../../resources/renderers/IRenderer.h"
#include "../../resources/renderers/GUIRenderer.h"
#include "../../resources/scene/GameScene.h"

#include "../events/NextPreviousInventoryItemEvent.h"

#include "../ShooterGameConstants.h"

#include "ItemHUD.h"

#include "../../utils/Log.h"

const std::string ITEM_QUAD_TEXTURE("item_quad_base");
const std::string ITEM_QUAD_SELECTED_TEXTURE("item_quad_selected");

const std::string ITEM_MATERIAL("item_quad_base_material");
const std::string ITEM_SELECTED_MATERIAL("item_quad_selected_material");
const int ITEM_SIZE_SELECTED = 75;
const float OFFSET_BETWEEN_ITEMS = 8.0f;

const float LAYER_SELECTED_ITEM_Z = 1.0f;


ItemsListHUD::ItemsListHUD(GameScene* scene, const glm::vec2& screenCoord, unsigned int numItems) :
	mSelectedItemEntity(nullptr),
	mScreenCoord(screenCoord),
	mSelectedItem(0),
	mNumItems(numItems)
{
	Create(scene);
}


ItemsListHUD::~ItemsListHUD()
{
}

void ItemsListHUD::SetVisibility(bool visible)
{
	mSelectedItemEntity->GetRenderer()->SetVisibility(visible);
	for (unsigned int i = 0; i < mItemsList.size(); ++i)
	{
		mItemsList[i]->SetVisibility(visible);
	}
}

bool ItemsListHUD::IsItemHUDEmpty(ItemHUD* itemHUD) const
{
	assert(itemHUD != nullptr);

	IRenderer* renderer = itemHUD->GetRenderer();
	IMaterial* emptyMaterial = NGenius::GetInstance().GetMaterial(ITEM_MATERIAL);
	assert(renderer != nullptr);
	bool isItemHUDEmpty = renderer->GetMaterial()->GetMaterialID() == emptyMaterial->GetMaterialID();

	return isItemHUDEmpty;
}

void ItemsListHUD::AddItem(InventoryItem* item)
{
	for (ItemHUD* itemHUD : mItemsList)
	{
		bool isItemHUDEmpty = IsItemHUDEmpty(itemHUD);
		if (isItemHUDEmpty)
		{
			IMaterial* material = NGenius::GetInstance().CreateDiffuseGUIMaterial(item->GetName() + std::string("_material"), item->GetName());
			itemHUD->SetItemMaterial(material);
			itemHUD->SetCounter(item->GetCounter());
			itemHUD->SetItemId(item->GetId());
			itemHUD->GetGameEntity()->GetRenderer()->SetVisibility(true);
			break;
		}
	}
}
void ItemsListHUD::RemoveSelectedItem()
{
	ItemHUD* item = mItemsList[mSelectedItem];
	if (item != nullptr)
	{
		IMaterial* emptyMaterial = NGenius::GetInstance().GetMaterial(ITEM_MATERIAL);
		item->SetItemMaterial(emptyMaterial);
		item->SetCounter(0);
		item->SetItemId(0);
	}
}

unsigned int ItemsListHUD::GetSelectedItemId() const
{
	return mItemsList[mSelectedItem]->GetItemId();
}

void ItemsListHUD::Create(GameScene* scene)
{
	CreateItems(scene);
	CreateSelectedItem(scene);
	UpdateSelectedItemPosition(mSelectedItem, mSelectedItem);
}

void ItemsListHUD::OnNextPreviousItem(NextPreviousInventoryItemEvent* nextPreviousEvent)
{
	if (!mSelectedItemEntity->GetRenderer()->IsVisible())
	{
		return;
	}
	int increment = nextPreviousEvent->IsNext() ? -1 : 1;
	int newSelectedItem = (static_cast<int>(mSelectedItem) + increment) % static_cast<int>(mNumItems);
	if (newSelectedItem < 0)
	{
		newSelectedItem = mNumItems + newSelectedItem;
	}
	UpdateSelectedItemPosition(mSelectedItem, newSelectedItem);
}

void ItemsListHUD::SetSize(GameEntity* entity, int size)
{
	Transformation* transformation = entity->GetTransformation();
	glm::vec3 scale = transformation->GetScale();
	scale = glm::vec3(static_cast<float>(size));
	transformation->SetScale(scale);
}

void ItemsListHUD::UpdateSelectedItemPosition(unsigned int selectedItem, unsigned int newSelectedItem)
{
	Transformation* transformation = mSelectedItemEntity->GetTransformation();
	glm::vec3 position = transformation->GetPosition();
	position.x = CalculateItemPosition(newSelectedItem).x;
	transformation->SetPosition(position);

	//recover size item before
	ItemHUD* selectedItemHUD = mItemsList[selectedItem];
	GameEntity* selectedEntity = selectedItemHUD->GetGameEntity();
	SetSize(selectedEntity, ITEM_SIZE);
	//selectedEntity->GetRenderer()->SetVisibility(true);

	//resize new item
	ItemHUD* newSelectedItemHUD = mItemsList[newSelectedItem];
	GameEntity* newSelectedEntity = newSelectedItemHUD->GetGameEntity();
	SetSize(newSelectedEntity, ITEM_SIZE_SELECTED);

	selectedEntity->GetRenderer()->SetVisibility(true);
	newSelectedEntity->GetRenderer()->SetVisibility(!IsItemHUDEmpty(newSelectedItemHUD));
	
	mSelectedItem = newSelectedItem;
}

glm::vec2 ItemsListHUD::CalculateItemPosition(unsigned int item)
{
	return mScreenCoord + glm::vec2((ITEM_SIZE + OFFSET_BETWEEN_ITEMS) * item, 0.0f);
}

void ItemsListHUD::CreateItems(GameScene* scene)
{
	for (unsigned int i = 0; i < mNumItems; ++i)
	{
		glm::vec2 screenCoords = CalculateItemPosition(i);
		CreateItem(scene, screenCoords, ITEM_MATERIAL, ITEM_QUAD_TEXTURE);
	}
}

void ItemsListHUD::CreateSelectedItem(GameScene* scene)
{
	glm::vec2 screenCoords = CalculateItemPosition(mSelectedItem);
	IMaterial* material = NGenius::GetInstance().CreateDiffuseGUIMaterial(ITEM_SELECTED_MATERIAL, ITEM_QUAD_SELECTED_TEXTURE);
	IRenderer* guiRenderer = new GUIRenderer(NGenius::GetInstance().GetModel(GUI_QUAD_MODEL), material);
	guiRenderer->SetLayer(IRenderer::LAYER_GUI);

	mSelectedItemEntity = new GameEntity(
		new Transformation(glm::vec3(screenCoords.x, screenCoords.y, LAYER_SELECTED_ITEM_Z),
		glm::vec3(0.0f),
		glm::vec3(ITEM_SIZE_SELECTED)),
		guiRenderer);

	scene->AddEntity(mSelectedItemEntity);
}

void ItemsListHUD::CreateItem(GameScene* scene, const glm::vec2& screenCoord, const std::string& materialName, const std::string& textureName)
{
	IMaterial* material = NGenius::GetInstance().CreateDiffuseGUIMaterial(materialName, textureName);
	IRenderer* guiRenderer = new GUIRenderer(NGenius::GetInstance().GetModel(GUI_QUAD_MODEL), material);
	guiRenderer->SetLayer(IRenderer::LAYER_GUI);

	ItemHUD* item = new ItemHUD(guiRenderer, screenCoord, ITEM_SIZE);
	mItemsList.push_back(item);
	scene->AddEntity(item->GetGameEntity());
}