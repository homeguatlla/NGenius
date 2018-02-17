#include "stdafx.h"
#include "ShadowShader.h"

const std::string ShadowShader::VERTEX_FILE = "data/shaders/vertex/v_shadow.cg";
const std::string ShadowShader::FRAGMENT_FILE = "data/shaders/fragment/f_shadow.cg";

const std::string ATTRIBUTE_MVP_MATRIX("MVP");


ShadowShader::ShadowShader() :
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationMVP(-1)
{
}


ShadowShader::~ShadowShader()
{
}

void ShadowShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{

}

void ShadowShader::BindAttributes()
{
}

void ShadowShader::GetAllUniformLocations()
{
	mLocationMVP = GetUniformLocation(ATTRIBUTE_MVP_MATRIX);
}

void ShadowShader::LoadMVPMatrix(const glm::mat4& matrix)
{
	LoadMatrix4(mLocationMVP, matrix);
}