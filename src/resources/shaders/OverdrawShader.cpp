#include "stdafx.h"
#include "OverdrawShader.h"
#include "../camera/ICamera.h"

const std::string OverdrawShader::VERTEX_FILE = "data/shaders/vertex/v_overdraw.cg";
const std::string OverdrawShader::FRAGMENT_FILE = "data/shaders/fragment/f_overdraw.cg";

const std::string ATTRIBUTE_MODEL_MATRIX("M");
const std::string ATTRIBUTE_VIEW_MATRIX("V");
const std::string ATTRIBUTE_PROJECTION_MATRIX("P");

OverdrawShader::OverdrawShader() :
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationM(-1),
mLocationV(-1),
mLocationP(-1)
{
}


OverdrawShader::~OverdrawShader()
{
}

void OverdrawShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	LoadMatrix4(mLocationV, const_cast<ICamera*>(camera)->GetViewMatrix());
	LoadMatrix4(mLocationP, camera->GetProjectionMatrix());
}

void OverdrawShader::BindAttributes()
{
	BindAttribute(mLocationM, ATTRIBUTE_MODEL_MATRIX);
}

void OverdrawShader::GetAllUniformLocations()
{
	mLocationM = GetAttributeLocation(ATTRIBUTE_MODEL_MATRIX);
	mLocationP = GetUniformLocation(ATTRIBUTE_PROJECTION_MATRIX);
	mLocationV = GetUniformLocation(ATTRIBUTE_VIEW_MATRIX);
}