#include "stdafx.h"
#include "IMaterial.h"
#include "../shaders/IShaderProgram.h"
#include "IMaterialEffect.h"
#include <GL/glew.h>

unsigned IMaterial::IDCounter = 0;

IMaterial::IMaterial(IShaderProgram* shader) :
mMaterialID(0),
mShader(shader)
{
	assert(shader != nullptr);
	mMaterialID = ++IDCounter;
}

IMaterial::~IMaterial()
{
	for (auto& pair : mEffects)
	{
		delete pair.second;
	}
	mEffects.clear();
}

unsigned int IMaterial::GetMaterialID() const
{
	return mMaterialID;
}

void IMaterial::Use()
{
	mShader->Use();
}

void IMaterial::UnUse()
{
	mShader->UnUse();
}

void IMaterial::Apply(const ICamera* camera, const Transformation* transformation)
{
	mShader->LoadData(camera, transformation, this);
}

IShaderProgram*  IMaterial::GetShader()
{
	return mShader;
}

void IMaterial::CopyMaterialEffectsValuesFrom(IMaterial* material)
{
	IMaterialEffectIterator it = mEffects.begin();

	for (; it != mEffects.end(); ++it)
	{
		IMaterialEffect* effect = it->second;
		effect->CopyValuesFrom(material);
	}
}

IMaterial* IMaterial::Clone() const
{
	IMaterial* clone = DoClone();

	assert(typeid(*clone) == typeid(*this));

	return clone;
}