#include "stdafx.h"
#include "RiseTerrainCommand.h"
#include "../../entities/Terrain.h"
#include "../../../../GameConstants.h"

RiseTerrainCommand::RiseTerrainCommand(GameEntity* entity) : 
	ICommand(entity),
	mScale(0.0f)
{
}


RiseTerrainCommand::~RiseTerrainCommand()
{
}

void RiseTerrainCommand::Execute()
{
	mTerrain = static_cast<Terrain*>(mEntity);
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
