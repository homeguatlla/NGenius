#include "stdafx.h"
#include "MaterialsLibrary.h"
#include "../shaders/ShadersLibrary.h"

#include "IMaterial.h"
#include "BasicMaterial.h"

#include <GL/glew.h>
#include <algorithm>

const char* MaterialsLibrary::OVERDRAW_MATERIAL_NAME = "overdraw";
const char* MaterialsLibrary::WIREFRAME_MATERIAL_NAME = "wireframe";

MaterialsLibrary::MaterialsLibrary(ShadersLibrary* shadersLibrary) :
	mShadersLibrary(shadersLibrary)
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
	CreateMaterial(OVERDRAW_MATERIAL_NAME, mShadersLibrary->GetElement(OVERDRAW_MATERIAL_NAME));
	CreateMaterial(WIREFRAME_MATERIAL_NAME, mShadersLibrary->GetElement("default"));
}