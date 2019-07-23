#include "stdafx.h"
#include "GameScene.h"
#include "IGameSceneListener.h"
#include "../systems/renderSystem/RenderSystem.h"
#include "../renderers/IRenderer.h"
#include "../components/LODComponent.h"
#include "../components/SpacePartitionComponent.h"

#include "../../utils/serializer/XMLSerializer.h"
#include "../InstantiableObject.h"

#include "../entities/GameEntity.h"
#include "../entities/Player.h"
#include "../entities/Terrain.h"
#include "../entities/Water.h"
#include "../Memory.h"

#include <algorithm>

GameScene::GameScene(const std::string& name, RenderSystem* renderSystem) :
mName(name),
mAABB(glm::vec3(std::numeric_limits<float>::max()), -glm::vec3(std::numeric_limits<float>::max())),
mRenderSystem { renderSystem }
{
	InstantiableObject::RegisterGameEntityType<GameEntity>();
	//InstantiableObject::RegisterType("player", DBG_NEW Player());
	InstantiableObject::RegisterGameEntityType<Terrain>();
	InstantiableObject::RegisterGameEntityType<Water>();
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

void GameScene::Start()
{
}

void GameScene::Update(float elapsedTime)
{
	RemoveEntities();
	AddNewEntities();

	for (IGameEntity* entity : mEntities)
	{
		entity->Update(elapsedTime);
	}
}

void GameScene::Render(RenderSystem* renderSystem)
{
	for (IGameEntity* entity : mEntities)
	{
		renderSystem->AddToRender(entity->GetRenderer());
	}
}

unsigned int GameScene::GetNumberGameEntities() const
{
	return mEntities.size();
}

std::vector<IGameEntity*>& GameScene::GetAllGameEntities()
{
	return mEntities;
}

void GameScene::AddEntity(IGameEntity* entity)
{
	entity->Build(mRenderSystem);
	entity->Init(mRenderSystem);

	if (entity->GetRenderer() != nullptr)
	{
		mAABB = mAABB.Merge(entity->GetRenderer()->GetAABB());
	}
	mNewEntitiesToAdd.push_back(entity);
}

void GameScene::RemoveEntity(IGameEntity* entity)
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
	for (IGameEntity* entity : mNewEntitiesToAdd)
	{
		mEntities.push_back(entity);
		NotifyEntityAdded(entity);
	}
	mNewEntitiesToAdd.clear();
}

void GameScene::NotifyEntityAdded(IGameEntity* entity)
{
	for (ListenersIterator it = mListeners.begin(); it != mListeners.end(); ++it)
	{
		(*it)->OnGameEntityAdded(entity);
	}
}

void GameScene::NotifyEntityRemoved(IGameEntity* entity)
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
		IGameEntity* entity = ReadEntityFrom(source);
		if (entity != nullptr)
		{
			ReadComponentsFrom(entity, source);
			AddEntity(entity);
		}
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
			for (IGameEntity* entity : mEntities)
			{
				entity->WriteTo(destination);
			}
		destination->EndAttribute();
	destination->EndAttribute();
}

IGameEntity* GameScene::ReadEntityFrom(core::utils::IDeserializer* source)
{
	IGameEntity* gameEntity = nullptr;
	
	std::string nodeName = source->GetCurrentNodeName();
	gameEntity = InstantiableObject::CreateEntity(nodeName);
	if (gameEntity != nullptr)
	{
		gameEntity->ReadFrom(source);
		return gameEntity;
	}
	return nullptr;
}

void GameScene::ReadComponentsFrom(IGameEntity* entity, core::utils::IDeserializer* source)
{
	if (source->HasAttribute("components"))
	{
		source->BeginAttribute(std::string("components"));
		unsigned int numElements = source->ReadNumberOfElements();

		source->BeginAttribute();
		do
		{
			ReadComponentFrom(entity, source);
			source->NextAttribute();
			numElements--;

		} while (numElements > 0);

		source->EndAttribute();
		source->EndAttribute();
	}
}

void GameScene::ReadComponentFrom(IGameEntity* entity, core::utils::IDeserializer* source)
{
	std::string componentType;
	source->ReadParameter("type", componentType);
	IComponent* component = InstantiableObject::CreateComponent(componentType, entity);
	if (component != nullptr)
	{
		component->ReadFrom(source);
	}
}

void GameScene::RemoveEntities()
{
	for (IGameEntity* entity : mEntitiesToRemove)
	{
		GameEntitiesIterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](IGameEntity* a) { return a == entity; });
		NotifyEntityRemoved(*it);
		
		mEntities.erase(it);
	}
	mEntitiesToRemove.clear();
}

void GameScene::ReleaseEntities(std::vector<IGameEntity*>* entities)
{
	for (IGameEntity* entity : *entities)
	{
		delete entity;
	}
	entities->clear();
}

BaseVisitable<>::ReturnType GameScene::Accept(BaseVisitor& guest)
{
	return AcceptImpl(*this, guest);
}