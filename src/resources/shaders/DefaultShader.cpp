#include "stdafx.h"
#include "DefaultShader.h"
#include "../camera/ICamera.h"

const std::string DefaultShader::VERTEX_FILE = "data/shaders/vertex/v_default.cg";
const std::string DefaultShader::FRAGMENT_FILE = "data/shaders/fragment/f_default.cg";

const std::string ATTRIBUTE_MODEL_MATRIX("M");
const std::string ATTRIBUTE_VIEW_MATRIX("V");
const std::string ATTRIBUTE_PROJECTION_MATRIX("P");

DefaultShader::DefaultShader() :
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationM(-1),
mLocationV(-1),
mLocationP(-1)
{
}


DefaultShader::~DefaultShader()
{
}

void DefaultShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	LoadMatrix4(mLocationV, const_cast<ICamera*>(camera)->GetViewMatrix());
	LoadMatrix4(mLocationP, camera->GetProjectionMatrix());
}

void DefaultShader::BindAttributes()
{
	BindAttribute(mLocationM, ATTRIBUTE_MODEL_MATRIX);
}

void DefaultShader::GetAllUniformLocations()
{
	mLocationP = GetUniformLocation(ATTRIBUTE_PROJECTION_MATRIX);
	mLocationV = GetUniformLocation(ATTRIBUTE_VIEW_MATRIX);
}