#pragma once
#include "../../NGenius.h"

#include <glm/glm.hpp>
#include <string>

class GameEntity;
class IRenderer;
class IMaterial;

class ItemHUD
{
	GameEntity* mBackgroundEntity;
	//TODO por hacer
	GameEntity* mCounterEntity;
	unsigned int mItemId;
	unsigned int mCounter;

public:
	ItemHUD(IRenderer* renderer, const glm::vec2& screenCoord, int size);
	~ItemHUD();

	void Create(IRenderer* renderer, const glm::vec2& screenCoord, int size);
	void SetItemMaterial(IMaterial* material);
	void SetVisibility(bool visible);
	void SetCounter(unsigned int counter);
	void SetItemId(unsigned int id);
	IRenderer* GetRenderer() const;

	unsigned int GetItemId() const;

	GameEntity* GetGameEntity();
};

