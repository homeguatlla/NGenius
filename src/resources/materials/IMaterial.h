#pragma once

#include <typeinfo>
#include <map>

class IShaderProgram;
class IMaterialEffect;

class IMaterial
{
	typedef std::map<const std::type_info*, IMaterialEffect*> MaterialEffectsMap;
	typedef MaterialEffectsMap::iterator IMaterialEffectIterator;

	static unsigned int IDCounter;

	unsigned int mMaterialID;
	IShaderProgram* mShader;

	MaterialEffectsMap mEffects;

protected:

public:
	IMaterial(IShaderProgram* shader);
	virtual ~IMaterial();

	unsigned int GetMaterialID() const;
	
	IShaderProgram* GetShader();

	virtual IMaterial* Clone() const;

	template<typename T> void AddEffect(T* effect)
	{
		assert(effect != nullptr);
		if (mEffects.count(&typeid(T)) == 0)
		{
			effect->SetParent(this);
			mEffects[&typeid(T)] = effect;
		}
		else
		{
			delete effect;
		}
	}

	template<typename T> void RemoveEffect()
	{
		if (HasEffect<T>())
		{
			T* effect = static_cast<T*>(mEffects[&typeid(T)]);
			mEffects.erase(&typeid(T));
			delete effect;
		}
	}

	template<typename T> bool HasEffect()
	{
		return mEffects.count(&typeid(T)) != 0;
	}

	template<typename T> T* GetEffect()
	{
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

