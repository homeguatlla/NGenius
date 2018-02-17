#include "stdafx.h"
#include "SkyBoxShader.h"

const std::string SkyBoxShader::VERTEX_FILE = "data/shaders/vertex/v_skybox.cg";
const std::string SkyBoxShader::FRAGMENT_FILE = "data/shaders/fragment/f_skybox.cg";

const std::string ATTRIBUTE_MODEL_MATRIX("M");
const std::string ATTRIBUTE_VIEW_MATRIX("V");
const std::string ATTRIBUTE_PROJECTION_MATRIX("P");
const std::string ATTRIBUTE_CUBEMAP_TEXTURE("cubeMap");
const std::string ATTRIBUTE_FOG_COLOR("fogColor");

SkyBoxShader::SkyBoxShader() :
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE), 
mLocationModelMatrix(-1),
mLocationViewMatrix(-1), 
mLocationProjectionMatrix(-1),
mLocationCubemapTexture(-1),
mLocationFogColor(-1)
{
}


SkyBoxShader::~SkyBoxShader()
{
}

void SkyBoxShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{

}

void SkyBoxShader::BindAttributes()
{

}

void SkyBoxShader::GetAllUniformLocations()
{
	mLocationModelMatrix = GetUniformLocation(ATTRIBUTE_MODEL_MATRIX);
	mLocationProjectionMatrix = GetUniformLocation(ATTRIBUTE_PROJECTION_MATRIX);
	mLocationViewMatrix = GetUniformLocation(ATTRIBUTE_VIEW_MATRIX);
	mLocationCubemapTexture = GetUniformLocation(ATTRIBUTE_CUBEMAP_TEXTURE);
	mLocationFogColor = GetUniformLocation(ATTRIBUTE_FOG_COLOR);
}

void SkyBoxShader::LoadModelMatrix(const glm::mat4& modelMatrix)
{
	LoadMatrix4(mLocationModelMatrix, modelMatrix);
}

void SkyBoxShader::LoadViewMatrix(const glm::mat4& viewMatrix)
{
	LoadMatrix4(mLocationViewMatrix, viewMatrix);
}

void SkyBoxShader::LoadProjectionMatrix(const glm::mat4& projectionMatrix)
{
	LoadMatrix4(mLocationProjectionMatrix, projectionMatrix);
}

void SkyBoxShader::LoadCubemapTexture(int unit)
{
	LoadTexture(mLocationCubemapTexture, unit);
}

void SkyBoxShader::LoadFogColor(const glm::vec3& fogColor)
{
	LoadVector3(mLocationFogColor, fogColor);
}