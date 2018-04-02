#include "stdafx.h"
#include "ContrastShader.h"
#include "../camera/ICamera.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../textures/ITexture.h"

const std::string ContrastShader::VERTEX_FILE = "data/shaders/vertex/v_contrast.cg";
const std::string ContrastShader::FRAGMENT_FILE = "data/shaders/fragment/f_contrast.cg";

const std::string ATTRIBUTE_TEXTURE("texture");

ContrastShader::ContrastShader() :
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationTexture(-1)
{
}


ContrastShader::~ContrastShader()
{
}

void ContrastShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	MaterialEffectDiffuseTexture* effectDiffuse = material->GetEffect<MaterialEffectDiffuseTexture>();
	if (effectDiffuse != nullptr)
	{
		LoadTexture(mLocationTexture, effectDiffuse->GetDiffuseTexture()->GetUnit());
	}
}

void ContrastShader::BindAttributes()
{
}

void ContrastShader::GetAllUniformLocations()
{
	mLocationTexture = GetUniformLocation(ATTRIBUTE_TEXTURE);
}