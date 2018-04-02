#pragma once

#include <vector>

#include "../../visitor/BaseVisitable.h"

class GameEntity;
class RenderSystem;
class PhysicsSystem;
class InputSystem;
class DebugSystem;

class EntitiesSystem : public BaseVisitable<>
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

	unsigned int GetNumberGameEntities() const;

	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

private:
	void ReleaseEntities(std::vector<GameEntity*>* entities);
	void RemoveEntities();
	void AddNewEntities();
};

