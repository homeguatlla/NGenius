#include "stdafx.h"
#include "SkyBoxShader.h"

#include "../Transformation.h"
#include "../camera/ICamera.h"

#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectFogProperties.h"
#include "../materials/effects/MaterialEffectTextureCubemap.h"

#include "../textures/ITexture.h"
#include "../textures/TextureCubemap.h"

const std::string SkyBoxShader::VERTEX_FILE = "data/shaders/vertex/v_skybox.cg";
const std::string SkyBoxShader::FRAGMENT_FILE = "data/shaders/fragment/f_skybox.cg";

const std::string ATTRIBUTE_MODEL_MATRIX("M");
const std::string ATTRIBUTE_VIEW_MATRIX("V");
const std::string ATTRIBUTE_PROJECTION_MATRIX("P");
const std::string ATTRIBUTE_CUBEMAP_TEXTURE("cubeMap");
const std::string ATTRIBUTE_FOG_COLOR("fogColor");

SkyBoxShader::SkyBoxShader() :
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE), 
mLocationModelMatrix(-1),
mLocationViewMatrix(-1), 
mLocationProjectionMatrix(-1),
mLocationCubemapTexture(-1),
mLocationFogColor(-1)
{
}


SkyBoxShader::~SkyBoxShader()
{
}

void SkyBoxShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	LoadMatrix4(mLocationModelMatrix, const_cast<Transformation*>(transformation)->GetModelMatrix());
	LoadMatrix4(mLocationViewMatrix, const_cast<ICamera*>(camera)->GetViewMatrix());
	LoadMatrix4(mLocationProjectionMatrix, camera->GetProjectionMatrix());

	MaterialEffectTextureCubemap* effectCubemap = material->GetEffect<MaterialEffectTextureCubemap>();
	if (effectCubemap != nullptr)
	{
		LoadTexture(mLocationCubemapTexture, effectCubemap->GetCubemap()->GetUnit());
	}

	MaterialEffectFogProperties* effectFog = material->GetEffect<MaterialEffectFogProperties>();
	if (effectFog != nullptr)
	{
		LoadVector3(mLocationFogColor, effectFog->GetColor());
	}
}

void SkyBoxShader::BindAttributes()
{
	BindAttribute(mLocationModelMatrix, ATTRIBUTE_MODEL_MATRIX);
}

void SkyBoxShader::GetAllUniformLocations()
{
	mLocationModelMatrix = GetAttributeLocation(ATTRIBUTE_MODEL_MATRIX);

	mLocationProjectionMatrix = GetUniformLocation(ATTRIBUTE_PROJECTION_MATRIX);
	mLocationViewMatrix = GetUniformLocation(ATTRIBUTE_VIEW_MATRIX);
	mLocationCubemapTexture = GetUniformLocation(ATTRIBUTE_CUBEMAP_TEXTURE);
	mLocationFogColor = GetUniformLocation(ATTRIBUTE_FOG_COLOR);
}

void SkyBoxShader::LoadCubemapTexture(int unit)
{
	LoadTexture(mLocationCubemapTexture, unit);
}