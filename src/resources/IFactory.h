#pragma once

class IShaderProgram;
class IMaterialEffect;
class IMaterial;
class IGameEntity;
class Transformation;
class ICamera;
class IRenderer;
class Model;

class IFactory
{
public:
	virtual IShaderProgram* CreateShader() { return nullptr;  }
	virtual IMaterialEffect* AddNewEffectToMaterial(IMaterial* material) { return nullptr; }
	virtual IGameEntity* CreateGameEntity() { return nullptr;  }
	virtual ICamera* CreateCamera() { return nullptr; }
	virtual IRenderer* CreateRenderer(Model* model, IMaterial* material) { return nullptr; }
};
