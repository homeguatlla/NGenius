#include "stdafx.h"
#include "ItemHUD.h"
#include "../../resources/Transformation.h"
#include "../../resources/GameEntity.h"
#include "../../resources/renderers/IRenderer.h"

ItemHUD::ItemHUD(IRenderer* renderer, const glm::vec2& screenCoord, int size)
{
	Create(renderer, screenCoord, size);
}


ItemHUD::~ItemHUD()
{
}

void ItemHUD::Create(IRenderer* renderer, const glm::vec2& screenCoord, int size)
{
	mItemEntity = new GameEntity(new Transformation(glm::vec3(screenCoord.x, screenCoord.y, 0.0f),
		glm::vec3(0.0f),
		glm::vec3(size)),
		renderer
	);
}

GameEntity* ItemHUD::GetGameEntity()
{
	return mItemEntity;
}

void ItemHUD::SetItemMaterial(IMaterial* material)
{
	if (mItemEntity != nullptr)
	{
		IRenderer* renderer = mItemEntity->GetRenderer();
		assert(renderer != nullptr);

		renderer->SetMaterial(material);
	}
}

void ItemHUD::SetVisibility(bool visible)
{
	mItemEntity->GetRenderer()->SetVisibility(visible);
}