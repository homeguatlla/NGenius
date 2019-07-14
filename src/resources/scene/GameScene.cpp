#include "stdafx.h"
#include "GameScene.h"
#include "IGameSceneListener.h"
#include "../GameEntity.h"
#include "../systems/renderSystem/RenderSystem.h"
#include "../renderers/IRenderer.h"
#include "../components/LODComponent.h"
#include "../components/SpacePartitionComponent.h"

#include "../../utils/serializer/XMLSerializer.h"
#include "../InstantiableObject.h"

#include "../entities/Player.h"
#include "../entities/Terrain.h"
#include "../entities/Water.h"
#include "../Memory.h"

#include <algorithm>

GameScene::GameScene(const std::string& name) :
mName(name),
mAABB(glm::vec3(std::numeric_limits<float>::max()), -glm::vec3(std::numeric_limits<float>::max()))
{
	InstantiableObject::RegisterType("entity", DBG_NEW GameEntity());
	//InstantiableObject::RegisterType("player", DBG_NEW Player());
	InstantiableObject::RegisterType("terrain", DBG_NEW Terrain());
	InstantiableObject::RegisterType("water", DBG_NEW Water());
}

GameScene::~GameScene()
{
	Release();
}

void GameScene::Release()
{
	mEntitiesToRemove.clear(); //these entities were removed when releasing mEntities.
	ReleaseEntities(&mEntities);
	ReleaseEntities(&mNewEntitiesToAdd);
}

void GameScene::Start(RenderSystem* renderSystem)
{
	
	for (GameEntity* entity : mNewEntitiesToAdd)
	{
		entity->Build(renderSystem);
	}
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

void GameScene::SaveToFile()
{
	core::utils::XMLSerializer xmlSerializer;
	WriteTo(&xmlSerializer);

	std::string filename;
	filename = mName + ".xml";
	xmlSerializer.Save(filename);
}

void GameScene::ReadFrom(core::utils::IDeserializer* source)
{
	Release();
	source->BeginAttribute(std::string("game_scene"));
	source->ReadParameter("name", mName);

	source->BeginAttribute(std::string("entities"));
	unsigned int numElements = source->ReadNumberOfElements();

	source->BeginAttribute();
	do
	{
		ReadEntityFrom(source);

		source->NextAttribute();
		numElements--;

	} while (numElements > 0);

	//source->EndAttribute();
	source->EndAttribute();
	source->EndAttribute();
}

void GameScene::WriteTo(core::utils::ISerializer* destination)
{
	destination->BeginAttribute(std::string("game_scene"));
		destination->WriteParameter(std::string("name"), mName);
		destination->BeginAttribute(std::string("entities"));
		destination->WriteParameter(std::string("counter"), mEntities.size());
			for (GameEntity* entity : mEntities)
			{
				entity->WriteTo(destination);
			}
		destination->EndAttribute();
	destination->EndAttribute();
}

void GameScene::ReadEntityFrom(core::utils::IDeserializer* source)
{
	GameEntity* gameEntity = nullptr;
	
	std::string nodeName = source->GetCurrentNodeName();
	gameEntity = InstantiableObject::CreateEntity(nodeName);
	if (gameEntity != nullptr)
	{
		gameEntity->ReadFrom(source);
		AddEntity(gameEntity);
	}
}

void GameScene::RemoveEntities()
{
	for (GameEntity* entity : mEntitiesToRemove)
	{
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