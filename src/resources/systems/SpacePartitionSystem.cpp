#include "stdafx.h"
#include "SpacePartitionSystem.h"
#include "renderSystem/RenderSystem.h"
#include "../scene/quadtree/GameEntityQuadTree.h"
#include "../GameEntity.h"
#include "../camera/ICamera.h"
#include "../components/SpacePartitionComponent.h"

#include <algorithm>

SpacePartitionSystem::SpacePartitionSystem()
{
}

SpacePartitionSystem::~SpacePartitionSystem()
{
	mNewEntitiesToAdd.clear();
	mEntitiesToRemove.clear();
}

void SpacePartitionSystem::Start(const GameScene& gameScene)
{
	AABB aabb = gameScene.GetAABB();
	aabb.Expand(glm::vec3(1.0f));
	mQuadTree = new GameEntityQuadTree(aabb);
}

void SpacePartitionSystem::Update(float elapsedTime)
{
	RemoveEntities();
	AddNewEntities();
}

void SpacePartitionSystem::Render(RenderSystem* renderSystem)
{
	const ICamera* camera = renderSystem->GetCamera("gameplay_camera");
	assert(camera != nullptr);

	if (camera != nullptr)
	{
		const AABB aabb = camera->GetAABB();
		std::vector<GameEntity*> entities;
		Query(aabb, entities);

		for (GameEntity* entity : entities)
		{
			renderSystem->AddToRender(entity->GetRenderer());
		}
	}
}

void SpacePartitionSystem::Query(const AABB& aabb, std::vector<GameEntity*>& result) const
{
	mQuadTree->Query(aabb, result);
}

void SpacePartitionSystem::AddEntity(GameEntity* entity)
{
	mNewEntitiesToAdd.push_back(entity);
}

void SpacePartitionSystem::RemoveEntity(GameEntity* entity)
{
	mEntitiesToRemove.push_back(entity);
}

bool SpacePartitionSystem::HasSpacePartitionComponents(const GameEntity* entity)
{
	return entity->GetRenderer() != nullptr && entity->HasComponent<SpacePartitionComponent>();
}

void SpacePartitionSystem::OnGameEntityAdded(GameEntity* entity)
{
	if (HasSpacePartitionComponents(entity))
	{
		AddEntity(entity);
	}
}

void SpacePartitionSystem::OnGameEntityRemoved(GameEntity* entity)
{
	if (HasSpacePartitionComponents(entity))
	{
		RemoveEntity(entity);
	}
}

void SpacePartitionSystem::AddNewEntities()
{
	assert(mQuadTree != nullptr);

	for (GameEntity* entity : mNewEntitiesToAdd)
	{
		mQuadTree->AddGameEntity(entity);
	}
	mNewEntitiesToAdd.clear();
}

void SpacePartitionSystem::RemoveEntities()
{
	assert(mQuadTree != nullptr);

	for (GameEntity* entity : mEntitiesToRemove)
	{
		mQuadTree->RemoveGameEntity(entity);
	}
	mEntitiesToRemove.clear();
}
