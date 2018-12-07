#pragma once
#include "ISystem.h"
#include <vector>

class GameEntity;

class DamageSystem : public ISystem
{
	typedef std::vector<GameEntity*>::iterator GameEntitiesIterator;
	bool mIsInitialized;

public:

	DamageSystem();
	~DamageSystem();

	void Start() override;

	void Update(float elapsedTime) override;
	bool HasToBeRegisteredToGameScene() const override;

private:

	bool HasSpecificComponents(const GameEntity* entity) const override;

	// Heredado vía ISystem
	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor & guest) override;
};

