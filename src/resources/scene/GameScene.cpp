#include "stdafx.h"
#include "GameScene.h"
#include "../systems/EntitiesSystem.h"
#include "../systems/renderSystem/RenderSystem.h"
#include "../GameEntity.h"

GameScene::GameScene(const std::string& name, EntitiesSystem* entitiesSystem, RenderSystem* renderSystem) : 
	mName(name),
	mEntitiesSystem(entitiesSystem),
	mRenderSystem(renderSystem)
{
}


GameScene::~GameScene()
{
}

void GameScene::Update()
{
	//actualizar el quadtree
	//añadir a renderizar
}

void GameScene::AddGameEntity(GameEntity* entity)
{
	mEntitiesSystem->AddEntity(entity);
	//mQuadTree->AddEntity(entity);
}

void GameScene::RemoveGameEntity(GameEntity* entity)
{
	mEntitiesSystem->RemoveEntity(entity);
	//mQuadTree->RemoveEntity();
}
