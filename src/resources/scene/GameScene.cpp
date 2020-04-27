#include "stdafx.h"
#include "GameScene.h"
#include "IGameSceneListener.h"
#include "src/resources/systems/renderSystem/RenderSystem.h"
#include "src/resources/renderers/IRenderer.h"
#include "src/resources/components/LODComponent.h"
#include "src/resources/components/SpacePartitionComponent.h"
#include "src/resources/components/PhysicsComponent.h"

#include "src/utils/serializer/XMLSerializer.h"
#include "src/resources/InstantiableObject.h"

#include "src/resources/entities/GameEntity.h"
#include "src/resources/entities/player/Player.h"
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
	mEntities.clear();
	mNewEntitiesToAdd.clear();
}

void GameScene::Start()
{
	RemoveEntities();
	AddNewEntities();
}

void GameScene::Update(float elapsedTime)
{
	RemoveEntities();
	AddNewEntities();

	for (auto&& entity : mEntities)
	{
		if (entity->IsEnabled())
		{
			entity->Update(elapsedTime);
		}
	}
}

void GameScene::Render(RenderSystem* renderSystem)
{
	for (auto&& entity : mEntities)
	{
		renderSystem->AddToRender(entity->GetRenderer());
	}
}

unsigned int GameScene::GetNumberGameEntities() const
{
	return mEntities.size();
}

std::vector<std::shared_ptr<IGameEntity>>& GameScene::GetAllGameEntities()
{
	return mEntities;
}

std::shared_ptr<IGameEntity> GameScene::GetGameEntity(const std::string& name)
{
	std::vector<std::shared_ptr<IGameEntity>>::iterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](std::shared_ptr<IGameEntity> a) { return a->GetName() == name; });

	if (it != mEntities.end())
	{
		return *it;
	}
	else
	{
		std::vector<std::shared_ptr<IGameEntity>>::iterator it = std::find_if(mNewEntitiesToAdd.begin(), mNewEntitiesToAdd.end(), [&](std::shared_ptr<IGameEntity> a) { return a->GetName() == name; });
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

std::shared_ptr<IGameEntity> GameScene::GetGameEntity(int id)
{
	std::vector<std::shared_ptr<IGameEntity>>::iterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](std::shared_ptr<IGameEntity> a) { return a->GetID() == id; });

	if (it != mEntities.end())
	{
		return *it;
	}
	else
	{
		return nullptr;
	}
}

void GameScene::AddEntity(std::shared_ptr<IGameEntity> entity)
{
	entity->Build(mEngine);
	
	if (entity->GetRenderer() != nullptr)
	{
		mAABB = mAABB.Merge(entity->GetRenderer()->GetAABB());
	}
	if (CheckIsGround(entity))
	{
		mGround = std::dynamic_pointer_cast<Terrain>(entity);
	}

	mNewEntitiesToAdd.push_back(entity);
}

void GameScene::RemoveEntity(std::shared_ptr<IGameEntity> entity)
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
	std::vector<std::shared_ptr<IGameEntity>> mEntitiesAdded;

	//adding new entities
	for (auto&& entity : mNewEntitiesToAdd)
	{
		mEntities.push_back(std::shared_ptr<IGameEntity>(entity));
		mEntitiesAdded.push_back(std::shared_ptr<IGameEntity>(entity));
	}
	mNewEntitiesToAdd.clear();

	//entities just added need to initialize.
	//Initialize can make some entities clone existing entities we want to be added in this frame
	for (auto&& entity : mEntitiesAdded)
	{
		entity->Init(this, mRenderSystem);
		if (entity->ShouldBeCreatedOnGround())
		{
			SetEntityOnGround(entity);
		}
		NotifyEntityAdded(entity);
	}

	mEntitiesAdded.clear();

	if (!mNewEntitiesToAdd.empty())
	{
		AddNewEntities();
	}
}

void GameScene::NotifyEntityAdded(std::shared_ptr<IGameEntity> entity)
{
	for (ListenersIterator it = mListeners.begin(); it != mListeners.end(); ++it)
	{
		(*it)->OnGameEntityAdded(entity);
	}
}

void GameScene::NotifyEntityRemoved(std::shared_ptr<IGameEntity> entity)
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
		std::shared_ptr<IGameEntity> entity = ReadEntityFrom(source);
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
			for (auto&& entity : mEntities)
			{
				entity->WriteTo(destination);
			}
		destination->EndAttribute();
	destination->EndAttribute();
}

std::shared_ptr<IGameEntity> GameScene::ReadEntityFrom(core::utils::IDeserializer* source)
{	
	std::string nodeName = source->GetCurrentNodeName();
	auto gameEntity = InstantiableObject::CreateEntity(nodeName);
	if (gameEntity != nullptr)
	{
		gameEntity->ReadFrom(source);
		return gameEntity;
	}
	return nullptr;
}

void GameScene::ReadComponentsFrom(std::shared_ptr<IGameEntity> entity, core::utils::IDeserializer* source)
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

void GameScene::ReadComponentFrom(std::shared_ptr<IGameEntity> entity, core::utils::IDeserializer* source)
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
	for (auto&& entity : mEntitiesToRemove)
	{
		GameEntitiesIterator it = std::find_if(mEntities.begin(), mEntities.end(), [&](std::shared_ptr<IGameEntity> a) { return a == entity; });
		NotifyEntityRemoved(*it);
		
		mEntities.erase(it);
	}
	mEntitiesToRemove.clear();
}

BaseVisitable<>::ReturnType GameScene::Accept(BaseVisitor& guest)
{
	return AcceptImpl(*this, guest);
}

bool GameScene::CheckIsGround(std::shared_ptr<IGameEntity> entity)
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

void GameScene::SetEntityOnGround(std::shared_ptr<IGameEntity> entity)
{
	if (mGround != nullptr)
	{
		glm::vec3 position = entity->GetTransformation()->GetPosition();
		float groundY = mGround->GetHeight(glm::vec2(position.x, position.z));
		float minY = 0.0f;
		if (entity->HasComponent<PhysicsComponent>())
		{
			auto physicsComponent = entity->GetComponent<PhysicsComponent>();
			auto boundingVolume = physicsComponent->GetPhysicsBoundingVolume();
			minY = - boundingVolume->GetSize().y * 0.5f + physicsComponent->GetTranslation().y;
		}
		else
		{
			minY = entity->GetRenderer()->GetModel()->GetAABB().GetVertexMin().y;			
		}
		position.y = groundY - minY;
		entity->GetTransformation()->SetPosition(position);
	}
}
