#include "stdafx.h"
#include "GUIShader.h"

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

void GUIShader::LoadViewMatrix(const glm::mat4& viewmatrix)
{
	LoadMatrix4(mLocationViewMatrix, viewmatrix);
}

void GUIShader::LoadProjectionMatrix(const glm::mat4& projectionMatrix)
{
	LoadMatrix4(mLocationProjectionMatrix, projectionMatrix);
}

void GUIShader::LoadGUITexture(int unit)
{
	LoadTexture(mLocationTexture, unit);
}
