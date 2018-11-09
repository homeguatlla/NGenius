#pragma once
#include "../../NGenius.h"

#include <glm/glm.hpp>
#include <string>

class GameEntity;
class IRenderer;
class IMaterial;

class ItemHUD
{
	GameEntity* mItemEntity;
public:
	ItemHUD(IRenderer* renderer, const glm::vec2& screenCoord, int size);
	~ItemHUD();

	void Create(IRenderer* renderer, const glm::vec2& screenCoord, int size);
	void SetItemMaterial(IMaterial* material);
	void SetVisibility(bool visible);

	GameEntity* GetGameEntity();
};

