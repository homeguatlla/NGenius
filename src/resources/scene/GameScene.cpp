#include "stdafx.h"
#include "GameScene.h"
#include "IGameSceneListener.h"
#include "../GameEntity.h"
#include "../systems/renderSystem/RenderSystem.h"
#include "../renderers/IRenderer.h"
#include "../components/LODComponent.h"
#include "../components/SpacePartitionComponent.h"

#include <algorithm>

GameScene::GameScene(const std::string& name) :
mName(name),
mAABB(glm::vec3(std::numeric_limits<float>::max()), -glm::vec3(std::numeric_limits<float>::max()))
{
}

GameScene::~GameScene()
{
	mEntitiesToRemove.clear(); //these entities were removed when releasing mEntities.
	mEntitiesOutsideSpacePartition.clear();
	ReleaseEntities(&mEntities);
	ReleaseEntities(&mNewEntitiesToAdd);	
}

void GameScene::Update(float elapsedTime)
{
	RemoveEntities();
	AddNewEntities();

	for (GameEntity* entity : mEntities)
	{
		entity->Update(elapsedTime);
	}
}

void GameScene::Render(RenderSystem* renderSystem)
{
	for (GameEntity* entity : mEntities)
	{
		renderSystem->AddToRender(entity->GetRenderer());
	}
}

void GameScene::RenderOutsideSpacePartition(RenderSystem* renderSystem)
{
	for (GameEntity* entity : mEntitiesOutsideSpacePartition)
	{
		renderSystem->AddToRender(entity->GetRenderer());
	}
}

unsigned int GameScene::GetNumberGameEntities() const
{
	return mEntities.size();
}

unsigned int GameScene::GetNumberGameEntitiesOutsideSpacePartition() const
{
	return mEntitiesOutsideSpacePartition.size();
}

std::vector<GameEntity*>& GameScene::GetAllGameEntities()
{
	return mEntities;
}

void GameScene::AddEntity(GameEntity* entity)
{
	entity->Init();

	if (entity->GetRenderer() != nullptr)
	{
		mAABB = mAABB.Merge(entity->GetRenderer()->GetAABB());
	}
	mNewEntitiesToAdd.push_back(entity);
}

void GameScene::RemoveEntity(GameEntity* entity)
{
	mEntitiesToRemove.push_back(entity);
}

const AABB& GameScene::GetAABB() const
{
	return mAABB;
}

void GameScene::RegisterGameSceneListener(IGameSceneListener* listener)
{
	bool found = std::find(mListeners.begin(), mListeners.end(), listener) != mListeners.end();
	if (!found)
	{
		mListeners.push_back(listener);
	}
}

void GameScene::UnRegisterGameSceneListener(IGameSceneListener* listener)
{
	ListenersIterator it = std::find(mListeners.begin(), mListeners.end(), listener);
	bool found = it != mListeners.end();
	if (found)
	{
		mListeners.erase(it);
	}
}

void GameScene::AddNewEntities()
{
	for (GameEntity* entity : mNewEntitiesToAdd)
	{
		if (!entity->HasComponent<SpacePartitionComponent>())
		{
			mEntitiesOutsideSpacePartition.push_back(entity);
		}
		mEntities.push_back(entity);
		NotifyEntityAdded(entity);
	}
	mNewEntitiesToAdd.clear();
}

void GameScene::NotifyEntityAdded(GameEntity* entity)
{
	for (ListenersIterator it = mListeners.begin(); it != mListeners.end(); ++it)
	{
		(*it)->OnGameEntityAdded(entity);
	}
}

void GameScene::NotifyEntityRemoved(GameEntity* entity)
{
	for (ListenersIterator it = mListeners.begin(); it != mListeners.end(); ++it)
	{
		(*it)->OnGameEntityRemoved(entity);
	}
}

void GameScene::RemoveEntities()
{
	for (GameEntity* entity : mEntitiesToRemove)
	{
		if (!entity->HasComponent<SpacePartitionComponent>())
		{
			GameEntitiesIterator itOutside = std::find_if(mEntitiesOutsideSpacePartition.begin(), mEntitiesOutsideSpacePartition.end(), [&](GameEntity* a) { return a == entity; });
			mEntitiesOutsideSpacePartition.erase(itOutside);
		}
		GameEntitiesIterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](GameEntity* a) { return a == entity; });
		NotifyEntityRemoved(*it);
		
		mEntities.erase(it);
	}
	mEntitiesToRemove.clear();
}

void GameScene::ReleaseEntities(std::vector<GameEntity*>* entities)
{
	for (GameEntity* entity : *entities)
	{
		delete entity;
	}
	entities->clear();
}

BaseVisitable<>::ReturnType GameScene::Accept(BaseVisitor& guest)
{
	return AcceptImpl(*this, guest);
}