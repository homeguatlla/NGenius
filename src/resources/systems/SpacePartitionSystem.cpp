#include "stdafx.h"
#include "SpacePartitionSystem.h"
#include "ISystem.h"
#include "renderSystem/RenderSystem.h"
#include "../scene/quadtree/GameEntityQuadTree.h"
#include "../GameEntity.h"
#include "../camera/ICamera.h"
#include "../camera/PerspectiveCamera.h"
#include "../components/SpacePartitionComponent.h"

#include <algorithm>

const float FOV_DILATATION = 12.0f;

SpacePartitionSystem::SpacePartitionSystem() : 
	mAABB(glm::vec3(std::numeric_limits<float>::max()), -glm::vec3(std::numeric_limits<float>::max())),
	mHasBuilt(false)
{
}

SpacePartitionSystem::~SpacePartitionSystem()
{
	mEntities.clear();
	mEntitiesToRemove.clear();
}

void SpacePartitionSystem::Start()
{
	mHasBuilt = false;
}

void SpacePartitionSystem::Build()
{
	mAABB.Expand(glm::vec3(5.0f));
	mQuadTree = new GameEntityQuadTree(mAABB);
	mHasBuilt = true;
}

void SpacePartitionSystem::Update(float elapsedTime)
{
	if (!mHasBuilt)
	{
		Build();
	}
	
	RemoveEntities();
	AddNewEntities();
}

bool SpacePartitionSystem::HasToBeRegisteredToGameScene() const
{
	return true;
}


void SpacePartitionSystem::MarkGameEntitiesInsideCameraAsVisible(ICamera* camera)
{
	assert(camera != nullptr);

	if (camera != nullptr)
	{
		UpdateVisibilityLastQueryResult();
		mLastQueryResult.clear();

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

void SpacePartitionSystem::FillWithGameEntitiesVisibleInsideRadius(const glm::vec3& origin, float radius, std::vector<std::pair<GameEntity*, float>>& list, bool isSorted) const
{
	list.clear();

	for (GameEntity* entity : mLastQueryResult)
	{
		Transformation* transformation = entity->GetTransformation();
		glm::vec3 position = transformation->GetPosition();
		float distance = glm::distance(origin, position);
		if (distance < radius)
		{
			list.push_back(std::pair<GameEntity*, float>(entity, distance));
		}
	}
	if (isSorted)
	{
		std::sort(list.begin(), list.end(), [](const std::pair<GameEntity*, float> a, const std::pair<GameEntity*, float> b) -> bool
		{
			return a.second < b.second;
		});
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

bool SpacePartitionSystem::HasSpecificComponents(const GameEntity* entity) const 
{
	return	entity->GetRenderer() != nullptr && entity->HasComponent<SpacePartitionComponent>() && 
			entity->GetRenderer()->GetLayer() != IRenderer::LAYER_GUI;
}

void SpacePartitionSystem::OnGameEntityAdded(GameEntity* entity)
{
	if (HasSpecificComponents(entity))
	{
		mAABB = mAABB.Merge(entity->GetRenderer()->GetAABB());
		AddEntity(entity);
	}
}

void SpacePartitionSystem::RemoveEntity(GameEntity* entity)
{
	mEntitiesToRemove.push_back(entity);
}

void SpacePartitionSystem::AddNewEntities()
{
	assert(mQuadTree != nullptr);

	for (GameEntity* entity : mEntities)
	{
		mQuadTree->AddGameEntity(entity);
	}
	mEntities.clear();
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