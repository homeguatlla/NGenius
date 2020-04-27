#include "stdafx.h"
#include "RiseTerrainCommand.h"
#include "src/resources/entities/Terrain.h"
#include "src/resources/IGameEntity.h"
#include "GameConstants.h"

RiseTerrainCommand::RiseTerrainCommand(std::shared_ptr<IGameEntity> entity) :
	ICommand(entity),
	mScale(0.0f)
{
}


RiseTerrainCommand::~RiseTerrainCommand()
{
}

void RiseTerrainCommand::Execute()
{
	mTerrain = std::static_pointer_cast<Terrain>(mEntity);
	mTerrain->SetScale(mScale);
}

bool RiseTerrainCommand::HasFinished() const
{
	return mScale >= TERRAIN_SCALE;
}

void RiseTerrainCommand::Update(float elapsedTime)
{
	mScale += 2.0f * elapsedTime;
	mTerrain->SetScale(mScale);
}
