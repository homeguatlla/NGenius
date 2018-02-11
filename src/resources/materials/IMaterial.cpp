#include "stdafx.h"
#include "IMaterial.h"
#include "../shaders/IShaderProgram.h"
#include <GL/glew.h>

unsigned IMaterial::IDCounter = 0;

IMaterial::IMaterial(IShaderProgram* shader, ITexture* diffuse, ITexture* normal) :
mMaterialID(0),
mShader(shader), 
mDiffuseTexture(diffuse), 
mNormalmapTexture(normal)
{
	mMaterialID = ++IDCounter;
}

IMaterial::~IMaterial()
{
}

unsigned int IMaterial::GetMaterialID() const
{
	return mMaterialID;
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