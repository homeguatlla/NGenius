#pragma once

#include "../visitor/BaseVisitor.h"
#include "../resources/systems/renderSystem/RenderSystem.h"

class Statistics : public BaseVisitor, Visitor<RenderSystem>
{
public:
	Statistics();
	~Statistics();

	void Update();
	
	void Visit(RenderSystem& renderSystem);
};

