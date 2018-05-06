#include "stdafx.h"
#include "SpacePartitionSystem.h"
#include "../scene/quadtree/GameEntityQuadTree.h"
#include "../GameEntity.h"

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
	mQuadTree = new GameEntityQuadTree(gameScene.GetAABB());
}

void SpacePartitionSystem::Update(float elapsedTime)
{
	RemoveEntities();
	AddNewEntities();
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
	return entity->GetRenderer() != nullptr;
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
