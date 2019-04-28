#include "stdafx.h"
#include "DefaultShader.h"
#include "../camera/ICamera.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectFloat3.h"

const std::string DefaultShader::VERTEX_FILE = "data/shaders/vertex/v_default.cg";
const std::string DefaultShader::FRAGMENT_FILE = "data/shaders/fragment/f_default.cg";

const std::string ATTRIBUTE_MODEL_MATRIX("M");
const std::string ATTRIBUTE_VIEW_MATRIX("V");
const std::string ATTRIBUTE_PROJECTION_MATRIX("P");
const std::string ATTRIBUTE_COLOR("color3");

const glm::vec3 DEFAULT_COLOR(1.0f, 0.0f, 1.0f);

DefaultShader::DefaultShader() :
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationM(-1),
mLocationV(-1),
mLocationP(-1),
mLocationColor(-1)
{
}

void DefaultShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	LoadMatrix4(mLocationV, const_cast<ICamera*>(camera)->GetViewMatrix());
	LoadMatrix4(mLocationP, camera->GetProjectionMatrix());

	MaterialEffectFloat3* effectFloat3 = material->GetEffect<MaterialEffectFloat3>();
	if (effectFloat3 != nullptr)
	{
		LoadVector3(mLocationColor, effectFloat3->GetValue());
	}
	else
	{
		LoadVector3(mLocationColor, DEFAULT_COLOR);
	}
}

void DefaultShader::BindAttributes()
{
	BindAttribute(mLocationM, ATTRIBUTE_MODEL_MATRIX);
}

void DefaultShader::GetAllUniformLocations()
{
	mLocationP = GetUniformLocation(ATTRIBUTE_PROJECTION_MATRIX);
	mLocationV = GetUniformLocation(ATTRIBUTE_VIEW_MATRIX);
	mLocationColor = GetUniformLocation(ATTRIBUTE_COLOR);
}

IShaderProgram* DefaultShader::CreateShader()
{
	return new DefaultShader();
}