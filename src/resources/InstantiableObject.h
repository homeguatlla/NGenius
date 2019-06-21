#pragma once
#include "IFactory.h"
#include <string>
#include <map>
#include <functional>

class IShaderProgram;
class IMaterialEffect;
class IMaterial;
class Model;
class IRenderer;
class Transformation;
class GameEntity;

class InstantiableObject
{
private:
	using RendererCreatorFunction = std::function<IRenderer* (Model*, IMaterial*)> ;

	public:

		template<class T>
		static void RegisterType()
		{
			mRenderersFactory[T::GetClassName()] = std::bind<IRenderer*>(&T::Create, std::placeholders::_1, std::placeholders::_2);
		}

		static void RegisterType(const std::string& name, IFactory* factory);
		static IShaderProgram* CreateShader(const std::string& name);
		static IMaterialEffect* AddNewEffectToMaterial(const std::string& name, IMaterial* material);
		static GameEntity* CreateEntity(const std::string& name);
		static ICamera* CreateCamera(const std::string& name);
		static IRenderer* CreateRenderer(const std::string& name, Model* model, IMaterial* material);

	private:
		static std::map<std::string, IFactory*> mFactories;
		static std::map<std::string, RendererCreatorFunction> mRenderersFactory;
};

