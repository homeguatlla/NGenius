#include "stdafx.h"
#include "IMaterial.h"
#include <GL/glew.h>

IMaterial::IMaterial(IShaderProgram* shader, ITexture* diffuse, ITexture* normal) :
mShader(shader), mDiffuseTexture(diffuse), mNormalmapTexture(normal)
{
}

IMaterial::~IMaterial()
{
}