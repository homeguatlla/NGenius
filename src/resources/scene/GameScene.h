#pragma once
#include "quadtree/QuadTree.h"
#include <string>

class EntitiesSystem;
class RenderSystem;
class GameEntity;

class GameScene
{
	EntitiesSystem* mEntitiesSystem;
	RenderSystem* mRenderSystem;
	QuadTree mQuadTree;
	std::string mName;

public:
	GameScene(const std::string& name,  EntitiesSystem* mEntitiesSystem, RenderSystem* renderSystem);
	~GameScene();

	void Update();
	void AddGameEntity(GameEntity* entity);
	void RemoveGameEntity(GameEntity* entity);
};

