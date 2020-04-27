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
	if (mRenderer != nullptr)
	{
		return std::make_shared<GameEntity>(new Transformation(*GetTransformation()), mRenderer->Clone());
	}
	else
	{
		return std::make_shared<GameEntity>(new Transformation(*GetTransformation()));
	}
}

std::shared_ptr<IGameEntity> GameEntity::DoCreate()
{
	return std::make_shared<GameEntity>();
}

void GameEntity::DoInit(GameScene* scene, RenderSystem* renderSystem)
{
}
