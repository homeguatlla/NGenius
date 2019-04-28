#pragma once
#include "IFactory.h"
#include <string>
#include <map>

class IShaderProgram;
class IMaterialEffect;
class IMaterial;

class InstantiableObject
{
	public:
		static void RegisterType(const std::string& name, IFactory* factory);
		static IShaderProgram* CreateShader(const std::string& name);
		static IMaterialEffect* AddNewEffectToMaterial(const std::string& name, IMaterial* material);
	private:
		static std::map<std::string, IFactory*> mFactories;
};

