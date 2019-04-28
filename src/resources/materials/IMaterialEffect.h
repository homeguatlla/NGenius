#pragma once
#include "../IFactory.h"
#include "../../utils/serializer/ISerializable.h"

class IMaterial;
class TexturesLibrary;

class IMaterialEffect : public core::utils::ISerializable, public IFactory
{
protected:
	IMaterial * mParent;

public:
	IMaterialEffect();
	virtual ~IMaterialEffect();

	void SetParent(IMaterial* parent);
	IMaterialEffect* Clone() const;

	virtual void Build(TexturesLibrary* texturesLibrary) {}
	virtual void CopyValuesFrom(IMaterial* material) = 0;
protected:
	virtual IMaterialEffect* DoClone() const = 0;
};

