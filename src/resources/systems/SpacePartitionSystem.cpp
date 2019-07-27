#include "stdafx.h"
#include "SpacePartitionSystem.h"
#include "renderSystem/RenderSystem.h"
#include "../scene/quadtree/GameEntityQuadTree.h"
#include "../IGameEntity.h"
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
	Release();
}

void SpacePartitionSystem::Start()
{
	mHasBuilt = false;
}

void SpacePartitionSystem::Build()
{
	if (mHasBuilt)
	{
		Release();
	}
	mAABB.Expand(glm::vec3(5.0f));
	mQuadTree = DBG_NEW GameEntityQuadTree(mAABB);
	mHasBuilt = true;
}

void SpacePartitionSystem::Release()
{
	mNewEntitiesToAdd.clear();
	mEntitiesToRemove.clear();
	mLastQueryResult.clear();
	delete mQuadTree;
	mQuadTree = nullptr;
	mHasBuilt = false;
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

void SpacePartitionSystem::Reload()
{
	Release();
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
		for (IGameEntity* entity : mLastQueryResult)
		{
			entity->GetComponent<SpacePartitionComponent>()->SetVisibility(true);
		}
	}
}

void SpacePartitionSystem::UpdateVisibilityLastQueryResult()
{
	for (IGameEntity* entity : mLastQueryResult)
	{
		entity->GetComponent<SpacePartitionComponent>()->SetVisibility(false);
	}
}

void SpacePartitionSystem::Query(const AABB& aabb, std::vector<IGameEntity*>& result) const
{
	mQuadTree->Query(aabb, result);
}

void SpacePartitionSystem::Query(const AABB& aabb, const Frustum& frustum, std::vector<IGameEntity*>& result) const
{
	mQuadTree->Query(aabb, frustum, result);
}

void SpacePartitionSystem::SetSpacePartitionComponentsEnabled(bool enable)
{
	std::vector<IGameEntity*> result;

	Query(mAABB, result);

	for (IGameEntity* entity : result)
	{
		entity->GetComponent<SpacePartitionComponent>()->SetEnabled(enable);
	}
}

unsigned int SpacePartitionSystem::GetNumberEntities() const
{
	if (mHasBuilt)
	{
		return mQuadTree->GetNumEntities();
	}
	else
	{
		return 0;
	}
}

void SpacePartitionSystem::AddEntity(IGameEntity* entity)
{
	mNewEntitiesToAdd.push_back(entity);
}

void SpacePartitionSystem::RemoveEntity(IGameEntity* entity)
{
	mEntitiesToRemove.push_back(entity);
}

bool SpacePartitionSystem::HasSpacePartitionComponents(const IGameEntity* entity)
{
	return	entity->GetRenderer() != nullptr && entity->HasComponent<SpacePartitionComponent>() && 
			entity->GetRenderer()->GetLayer() != IRenderer::LAYER_GUI;
}

void SpacePartitionSystem::OnGameEntityAdded(IGameEntity* entity)
{
	if (HasSpacePartitionComponents(entity))
	{
		mAABB = mAABB.Merge(entity->GetRenderer()->GetAABB());
		AddEntity(entity);
	}
}

void SpacePartitionSystem::OnGameEntityRemoved(IGameEntity* entity)
{
	if (HasSpacePartitionComponents(entity))
	{
		RemoveEntity(entity);
	}
}

void SpacePartitionSystem::AddNewEntities()
{
	assert(mQuadTree != nullptr);

	for (IGameEntity* entity : mNewEntitiesToAdd)
	{
		mQuadTree->AddGameEntity(entity);
	}
	mNewEntitiesToAdd.clear();
}

void SpacePartitionSystem::RemoveEntities()
{
	assert(mQuadTree != nullptr);

	for (IGameEntity* entity : mEntitiesToRemove)
	{
		mQuadTree->RemoveGameEntity(entity);
	}
	mEntitiesToRemove.clear();
}

BaseVisitable<>::ReturnType SpacePartitionSystem::Accept(BaseVisitor& guest)
{
	return AcceptImpl(*this, guest);
}