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
	using CameraCreatorFunction = std::function<ICamera* (const std::string& name, float, float, float, float)>;
	using ShaderCreatorFunction = std::function<IShaderProgram* ()>;
	using MaterialEffectCreatorFunction = std::function<IMaterialEffect* (IMaterial*)>;

	public:
		template<class T>
		static void RegisterMaterialEffectType()
		{
			mMaterialEffectFactory[T::GetClassName()] = std::bind<IMaterialEffect*>(&T::Create, std::placeholders::_1);
		}

		template<class T>
		static void RegisterShaderType()
		{
			mShadersFactory[T::GetClassName()] = std::bind<IShaderProgram*>(&T::Create);
		}

		template<class T>
		static void RegisterRendererType()
		{
			mRenderersFactory[T::GetClassName()] = std::bind<IRenderer*>(&T::Create, std::placeholders::_1, std::placeholders::_2);
		}

		template<class T>
		static void RegisterCameraType()
		{
			mCamerasFactory[T::GetClassName()] = std::bind<ICamera*>(&T::Create, std::placeholders::_1,
				std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
		}

		static void RegisterType(const std::string& name, IFactory* factory);
		static GameEntity* CreateEntity(const std::string& name);
		
		static IMaterialEffect* CreateMaterialEffect(const std::string& name, IMaterial* material);
		static ICamera* CreateOrthogonalCamera(const std::string& name, float screenWidth, float screenHeight, float nearPlane, float farPlane);
		static ICamera* CreatePerspectiveCamera(const std::string& name, float fov, float aspectRatio, float nearPlane, float farPlane);
		static IRenderer* CreateRenderer(const std::string& name, Model* model, IMaterial* material);
		static IShaderProgram* CreateShader(const std::string& name);

	private:
		static std::map<std::string, IFactory*> mFactories;
		static std::map<std::string, RendererCreatorFunction> mRenderersFactory;
		static std::map<std::string, CameraCreatorFunction> mCamerasFactory;
		static std::map<std::string, ShaderCreatorFunction> mShadersFactory;
		static std::map<std::string, MaterialEffectCreatorFunction> mMaterialEffectFactory;
};

