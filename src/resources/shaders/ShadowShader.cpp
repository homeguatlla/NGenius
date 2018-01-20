#include "stdafx.h"
#include "ShadowShader.h"

const std::string ShadowShader::VERTEX_FILE = "data/shaders/vertex/v_shadow.cg";
const std::string ShadowShader::FRAGMENT_FILE = "data/shaders/fragment/f_shadow.cg";

const std::string ATTRIBUTE_MVP_MATRIX("MVP");


ShadowShader::ShadowShader() :
mLocationMVP(-1),
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE)
{
}


ShadowShader::~ShadowShader()
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