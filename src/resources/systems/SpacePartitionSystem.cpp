#include "stdafx.h"
#include "SpacePartitionSystem.h"
#include "renderSystem/RenderSystem.h"
#include "../scene/quadtree/GameEntityQuadTree.h"
#include "../GameEntity.h"
#include "../camera/ICamera.h"
#include "../camera/PerspectiveCamera.h"
#include "../components/SpacePartitionComponent.h"

#include <algorithm>

const float FOV_DILATATION = 5.0f;

SpacePartitionSystem::SpacePartitionSystem() : 
	mAABB(glm::vec3(std::numeric_limits<float>::max()), -glm::vec3(std::numeric_limits<float>::max())),
	mHasBuilt(false)
{
}

SpacePartitionSystem::~SpacePartitionSystem()
{
	mNewEntitiesToAdd.clear();
	mEntitiesToRemove.clear();
}

void SpacePartitionSystem::Start()
{
	mHasBuilt = false;
}

void SpacePartitionSystem::Build()
{
	mAABB.Expand(glm::vec3(5.0f));
	mQuadTree = DBG_NEW GameEntityQuadTree(mAABB);
	mHasBuilt = true;
}

void SpacePartitionSystem::Update(float elapsedTime)
{
	if (!mHasBuilt)
	{
		Build();
	}
	
	UpdateVisibilityLastQueryResult();
	mLastQueryResult.clear();

	RemoveEntities();
	AddNewEntities();
}

void SpacePartitionSystem::MarkGameEntitiesInsideCameraAsVisible(ICamera* camera)
{
	assert(camera != nullptr);

	if (camera != nullptr)
	{
		const AABB aabb = camera->GetAABB();
		camera->SetFrustumDilatation(FOV_DILATATION);
		const Frustum frustum = const_cast<ICamera*>(camera)->GetFrustum();
		
		//Query(aabb, entities);
		Query(aabb, frustum, mLastQueryResult);
		for (GameEntity* entity : mLastQueryResult)
		{
			entity->GetComponent<SpacePartitionComponent>()->SetVisibility(true);
		}
	}
}

void SpacePartitionSystem::UpdateVisibilityLastQueryResult()
{
	for (GameEntity* entity : mLastQueryResult)
	{
		entity->GetComponent<SpacePartitionComponent>()->SetVisibility(false);
	}
}

void SpacePartitionSystem::Query(const AABB& aabb, std::vector<GameEntity*>& result) const
{
	mQuadTree->Query(aabb, result);
}

void SpacePartitionSystem::Query(const AABB& aabb, const Frustum& frustum, std::vector<GameEntity*>& result) const
{
	mQuadTree->Query(aabb, frustum, result);
}

void SpacePartitionSystem::SetSpacePartitionComponentsEnabled(bool enable)
{
	std::vector<GameEntity*> result;

	Query(mAABB, result);

	for (GameEntity* entity : result)
	{
		entity->GetComponent<SpacePartitionComponent>()->SetEnabled(enable);
	}
}

unsigned int SpacePartitionSystem::GetNumberEntities() const
{
	return mQuadTree->GetNumEntities();
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
	return	entity->GetRenderer() != nullptr && entity->HasComponent<SpacePartitionComponent>() && 
			entity->GetRenderer()->GetLayer() != IRenderer::LAYER_GUI;
}

void SpacePartitionSystem::OnGameEntityAdded(GameEntity* entity)
{
	if (HasSpacePartitionComponents(entity))
	{
		mAABB = mAABB.Merge(entity->GetRenderer()->GetAABB());
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

BaseVisitable<>::ReturnType SpacePartitionSystem::Accept(BaseVisitor& guest)
{
	return AcceptImpl(*this, guest);
}