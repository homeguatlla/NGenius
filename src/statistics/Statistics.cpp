#include "stdafx.h"
#include "statistics.h"
#include <iostream>

Statistics::Statistics() : 
	mNumberFPS(0.0f), 
	mNumberTrianglesRendered(0), 
	mNumberDrawCalls(0),
	mNumberGameEntities(0),
	mNumberGameEntitiesWithPhysics(0)
{
}

Statistics::~Statistics()
{
}

void Statistics::Visit(NGenius& nGenius)
{
	mNumberFPS = nGenius.GetNumberFPS();
	mDayTime = nGenius.GetDayTime();
}

void Statistics::Visit(RenderSystem& renderSystem)
{
	mNumberTrianglesRendered = renderSystem.GetNumberTrianglesRendered();
	mNumberDrawCalls = renderSystem.GetNumberDrawCalls();
	mNumberRenderers = renderSystem.GetNumberRenderers();
	//std::cout << "triangles: " << mNumberTrianglesRendered << " calls: " << mNumberDrawCalls << "\n";
}

void Statistics::Visit(GameScene& gameScene)
{
	mNumberGameEntities = gameScene.GetNumberGameEntities();
}

void Statistics::Visit(PhysicsSystem& physicsSystem)
{
	mNumberGameEntitiesWithPhysics = physicsSystem.GetNumberGameEntities();
}

void Statistics::Visit(SpacePartitionSystem& spacePartitionSystem)
{
	mNumberGameEntitiesInsideSpacePartition = spacePartitionSystem.GetNumberEntities();
}

unsigned int Statistics::GetNumberTrianglesRendered() const
{
	return mNumberTrianglesRendered;
}

unsigned int Statistics::GetNumberDrawCalls() const
{
	return mNumberDrawCalls;
}

float Statistics::GetNumberFPS() const
{
	return mNumberFPS;
}

unsigned int Statistics::GetNumberGameEntities() const
{
	return mNumberGameEntities;
}

unsigned int Statistics::GetNumberGameEntitiesWithPhysics() const
{
	return mNumberGameEntitiesWithPhysics;
}

unsigned int Statistics::GetNumberGameEntitiesInsideSpacePartition() const
{
	return mNumberGameEntitiesInsideSpacePartition;
}

unsigned int Statistics::GetNumberRenderers() const
{
	return mNumberRenderers;
}

float Statistics::GetDayTime() const
{
	return mDayTime;
}