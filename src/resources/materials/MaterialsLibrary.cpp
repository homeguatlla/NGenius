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

IMaterial* MaterialsLibrary::CreateMaterial(const std::string& name, IShaderProgram* shader)
{
	assert(shader != nullptr);

	IMaterial* material = new BasicMaterial(shader);
	AddElement(name, material);

	return material;
}

void MaterialsLibrary::Load()
{
}