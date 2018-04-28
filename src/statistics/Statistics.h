#pragma once

#include "../visitor/BaseVisitor.h"
#include "../NGenius.h"
#include "../resources/systems/renderSystem/RenderSystem.h"
#include "../resources/systems/PhysicsSystem.h"
#include "../resources/scene/GameScene.h"

class Statistics : 
	public BaseVisitor, 
	public Visitor<NGenius>, 
	public Visitor<RenderSystem>,
	public Visitor<GameScene>,
	public Visitor<PhysicsSystem>
{
	float mNumberFPS;
	unsigned int mNumberTrianglesRendered;
	unsigned int mNumberDrawCalls;
	unsigned int mNumberGameEntities;
	unsigned int mNumberGameEntitiesWithPhysics;

public:
	Statistics();
	~Statistics();
	
	void Visit(NGenius& nGenius);
	void Visit(RenderSystem& renderSystem);
	void Visit(GameScene& gameScene);
	void Visit(PhysicsSystem& physicsSystem);

	unsigned int GetNumberTrianglesRendered() const;
	unsigned int GetNumberDrawCalls() const;
	float GetNumberFPS() const;
	unsigned int GetNumberGameEntities() const;
	unsigned int GetNumberGameEntitiesWithPhysics() const;
};

