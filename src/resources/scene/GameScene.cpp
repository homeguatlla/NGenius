#include "stdafx.h"
#include "GameScene.h"
#include "IGameSceneListener.h"
#include "src/resources/systems/renderSystem/RenderSystem.h"
#include "src/resources/renderers/IRenderer.h"
#include "src/resources/components/LODComponent.h"
#include "src/resources/components/SpacePartitionComponent.h"

#include "src/utils/serializer/XMLSerializer.h"
#include "src/resources/InstantiableObject.h"

#include "src/resources/entities/GameEntity.h"
#include "src/resources/entities/Player.h"
#include "src/resources/entities/Terrain.h"
#include "src/resources/entities/Water.h"
#include "src/resources/entities/PointsPatch.h"
#include "src/resources/entities/EntitiesPatch.h"
#include "src/resources/entities/Particle.h"
#include "src/resources/entities/ParticlesEmitter.h"

#include "Memory.h"

#include <algorithm>

GameScene::GameScene(const std::string& name, NGenius* engine, RenderSystem* renderSystem) :
mName(name),
mAABB(glm::vec3(std::numeric_limits<float>::max()), -glm::vec3(std::numeric_limits<float>::max())),
mEngine { engine },
mRenderSystem { renderSystem }
{
	InstantiableObject::RegisterGameEntityType<GameEntity>();
	InstantiableObject::RegisterGameEntityType<Player>();
	InstantiableObject::RegisterGameEntityType<Terrain>();
	InstantiableObject::RegisterGameEntityType<Water>();
	InstantiableObject::RegisterGameEntityType<PointsPatch>();
	InstantiableObject::RegisterGameEntityType<EntitiesPatch>();
	InstantiableObject::RegisterGameEntityType<Particle>();
	InstantiableObject::RegisterGameEntityType<ParticlesEmitter>();
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
	//all entities on ground have its y modified to ground
	SetEntitiesOnGround();

}

void GameScene::Update(float elapsedTime)
{
	RemoveEntities();
	AddNewEntities();

	for (IGameEntity* entity : mEntities)
	{
		if (entity->IsEnabled())
		{
			entity->Update(elapsedTime);
		}
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

IGameEntity* GameScene::GetGameEntity(const std::string& name)
{
	std::vector<IGameEntity*>::iterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](IGameEntity* a) { return a->GetName() == name; });

	if (it != mEntities.end())
	{
		return *it;
	}
	else
	{
		std::vector<IGameEntity*>::iterator it = std::find_if(mNewEntitiesToAdd.begin(), mNewEntitiesToAdd.end(), [&](IGameEntity* a) { return a->GetName() == name; });
		if (it != mNewEntitiesToAdd.end())
		{
			return *it;
		}
		else
		{
			return nullptr;
		}
	}
}

IGameEntity* GameScene::GetGameEntity(int id)
{
	std::vector<IGameEntity*>::iterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](IGameEntity* a) { return a->GetID() == id; });

	if (it != mEntities.end())
	{
		return *it;
	}
	else
	{
		return nullptr;
	}
}

void GameScene::AddEntity(IGameEntity* entity)
{
	entity->Build(mEngine);
	
	if (entity->GetRenderer() != nullptr)
	{
		mAABB = mAABB.Merge(entity->GetRenderer()->GetAABB());
	}
	if (CheckIsGround(entity))
	{
		mGround = dynamic_cast<Terrain*>(entity);
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
	std::vector<IGameEntity*> mEntitiesAdded;

	//adding new entities
	for (IGameEntity* entity : mNewEntitiesToAdd)
	{
		mEntities.push_back(entity);
		mEntitiesAdded.push_back(entity);
	}
	mNewEntitiesToAdd.clear();

	//entities just added need to initialize.
	//Initialize can make some entities clone existing entities we want to be added in this frame
	for (IGameEntity* entity : mEntitiesAdded)
	{
		entity->Init(this, mRenderSystem);
		NotifyEntityAdded(entity);
		if (entity->ShouldBeCreatedOnGround())
		{
			SetEntityOnGround(entity);
		}
	}

	mEntitiesAdded.clear();

	if (!mNewEntitiesToAdd.empty())
	{
		AddNewEntities();
	}
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

bool GameScene::CheckIsGround(IGameEntity* entity)
{
	return typeid(*entity) == typeid(Terrain);
}

void GameScene::SetEntitiesOnGround()
{
	if (mGround != nullptr)
	{
		for (auto&& entity : mEntities)
		{
			if (entity->ShouldBeCreatedOnGround())
			{
				SetEntityOnGround(entity);
			}
		}
	}
}

void GameScene::SetEntityOnGround(IGameEntity* entity)
{
	if (mGround != nullptr)
	{
		glm::vec3 position = entity->GetTransformation()->GetPosition();
		float groundY = mGround->GetHeight(glm::vec2(position.x, position.z));
		float min = entity->GetRenderer()->GetAABB().GetVertexMin().y;

		position.y = min < groundY ? groundY - min : min - groundY;
		entity->GetTransformation()->SetPosition(position);
	}
}
