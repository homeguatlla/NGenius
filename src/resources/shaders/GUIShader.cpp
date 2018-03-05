#include "stdafx.h"
#include "GUIShader.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../textures/ITexture.h"
#include "../camera/ICamera.h"

const std::string GUIShader::VERTEX_FILE = "data/shaders/vertex/v_gui_quad.cg";
const std::string GUIShader::FRAGMENT_FILE = "data/shaders/fragment/f_gui_quad.cg";

const std::string ATTRIBUTE_MODEL_MATRIX("M");
const std::string ATTRIBUTE_VIEW_MATRIX("V");
const std::string ATTRIBUTE_PROJECTION_MATRIX("P");
const std::string ATTRIBUTE_TEXTURE_COORDS("textureCoordsModelspace");
const std::string ATTRIBUTE_TEXTURE("texture");

GUIShader::GUIShader() :
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationModelMatrix(-1),
mLocationViewMatrix(-1),
mLocationProjectionMatrix(-1),
mLocationTexture(-1),
mLocationTextureCoords(-1)
{
}


GUIShader::~GUIShader()
{
}

void GUIShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	LoadMatrix4(mLocationViewMatrix, const_cast<ICamera*>(camera)->GetViewMatrix());
	LoadMatrix4(mLocationProjectionMatrix, camera->GetProjectionMatrix());
	
	MaterialEffectDiffuseTexture* effect = material->GetEffect<MaterialEffectDiffuseTexture>();
	if (effect != nullptr)
	{
		LoadTexture(mLocationTexture, effect->GetDiffuseTexture()->GetUnit());
	}
}

void GUIShader::BindAttributes()
{
	BindAttribute(mLocationTextureCoords, ATTRIBUTE_TEXTURE_COORDS);
	BindAttribute(mLocationModelMatrix, ATTRIBUTE_MODEL_MATRIX);
}

void GUIShader::GetAllUniformLocations()
{
	mLocationTextureCoords = GetAttributeLocation(ATTRIBUTE_TEXTURE_COORDS);
	mLocationModelMatrix = GetAttributeLocation(ATTRIBUTE_MODEL_MATRIX);
	mLocationProjectionMatrix = GetUniformLocation(ATTRIBUTE_PROJECTION_MATRIX);
	mLocationViewMatrix = GetUniformLocation(ATTRIBUTE_VIEW_MATRIX);
	mLocationTexture = GetUniformLocation(ATTRIBUTE_TEXTURE);
}