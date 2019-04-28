#pragma once

class IShaderProgram;
class IMaterialEffect;
class IMaterial;
class GameEntity;

class IFactory
{
public:
	virtual IShaderProgram* CreateShader() { return nullptr;  }
	virtual IMaterialEffect* AddNewEffectToMaterial(IMaterial* material) { return nullptr; }
	virtual GameEntity* CreateGameEntity() { return nullptr;  }
};
