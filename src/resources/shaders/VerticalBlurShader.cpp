#include "stdafx.h"
#include "VerticalBlurShader.h"
#include "../camera/ICamera.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../materials/effects/MaterialEffectFloat.h"
#include "../textures/ITexture.h"

const std::string VerticalBlurShader::VERTEX_FILE = "data/shaders/vertex/v_vertical_blur.cg";
const std::string VerticalBlurShader::FRAGMENT_FILE = "data/shaders/fragment/f_blur.cg";

const std::string ATTRIBUTE_TEXTURE("texture");
const std::string ATTRIBUTE_HEIGHT("height");

VerticalBlurShader::VerticalBlurShader() :
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationTexture(-1),
mLocationHeight(-1)
{
}

void VerticalBlurShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	MaterialEffectDiffuseTexture* effectDiffuse = material->GetEffect<MaterialEffectDiffuseTexture>();
	if (effectDiffuse != nullptr)
	{
		LoadTexture(mLocationTexture, effectDiffuse->GetDiffuseTexture()->GetUnit());
	}
	MaterialEffectFloat* effectFloat = material->GetEffect<MaterialEffectFloat>();
	if (effectFloat != nullptr)
	{
		LoadFloat(mLocationHeight, effectFloat->GetValue());
	}
}

void VerticalBlurShader::BindAttributes()
{
}

void VerticalBlurShader::GetAllUniformLocations()
{
	mLocationTexture = GetUniformLocation(ATTRIBUTE_TEXTURE);
	mLocationHeight = GetUniformLocation(ATTRIBUTE_HEIGHT);
}

IShaderProgram* VerticalBlurShader::CreateShader()
{
	return new VerticalBlurShader();
}