#pragma once

#include "../scene/GameScene.h"
#include "../scene/IGameSceneListener.h"
#include "../../Frustum.h"
#include <vector>

class IGameEntity;
class GameEntityQuadTree;
class ICamera;

class SpacePartitionSystem : public IGameSceneListener, BaseVisitable<>
{
	std::vector<std::shared_ptr<IGameEntity>> mNewEntitiesToAdd;
	std::vector<std::shared_ptr<IGameEntity>> mEntitiesToRemove;
	std::vector<std::shared_ptr<IGameEntity>> mLastQueryResult;
	GameEntityQuadTree* mQuadTree;
	AABB mAABB;
	bool mHasBuilt;

public:
	SpacePartitionSystem();
	~SpacePartitionSystem();

	void Start();
	void Update(float elapsedTime);
	void Reload();
	void MarkGameEntitiesInsideCameraAsVisible(ICamera* camera);

	void Query(const AABB& aabb, std::vector<std::shared_ptr<IGameEntity>>& result) const;
	void Query(const AABB& aabb, const Frustum& frustum, std::vector<std::shared_ptr<IGameEntity>>& result) const;

	unsigned int GetNumberEntities() const;

	void SetSpacePartitionComponentsEnabled(bool enable);

	BaseVisitable<>::ReturnType Accept(BaseVisitor& guest) override;

private:
	bool HasSpacePartitionComponents(const std::shared_ptr<IGameEntity> entity);
	void AddEntity(std::shared_ptr<IGameEntity> entity);
	void RemoveEntity(std::shared_ptr<IGameEntity> entity);
	
	void OnGameEntityAdded(std::shared_ptr<IGameEntity> entity) override;
	void OnGameEntityRemoved(std::shared_ptr<IGameEntity> entity) override;
	
	void RemoveEntities();
	void AddNewEntities();

	void Build();
	void Release();
	void UpdateVisibilityLastQueryResult();
};

