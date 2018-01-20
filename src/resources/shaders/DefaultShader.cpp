#include "stdafx.h"
#include "DefaultShader.h"

const std::string DefaultShader::VERTEX_FILE = "data/shaders/vertex/v_default.cg";
const std::string DefaultShader::FRAGMENT_FILE = "data/shaders/fragment/f_default.cg";

DefaultShader::DefaultShader() :
mLocationMVP(-1),
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE)
{
}


DefaultShader::~DefaultShader()
{
}


void DefaultShader::BindAttributes()
{
}

void DefaultShader::GetAllUniformLocations()
{
}