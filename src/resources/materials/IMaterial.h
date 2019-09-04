#pragma once

#include <typeinfo>
#include <map>
#include <string>
#include <assert.h>

class IShaderProgram;
class IMaterialEffect;
class ICamera;
class Transformation;
class TexturesLibrary;

class IMaterial
{
	typedef std::map<const std::type_info*, IMaterialEffect*> MaterialEffectsMap;
	typedef MaterialEffectsMap::iterator IMaterialEffectIterator;

	static unsigned int IDCounter;

	unsigned int mMaterialID;
	IShaderProgram* mShader;

	MaterialEffectsMap mEffects;

public:
	IMaterial(IShaderProgram* shader);
	virtual ~IMaterial();

	unsigned int GetMaterialID() const;
	
	void Use();
	void UnUse();

	void Apply(const ICamera* camera, const Transformation* transformation);
	void CopyMaterialEffectsValuesFrom(IMaterial* material);

	IShaderProgram* GetShader();
	const IShaderProgram* GetShader() const;

	void Build(TexturesLibrary* texturesLibrary);
	virtual IMaterial* Clone() const;

	template<typename T> void AddEffect(T* effect)
	{
		static_assert(std::is_base_of<IMaterialEffect, T>::value, "The type must inherit from IMaterialEffect");
		assert(effect != nullptr);

		if (!HasEffect<T>())
		{
			effect->SetParent(this);
			mEffects[&typeid(T)] = effect;
		}
		else
		{
			delete effect;
		}
	}

	template<typename T> void AddOrReplaceEffect(T* effect)
	{
		static_assert(std::is_base_of<IMaterialEffect, T>::value, "The type must inherit from IMaterialEffect");
		assert(effect != nullptr);

		if (HasEffect<T>())
		{
			RemoveEffect<T>();
		}
		else
		{
			AddEffect(effect);
		}
	}

	template<typename T> void RemoveEffect()
	{
		static_assert(std::is_base_of<IMaterialEffect, T>::value, "The type must inherit from IMaterialEffect");
		if (HasEffect<T>())
		{
			T* effect = static_cast<T*>(mEffects[&typeid(T)]);
			mEffects.erase(&typeid(T));
			delete effect;
		}
	}

	template<typename T> bool HasEffect()
	{
		static_assert(std::is_base_of<IMaterialEffect, T>::value, "The type must inherit from IMaterialEffect");
		return mEffects.count(&typeid(T)) != 0;
	}

	template<typename T> T* GetEffect()
	{
		static_assert(std::is_base_of<IMaterialEffect, T>::value, "The type must inherit from IMaterialEffect");
		if (HasEffect<T>())
		{
			return static_cast<T*>(mEffects[&typeid(T)]);
		}
		else
		{
			return nullptr;
		}
	}

protected:
	virtual IMaterial* DoClone() const = 0;

private:
	template<typename T> void AddEffect(const std::type_info* type, T* effect)
	{
		static_assert(std::is_base_of<IMaterialEffect, T>::value, "The type must inherit from IMaterialEffect");
		assert(effect != nullptr);
		if (mEffects.count(type) == 0)
		{
			effect->SetParent(this);
			mEffects[type] = effect;
		}
		else
		{
			delete effect;
		}
	}
};

