#pragma once
class IMaterial;

class IMaterialEffect
{
protected:
	IMaterial * mParent;

public:
	IMaterialEffect();
	virtual ~IMaterialEffect();

	void SetParent(IMaterial* parent);
	IMaterialEffect* Clone() const;

	virtual void CopyValuesFrom(IMaterial* material) = 0;
protected:
	virtual IMaterialEffect* DoClone() const = 0;
};

