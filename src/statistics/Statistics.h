#pragma once

#include "../visitor/BaseVisitor.h"
#include "../NGenius.h"
#include "../resources/systems/renderSystem/RenderSystem.h"
#include "../resources/systems/PhysicsSystem.h"
#include "../resources/systems/SpacePartitionSystem.h"
#include "../resources/scene/GameScene.h"

class Statistics : 
	public BaseVisitor, 
	public Visitor<NGenius>, 
	public Visitor<RenderSystem>,
	public Visitor<GameScene>,
	public Visitor<PhysicsSystem>,
	public Visitor<SpacePartitionSystem>
{
	float mNumberFPS;
	unsigned int mNumberTrianglesRendered;
	unsigned int mNumberDrawCalls;
	unsigned int mNumberGameEntities;
	unsigned int mNumberGameEntitiesInsideSpacePartition;
	unsigned int mNumberGameEntitiesWithPhysics;
	unsigned int mNumberRenderers;
	float mDayTime;

public:
	Statistics();
	~Statistics();
	
	void Visit(const NGenius& nGenius);
	void Visit(const RenderSystem& renderSystem);
	void Visit(const GameScene& gameScene);
	void Visit(const PhysicsSystem& physicsSystem);
	void Visit(const SpacePartitionSystem& spacePartitionSystem);

	unsigned int GetNumberTrianglesRendered() const;
	unsigned int GetNumberDrawCalls() const;
	float GetNumberFPS() const;
	unsigned int GetNumberGameEntities() const;
	unsigned int GetNumberGameEntitiesWithPhysics() const;
	unsigned int GetNumberGameEntitiesInsideSpacePartition() const;
	unsigned int GetNumberRenderers() const;
	float GetDayTime() const;
};

