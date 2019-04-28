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
const std::string ATTRIBUTE_CUBEMAP1_TEXTURE("cubeMap1");
const std::string ATTRIBUTE_CUBEMAP2_TEXTURE("cubeMap2");
const std::string ATTRIBUTE_BLENDER_FACTOR("blendFactor");

SkyBoxShader::SkyBoxShader() :
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE), 
mLocationModelMatrix(-1),
mLocationViewMatrix(-1), 
mLocationProjectionMatrix(-1),
mLocationCubemap1Texture(-1),
mLocationCubemap2Texture(-1),
mLocationBlendFactor(-1)
{
}

void SkyBoxShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	IShaderProgram::LoadData(camera, transformation, material);

	LoadMatrix4(mLocationModelMatrix, const_cast<Transformation*>(transformation)->GetModelMatrix());
	LoadMatrix4(mLocationViewMatrix, const_cast<ICamera*>(camera)->GetViewMatrix());
	LoadMatrix4(mLocationProjectionMatrix, camera->GetProjectionMatrix());

	MaterialEffectTextureCubemap* effectCubemap = material->GetEffect<MaterialEffectTextureCubemap>();
	if (effectCubemap != nullptr)
	{
		LoadTexture(mLocationCubemap1Texture, effectCubemap->GetCubemap1()->GetUnit());
		LoadTexture(mLocationCubemap2Texture, effectCubemap->GetCubemap2()->GetUnit());
		LoadFloat(mLocationBlendFactor, effectCubemap->GetBlendFactor());
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
	mLocationCubemap1Texture = GetUniformLocation(ATTRIBUTE_CUBEMAP1_TEXTURE);
	mLocationCubemap2Texture = GetUniformLocation(ATTRIBUTE_CUBEMAP2_TEXTURE);
	mLocationBlendFactor = GetUniformLocation(ATTRIBUTE_BLENDER_FACTOR);
}

IShaderProgram* SkyBoxShader::CreateShader()
{
	return new SkyBoxShader();
}