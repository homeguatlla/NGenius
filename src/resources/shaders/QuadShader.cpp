#include "stdafx.h"
#include "QuadShader.h"

const std::string QuadShader::VERTEX_FILE = "data/shaders/vertex/v_quad.cg";
const std::string QuadShader::FRAGMENT_FILE = "data/shaders/fragment/f_quad.cg";

const std::string ATTRIBUTE_MODEL_MATRIX("M");
const std::string ATTRIBUTE_VIEW_MATRIX("V");
const std::string ATTRIBUTE_PROJECTION_MATRIX("P");
const std::string ATTRIBUTE_TEXTURE_COORDS("textureCoordsModelspace");
const std::string ATTRIBUTE_TEXTURE("texture");

QuadShader::QuadShader() : 
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationModelMatrix(-1),
mLocationViewMatrix(-1),
mLocationProjectionMatrix(-1),
mLocationTexture(-1),
mLocationTextureCoords(-1)
{
}


QuadShader::~QuadShader()
{
}

void QuadShader::BindAttributes()
{
	BindAttribute(mLocationTextureCoords, ATTRIBUTE_TEXTURE_COORDS);
	BindAttribute(mLocationModelMatrix, ATTRIBUTE_MODEL_MATRIX);
}

void QuadShader::GetAllUniformLocations()
{
	mLocationTextureCoords = GetAttributeLocation(ATTRIBUTE_TEXTURE_COORDS);
	mLocationModelMatrix = GetAttributeLocation(ATTRIBUTE_MODEL_MATRIX);
	mLocationProjectionMatrix = GetUniformLocation(ATTRIBUTE_PROJECTION_MATRIX);
	mLocationViewMatrix = GetUniformLocation(ATTRIBUTE_VIEW_MATRIX);
	mLocationTexture = GetUniformLocation(ATTRIBUTE_TEXTURE);
}

void QuadShader::LoadViewMatrix(const glm::mat4& viewmatrix)
{
	LoadMatrix4(mLocationViewMatrix, viewmatrix);
}

void QuadShader::LoadProjectionMatrix(const glm::mat4& projectionMatrix)
{
	LoadMatrix4(mLocationProjectionMatrix, projectionMatrix);
}

void QuadShader::LoadQuadTexture(int unit)
{
	LoadTexture(mLocationTexture, unit);
}
