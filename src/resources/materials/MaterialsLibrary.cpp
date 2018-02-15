#include "stdafx.h"
#include "MaterialsLibrary.h"

#include "IMaterial.h"
#include "BasicMaterial.h"

#include <GL/glew.h>

#include <algorithm>

MaterialsLibrary::MaterialsLibrary()
{
}

MaterialsLibrary::~MaterialsLibrary()
{

}

void MaterialsLibrary::CreateMaterial(const std::string& name, IShaderProgram* shader, ITexture* diffuseTexture, ITexture* normalTexture)
{
	assert(shader != nullptr);

	IMaterial* material = new BasicMaterial(shader);
	AddElement(name, material);
}

void MaterialsLibrary::Load()
{

}