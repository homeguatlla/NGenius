#include "stdafx.h"
#include "HorizontalBlurShader.h"
#include "../camera/ICamera.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../materials/effects/MaterialEffectFloat.h"
#include "../textures/ITexture.h"

const std::string HorizontalBlurShader::VERTEX_FILE = "data/shaders/vertex/v_horizontal_blur.cg";
const std::string HorizontalBlurShader::FRAGMENT_FILE = "data/shaders/fragment/f_blur.cg";

const std::string ATTRIBUTE_TEXTURE("texture");
const std::string ATTRIBUTE_WIDTH("width");

HorizontalBlurShader::HorizontalBlurShader() :
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationTexture(-1),
mLocationWidth(-1)
{
}

void HorizontalBlurShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	MaterialEffectDiffuseTexture* effectDiffuse = material->GetEffect<MaterialEffectDiffuseTexture>();
	if (effectDiffuse != nullptr)
	{
		LoadTexture(mLocationTexture, effectDiffuse->GetDiffuseTexture()->GetUnit());
	}
	MaterialEffectFloat* effectFloat = material->GetEffect<MaterialEffectFloat>();
	if (effectFloat != nullptr)
	{
		LoadFloat(mLocationWidth, effectFloat->GetValue());
	}
}

void HorizontalBlurShader::BindAttributes()
{
}

void HorizontalBlurShader::GetAllUniformLocations()
{
	mLocationTexture = GetUniformLocation(ATTRIBUTE_TEXTURE);
	mLocationWidth = GetUniformLocation(ATTRIBUTE_WIDTH);
}

IShaderProgram* HorizontalBlurShader::CreateShader()
{
	return new HorizontalBlurShader();
}