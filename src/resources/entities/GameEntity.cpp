#include "stdafx.h"
#include "GameEntity.h"


GameEntity::GameEntity(Transformation* transformation, IRenderer* renderer) :
	BaseGameEntity(transformation, renderer)
{

}

GameEntity::GameEntity(Transformation* transformation) :
	BaseGameEntity(transformation)
{

}

std::shared_ptr<IGameEntity> GameEntity::DoClone() const
{
	std::shared_ptr<GameEntity> clone;

	if (mRenderer != nullptr)
	{
		clone = std::make_shared<GameEntity>(new Transformation(*GetTransformation()), mRenderer->Clone());
	}
	else
	{
		clone = std::make_shared<GameEntity>(new Transformation(*GetTransformation()));
	}
	clone->SetEnabled(IsEnabled());
	clone->SetShouldBeCreatedOnGround(ShouldBeCreatedOnGround());
	return clone;
}

std::shared_ptr<IGameEntity> GameEntity::DoCreate()
{
	return std::make_shared<GameEntity>();
}

void GameEntity::DoInit(GameScene* scene, RenderSystem* renderSystem)
{
}
