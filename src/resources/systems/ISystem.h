#pragma once
#include "../../visitor/BaseVisitable.h"
#include "../scene/IGameSceneListener.h"
#include <vector>

class GameEntity;

class ISystem : public BaseVisitable<>, public IGameSceneListener
{
protected:
	std::vector<GameEntity*> mEntities;

public:
	ISystem();
	virtual ~ISystem();

	virtual void Start() {};
	virtual void Update(float deltaTime) = 0;
	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest) = 0;
	virtual bool HasToBeRegisteredToGameScene() const = 0;
private:

	virtual bool HasSpecificComponents(const GameEntity* entity) const = 0;

	virtual void OnGameEntityAdded(GameEntity* entity);
	virtual void OnGameEntityRemoved(GameEntity* entity);

protected:
	virtual void AddEntity(GameEntity* entity);
	virtual void RemoveEntity(GameEntity* entity);
};

