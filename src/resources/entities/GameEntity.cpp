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

GameEntity* GameEntity::DoClone() const
{
	if (mRenderer != nullptr)
	{
		return DBG_NEW GameEntity(new Transformation(*GetTransformation()), mRenderer->Clone());
	}
	else
	{
		return DBG_NEW GameEntity(new Transformation(*GetTransformation()));
	}
}

IGameEntity* GameEntity::DoCreate()
{
	return DBG_NEW GameEntity();
}

void GameEntity::DoInit(GameScene* scene, RenderSystem* renderSystem)
{
}
