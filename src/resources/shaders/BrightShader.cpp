#include "stdafx.h"
#include "BrightShader.h"
#include "../camera/ICamera.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../materials/effects/MaterialEffectFloat3.h"
#include "../textures/ITexture.h"

const std::string BrightShader::VERTEX_FILE = "data/shaders/vertex/v_bright.cg";
const std::string BrightShader::FRAGMENT_FILE = "data/shaders/fragment/f_bright.cg";

const std::string ATTRIBUTE_TEXTURE("texture");
const std::string ATTRIBUTE_BRIGHT("bright");

BrightShader::BrightShader() :
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationTexture(-1),
mLocationBright(-1)
{
}


BrightShader::~BrightShader()
{
}

void BrightShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	MaterialEffectDiffuseTexture* effectDiffuse = material->GetEffect<MaterialEffectDiffuseTexture>();
	if (effectDiffuse != nullptr)
	{
		LoadTexture(mLocationTexture, effectDiffuse->GetDiffuseTexture()->GetUnit());
	}

	MaterialEffectFloat3* effectBright = material->GetEffect<MaterialEffectFloat3>();
	if (effectBright != nullptr)
	{
		LoadVector3(mLocationBright, effectBright->GetValue());
	}
}

void BrightShader::BindAttributes()
{
}

void BrightShader::GetAllUniformLocations()
{
	mLocationTexture = GetUniformLocation(ATTRIBUTE_TEXTURE);
	mLocationBright = GetUniformLocation(ATTRIBUTE_BRIGHT);
}