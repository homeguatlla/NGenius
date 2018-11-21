#include "stdafx.h"
#include "GameScene.h"
#include "IGameSceneListener.h"
#include "../GameEntity.h"
#include "../systems/renderSystem/RenderSystem.h"
#include "../renderers/IRenderer.h"
#include "../components/LODComponent.h"
#include "../components/SpacePartitionComponent.h"
#include "../components/AttachGameEntityComponent.h"

#include <algorithm>

GameScene::GameScene(const std::string& name) :
mName(name),
mAABB(glm::vec3(std::numeric_limits<float>::max()), -glm::vec3(std::numeric_limits<float>::max()))
{
}

GameScene::~GameScene()
{
	mEntitiesToRemove.clear(); //these entities were removed when releasing mEntities.
	ReleaseEntities(&mEntities);
	ReleaseEntities(&mNewEntitiesToAdd);	
}

void GameScene::Update(float elapsedTime)
{
	ReleaseEntities(&mEntitiesToRemove);
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

unsigned int GameScene::GetNumberGameEntities() const
{
	return mEntities.size();
}

std::vector<GameEntity*>& GameScene::GetAllGameEntities()
{
	return mEntities;
}

void GameScene::AddEntity(GameEntity* entity)
{
	if (entity != nullptr)
	{
		entity->Init();

		if (entity->GetRenderer() != nullptr)
		{
			mAABB = mAABB.Merge(entity->GetRenderer()->GetAABB());
		}
		mNewEntitiesToAdd.push_back(entity);

		AttachGameEntityComponent* component = entity->GetComponent<AttachGameEntityComponent>();
		if (component != nullptr)
		{
			AddEntity(component->GetGameEntityAttached());
		}
	}
}

void GameScene::RemoveEntity(GameEntity* entity)
{
	if (entity != nullptr)
	{
		mEntitiesToNotifyRemove.push_back(entity);
		AttachGameEntityComponent* component = entity->GetComponent<AttachGameEntityComponent>();
		if (component != nullptr)
		{
			RemoveEntity(component->GetGameEntityAttached());
		}
	}
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
	for (GameEntity* entity : mEntitiesToNotifyRemove)
	{
		GameEntitiesIterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](GameEntity* a) { return a == entity; });
		if (it != mEntities.end())
		{
			NotifyEntityRemoved(*it);
			mEntitiesToRemove.push_back(*it);
			mEntities.erase(it);
		}
		else
		{
			assert("Game entity not found when removing it!");
		}
	}
	mEntitiesToNotifyRemove.clear();
}

void GameScene::ReleaseEntities(std::vector<GameEntity*>* entities)
{
	for (GameEntity* entity : *entities)
	{
		if (entity->CanDeleteWhenRemovingFromScene())
		{
			delete entity;
		}
	}
	entities->clear();
}

BaseVisitable<>::ReturnType GameScene::Accept(BaseVisitor& guest)
{
	return AcceptImpl(*this, guest);
}