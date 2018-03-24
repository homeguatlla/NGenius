#pragma once

#include <vector>
class GameEntity;
class RenderSystem;
class PhysicsSystem;
class InputSystem;
class DebugSystem;

class EntitiesSystem
{
	std::vector<GameEntity*> mEntities;
	std::vector<GameEntity*> mNewEntitiesToAdd;
	std::vector<GameEntity*> mEntitiesToRemove;
	typedef std::vector<GameEntity*>::iterator GameEntitiesIterator;

	RenderSystem* mRenderSystem;
	PhysicsSystem* mPhysicsSystem;
	InputSystem* mInputSystem;
	DebugSystem* mDebugSystem;

public:
	EntitiesSystem(RenderSystem* rendererSystem, PhysicsSystem* physicsSystem, InputSystem* inputSystem, DebugSystem* debugSystem);
	~EntitiesSystem();

	void Update(float elapsedTime);
	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);

private:
	void ReleaseEntities(std::vector<GameEntity*>* entities);
	void RemoveEntities();
	void AddNewEntities();
};

