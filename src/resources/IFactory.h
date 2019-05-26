#pragma once

class IShaderProgram;
class IMaterialEffect;
class IMaterial;
class GameEntity;
class Transformation;
class ICamera;

class IFactory
{
public:
	virtual IShaderProgram* CreateShader() { return nullptr;  }
	virtual IMaterialEffect* AddNewEffectToMaterial(IMaterial* material) { return nullptr; }
	virtual GameEntity* CreateGameEntity() { return nullptr;  }
	virtual ICamera* CreateCamera() { return nullptr; }
};
