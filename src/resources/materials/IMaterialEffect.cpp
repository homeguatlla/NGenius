#include "stdafx.h"
#include "IMaterialEffect.h"

#include <assert.h>
#include <typeinfo>

IMaterialEffect::IMaterialEffect()
{
}


IMaterialEffect::~IMaterialEffect()
{
}

void IMaterialEffect::SetParent(IMaterial* parent)
{
	mParent = parent;
}

IMaterialEffect* IMaterialEffect::Clone() const
{
	IMaterialEffect* clone = DoClone();
	assert(typeid(*clone) == typeid(*this));

	return clone;
}
