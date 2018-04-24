#include "stdafx.h"
#include "GameScene.h"
#include "../systems/EntitiesSystem.h"
#include "../systems/renderSystem/RenderSystem.h"
#include "quadtree/GameEntityQuadTree.h"
#include "../GameEntity.h"

GameScene::GameScene(const std::string& name, EntitiesSystem* entitiesSystem, RenderSystem* renderSystem) : 
	mEntitiesSystem(entitiesSystem),
	mRenderSystem(renderSystem),
	mSceneGraph(nullptr),
	mName(name),
	mAABB(glm::vec3(std::numeric_limits<float>::max()), glm::vec3(std::numeric_limits<float>::min()))
{
}


GameScene::~GameScene()
{
	delete mSceneGraph;
}

void GameScene::Init()
{
	
	mSceneGraph = new GameEntityQuadTree(mAABB);
}

void GameScene::Update()
{
	//actualizar el quadtree
	//añadir a renderizar
}

void GameScene::AddGameEntity(GameEntity* entity)
{
	mEntitiesSystem->AddEntity(entity);
	//mAABB.Concat(entity->GetRenderer()->GetAABB());
	//mQuadTree->AddEntity(entity);
}

void GameScene::RemoveGameEntity(GameEntity* entity)
{
	mEntitiesSystem->RemoveEntity(entity);
	//mQuadTree->RemoveEntity();
}
