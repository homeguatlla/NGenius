#pragma once
#include "../ICommand.h"

class Terrain;

class RiseTerrainCommand : public ICommand
{
	Terrain* mTerrain;
	float mScale;

public:
	RiseTerrainCommand(GameEntity* entity);
	~RiseTerrainCommand();

	void Execute() override;
	bool HasFinished() const override;
	void Update(float elapsedTime) override;
};
