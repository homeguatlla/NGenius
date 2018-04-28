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
}

void Statistics::Visit(RenderSystem& renderSystem)
{
	mNumberTrianglesRendered = renderSystem.GetNumberTrianglesRendered();
	mNumberDrawCalls = renderSystem.GetNumberDrawCalls();
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