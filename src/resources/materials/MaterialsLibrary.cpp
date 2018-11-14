#include "stdafx.h"
#include "MaterialsLibrary.h"
#include "../shaders/ShadersLibrary.h"

#include "IMaterial.h"
#include "BasicMaterial.h"

#include <GL/glew.h>
#include <algorithm>

const char* MaterialsLibrary::OVERDRAW_MATERIAL_NAME = "overdraw";
const char* MaterialsLibrary::WIREFRAME_MATERIAL_NAME = "wireframe";
const char* MaterialsLibrary::MODEL_MATERIAL_NAME = "model";
const char* MaterialsLibrary::MODEL_NORMALMAP_MATERIAL_NAME = "normalmap";
const char* MaterialsLibrary::TEXT_MATERIAL_NAME = "text";
const char* MaterialsLibrary::TEXT3D_MATERIAL_NAME = "text3d";
const char* MaterialsLibrary::GUI_MATERIAL_NAME = "gui";

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
	CreateMaterial(MODEL_MATERIAL_NAME, mShadersLibrary->GetElement("model"));
	CreateMaterial(MODEL_NORMALMAP_MATERIAL_NAME, mShadersLibrary->GetElement("normalmap"));
	CreateMaterial(TEXT_MATERIAL_NAME, mShadersLibrary->GetElement("text"));
	CreateMaterial(TEXT3D_MATERIAL_NAME, mShadersLibrary->GetElement("text"));
	CreateMaterial(GUI_MATERIAL_NAME, mShadersLibrary->GetElement("gui"));

	//TODO aquí podríamos crear todos los materiales de los modelos siempre que tengan texturas distintas etc...
	//ojo que los modelos que comparten atlases nos lo complican
}