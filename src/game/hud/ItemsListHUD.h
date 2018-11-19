#pragma once

#include "../items/Item.h"

#include <glm/glm.hpp>
#include <vector>
#include <string>

class GameScene;
class ItemHUD;
class GameEntity;
class IMaterial;

class ItemsListHUD
{
	std::vector<ItemHUD*> mItemsList;
	GameEntity* mSelectedItemEntity;
	glm::vec2 mScreenCoord;
	unsigned int mSelectedItem;
	unsigned int mNumItems;

public:
	ItemsListHUD(GameScene* scene, const glm::vec2& screenCoord, unsigned int numItems);
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
	void UpdateSelectedItemPosition(unsigned int selectedItem, unsigned int newSelectedItem);

	IMaterial* CreateMaterial(const std::string& materialName, const std::string& textureName);
	glm::vec2 CalculateItemPosition(unsigned int item);

	void SetSize(GameEntity* entity, int size);
	bool IsItemHUDEmpty(ItemHUD* itemHUD) const;
};

