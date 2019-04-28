#include "stdafx.h"
#include "ContrastShader.h"
#include "../camera/ICamera.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../materials/effects/MaterialEffectFloat.h"
#include "../textures/ITexture.h"

const std::string ContrastShader::VERTEX_FILE = "data/shaders/vertex/v_contrast.cg";
const std::string ContrastShader::FRAGMENT_FILE = "data/shaders/fragment/f_contrast.cg";

const std::string ATTRIBUTE_TEXTURE("texture");
const std::string ATTRIBUTE_CONTRAST("contrast");

ContrastShader::ContrastShader() :
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationTexture(-1),
mLocationContrast(-1)
{
}

void ContrastShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	MaterialEffectDiffuseTexture* effectDiffuse = material->GetEffect<MaterialEffectDiffuseTexture>();
	if (effectDiffuse != nullptr)
	{
		LoadTexture(mLocationTexture, effectDiffuse->GetDiffuseTexture()->GetUnit());
	}

	MaterialEffectFloat* effectFloat = material->GetEffect<MaterialEffectFloat>();
	if (effectFloat != nullptr)
	{
		LoadFloat(mLocationContrast, effectFloat->GetValue());
	}
}

void ContrastShader::BindAttributes()
{
}

void ContrastShader::GetAllUniformLocations()
{
	mLocationTexture = GetUniformLocation(ATTRIBUTE_TEXTURE);
	mLocationContrast = GetUniformLocation(ATTRIBUTE_CONTRAST);
}

IShaderProgram* ContrastShader::CreateShader()
{
	return new ContrastShader();
}