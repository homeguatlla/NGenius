#pragma once
#include "GameEntity.h"
#include "shaders/IShaderProgram.h"

class IFactory
{
public:
	virtual IShaderProgram* CreateShader() { return nullptr;  }
	virtual GameEntity* CreateGameEntity() { return nullptr;  }
};
