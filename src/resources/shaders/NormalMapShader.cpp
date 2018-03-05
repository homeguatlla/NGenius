#include "stdafx.h"
#include "NormalMapShader.h"
#include "ModelShader.h"
#include "../textures/ITexture.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectNormalTexture.h"

const std::string NormalMapShader::VERTEX_FILE = "data/shaders/vertex/v_model_normalmap.cg";
const std::string NormalMapShader::FRAGMENT_FILE = "data/shaders/fragment/f_model_normalmap.cg";

const std::string ATTRIBUTE_VERTEX_TANGENT("tangentModelspace");
const std::string ATTRIBUTE_NORMALMAP_TEXTURE("normalmap");


NormalMapShader::NormalMapShader() : 
ModelShader(VERTEX_FILE, FRAGMENT_FILE),
mLocationTangent(-1),
mLocationNormalmapTexture(-1)
{
}


NormalMapShader::~NormalMapShader()
{
}

void NormalMapShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	ModelShader::LoadData(camera, transformation, material);

	MaterialEffectNormalTexture* effect = material->GetEffect<MaterialEffectNormalTexture>();
	if (effect != nullptr)
	{
		LoadTexture(mLocationNormalmapTexture, effect->GetNormalTexture()->GetUnit());
		//LoadFloat(mLocationTile, effect->GetTile());
	}
}

void NormalMapShader::BindAttributes()
{
	ModelShader::BindAttributes();
	BindAttribute(mLocationTangent, ATTRIBUTE_VERTEX_TANGENT);
}

void NormalMapShader::GetAllUniformLocations()
{
	ModelShader::GetAllUniformLocations();

	mLocationTangent = GetAttributeLocation(ATTRIBUTE_VERTEX_TANGENT);
	mLocationNormalmapTexture = GetUniformLocation(ATTRIBUTE_NORMALMAP_TEXTURE);
}