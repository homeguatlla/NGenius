#include "stdafx.h"
#include "IMaterial.h"
#include "../shaders/IShaderProgram.h"
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

IMaterial* IMaterial::Clone() const
{
	IMaterial* clone = DoClone();

	assert(typeid(*clone) == typeid(*this));

	return clone;
}