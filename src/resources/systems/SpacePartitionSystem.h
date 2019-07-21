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
	std::vector<IGameEntity*> mNewEntitiesToAdd;
	std::vector<IGameEntity*> mEntitiesToRemove;
	std::vector<IGameEntity*> mLastQueryResult;
	GameEntityQuadTree* mQuadTree;
	AABB mAABB;
	bool mHasBuilt;

public:
	SpacePartitionSystem();
	~SpacePartitionSystem();

	void Start();
	void Update(float elapsedTime);
	void MarkGameEntitiesInsideCameraAsVisible(ICamera* camera);

	void Query(const AABB& aabb, std::vector<IGameEntity*>& result) const;
	void Query(const AABB& aabb, const Frustum& frustum, std::vector<IGameEntity*>& result) const;

	unsigned int GetNumberEntities() const;

	void SetSpacePartitionComponentsEnabled(bool enable);

	BaseVisitable<>::ReturnType Accept(BaseVisitor& guest) override;

private:
	bool HasSpacePartitionComponents(const IGameEntity* entity);
	void AddEntity(IGameEntity* entity);
	void RemoveEntity(IGameEntity* entity);
	
	void OnGameEntityAdded(IGameEntity* entity) override;
	void OnGameEntityRemoved(IGameEntity* entity) override;
	
	void RemoveEntities();
	void AddNewEntities();

	void Build();
	void Release();
	void UpdateVisibilityLastQueryResult();
};

