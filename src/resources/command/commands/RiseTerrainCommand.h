#pragma once
#include "src/resources/command/ICommand.h"

class Terrain;

class RiseTerrainCommand : public ICommand
{
	std::shared_ptr<Terrain> mTerrain;
	float mScale;

public:
	RiseTerrainCommand(std::shared_ptr<IGameEntity> entity);
	~RiseTerrainCommand();

	void Execute() override;
	bool HasFinished() const override;
	void Update(float elapsedTime) override;
};

