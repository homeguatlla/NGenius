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

IGameEntity* GameEntity::DoCreate()
{
	return DBG_NEW GameEntity();
}

void GameEntity::DoInit(GameScene* scene, RenderSystem* renderSystem)
{
}
