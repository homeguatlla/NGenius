#include "stdafx.h"
#include "BloomShader.h"
#include "../camera/ICamera.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../materials/effects/MaterialEffectDepthTexture.h"
#include "../textures/ITexture.h"

const std::string BloomShader::VERTEX_FILE = "data/shaders/vertex/v_bloom.cg";
const std::string BloomShader::FRAGMENT_FILE = "data/shaders/fragment/f_bloom.cg";

const std::string ATTRIBUTE_TEXTURE("texture");
const std::string ATTRIBUTE_BRIGHT_TEXTURE("original_texture");

BloomShader::BloomShader() :
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationTexture(-1),
mLocationBrightTexture(-1)
{
}

void BloomShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	MaterialEffectDiffuseTexture* effectBright = material->GetEffect<MaterialEffectDiffuseTexture>();
	if (effectBright != nullptr)
	{
		LoadTexture(mLocationBrightTexture, effectBright->GetDiffuseTexture()->GetUnit());
	}

	MaterialEffectDepthTexture* effectOriginalTexture = material->GetEffect<MaterialEffectDepthTexture>();
	if (effectOriginalTexture != nullptr)
	{
		LoadTexture(mLocationTexture, effectOriginalTexture->GetDepthTexture()->GetUnit());
	}
}

void BloomShader::BindAttributes()
{
}

void BloomShader::GetAllUniformLocations()
{
	mLocationTexture = GetUniformLocation(ATTRIBUTE_TEXTURE);
	mLocationBrightTexture = GetUniformLocation(ATTRIBUTE_BRIGHT_TEXTURE);
}