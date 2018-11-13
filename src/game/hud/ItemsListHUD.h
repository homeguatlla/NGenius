#pragma once
#include "../../NGenius.h"
#include "../items/Item.h"

class GameScene;
class ItemHUD;

class ItemsListHUD
{
	NGenius* mEngine;
	std::vector<ItemHUD*> mItemsList;
	GameEntity* mSelectedItemEntity;
	glm::vec2 mScreenCentre;
	glm::vec2 mScreenCoord;
	unsigned int mSelectedItem;

public:
	ItemsListHUD(NGenius& engine, GameScene* scene, const glm::vec2& screenCoord);
	~ItemsListHUD();

	void SetVisibility(bool visible);
	void Update(float elapsedTime);
	void AddItem(Item* item);
	void RemoveSelectedItem();
	unsigned int GetSelectedItemId() const;

private:
	void Create(GameScene* scene);
	void CreateItem(GameScene* scene, const glm::vec2& screenCoord, const std::string& materialName, const std::string& textureName);
	void CreateItems(GameScene* scene);
	void CreateSelectedItem(GameScene* scene);

	void UpdateGameEvents();
	void UpdateSelectedItemPosition();

	IMaterial* CreateMaterial(const std::string& materialName, const std::string& textureName);
	glm::vec2 CalculateItemPosition(unsigned int item);
};

