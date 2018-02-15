#include "stdafx.h"
#include "BasicMaterial.h"
#include "../shaders/IShaderProgram.h"
#include <GL/glew.h>

BasicMaterial::BasicMaterial(IShaderProgram* shader) :
IMaterial(shader)
{
}

BasicMaterial::~BasicMaterial()
{
}