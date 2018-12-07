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

	virtual void Update(float deltaTime) = 0;
	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

private:

	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);
	virtual bool HasSpecificComponents(const GameEntity* entity) const = 0;

	void OnGameEntityAdded(GameEntity* entity) override;
	void OnGameEntityRemoved(GameEntity* entity) override;
};

