#include "stdafx.h"
#include "ItemHUD.h"
#include "../../resources/Transformation.h"
#include "../../resources/GameEntity.h"
#include "../../resources/renderers/IRenderer.h"
#include "../../resources/materials/IMaterial.h"


ItemHUD::ItemHUD(IRenderer* renderer, const glm::vec2& screenCoord, int size)
{
	Create(renderer, screenCoord, size);
}


ItemHUD::~ItemHUD()
{
}

void ItemHUD::Create(IRenderer* renderer, const glm::vec2& screenCoord, int size)
{
	mBackgroundEntity = new GameEntity(new Transformation(glm::vec3(screenCoord.x, screenCoord.y, 0.0f),
		glm::vec3(0.0f),
		glm::vec3(size)),
		renderer
	);
}

GameEntity* ItemHUD::GetGameEntity()
{
	return mBackgroundEntity;
}

void ItemHUD::SetItemMaterial(IMaterial* material)
{
	if (mBackgroundEntity != nullptr)
	{
		IRenderer* renderer = mBackgroundEntity->GetRenderer();
		assert(renderer != nullptr);

		renderer->SetMaterial(material);
	}
}

void ItemHUD::SetVisibility(bool visible)
{
	mBackgroundEntity->GetRenderer()->SetVisibility(visible);
}

void ItemHUD::SetCounter(unsigned int counter)
{
	mCounter = counter;
}

IRenderer* ItemHUD::GetRenderer() const
{
	return mBackgroundEntity->GetRenderer();
}

void ItemHUD::SetItemId(unsigned int id)
{
	mItemId = id;
}

unsigned int ItemHUD::GetItemId() const
{
	return mItemId;
}