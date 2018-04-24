#pragma once
#include <string>
#include "../../AABB.h"

class EntitiesSystem;
class RenderSystem;
class GameEntity;
class ISpacePartition;

class GameScene
{
	EntitiesSystem* mEntitiesSystem;
	RenderSystem* mRenderSystem;
	ISpacePartition* mSceneGraph;
	std::string mName;
	AABB mAABB;

public:
	GameScene(const std::string& name,  EntitiesSystem* mEntitiesSystem, RenderSystem* renderSystem);
	~GameScene();

	void Init();
	void Update();
	void AddGameEntity(GameEntity* entity);
	void RemoveGameEntity(GameEntity* entity);

};

