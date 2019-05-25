#pragma once
#include "IFactory.h"
#include <string>
#include <map>

class IShaderProgram;
class IMaterialEffect;
class IMaterial;
class Transformation;
class GameEntity;

class InstantiableObject
{
	public:
		static void RegisterType(const std::string& name, IFactory* factory);
		static IShaderProgram* CreateShader(const std::string& name);
		static IMaterialEffect* AddNewEffectToMaterial(const std::string& name, IMaterial* material);
		static GameEntity* CreateEntity(const std::string& name);
	private:
		static std::map<std::string, IFactory*> mFactories;
};

