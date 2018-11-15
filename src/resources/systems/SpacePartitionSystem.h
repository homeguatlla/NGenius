#pragma once

#include "../scene/GameScene.h"
#include "../scene/IGameSceneListener.h"
#include "../../Frustum.h"
#include <vector>

class GameEntity;
class GameEntityQuadTree;
class ICamera;

class SpacePartitionSystem : public IGameSceneListener, BaseVisitable<>
{
	std::vector<GameEntity*> mNewEntitiesToAdd;
	std::vector<GameEntity*> mEntitiesToRemove;
	std::vector<GameEntity*> mLastQueryResult;
	GameEntityQuadTree* mQuadTree;
	AABB mAABB;
	bool mHasBuilt;

public:
	SpacePartitionSystem();
	~SpacePartitionSystem();

	void Start();
	void Update(float elapsedTime);
	void MarkGameEntitiesInsideCameraAsVisible(ICamera* camera);
	
	void FillWithGameEntitiesVisibleInsideRadius(const glm::vec3& origin, float radius, std::vector<std::pair<GameEntity*, float>>& list, bool isSorted = false);
	void Query(const AABB& aabb, std::vector<GameEntity*>& result) const;
	void Query(const AABB& aabb, const Frustum& frustum, std::vector<GameEntity*>& result) const;

	unsigned int GetNumberEntities() const;

	void SetSpacePartitionComponentsEnabled(bool enable);

	BaseVisitable<>::ReturnType Accept(BaseVisitor& guest) override;

private:
	bool HasSpacePartitionComponents(const GameEntity* entity);
	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);
	
	void OnGameEntityAdded(GameEntity* entity) override;
	void OnGameEntityRemoved(GameEntity* entity) override;
	
	void RemoveEntities();
	void AddNewEntities();

	void Build();
	void UpdateVisibilityLastQueryResult();
};

