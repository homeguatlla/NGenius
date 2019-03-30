#include "stdafx.h"
#include "ShadowShader.h"
#include "../camera/ICamera.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../materials/effects/MaterialEffectMatrix4Array.h"
#include "../textures/ITexture.h"

const std::string ShadowShader::VERTEX_FILE = "data/shaders/vertex/v_shadow.cg";
const std::string ShadowShader::FRAGMENT_FILE = "data/shaders/fragment/f_shadow.cg";

const std::string ATTRIBUTE_MODEL_MATRIX("M");
const std::string ATTRIBUTE_VIEW_MATRIX("V");
const std::string ATTRIBUTE_PROJECTION_MATRIX("P");
const std::string ATTRIBUTE_TEXTURE_COORDS("textureCoordsModelspace");
const std::string ATTRIBUTE_TEXTURE("texture");
const std::string ATTRIBUTE_TILE("tile");
const std::string ATTRIBUTE_JOINT_TRANSFORM_MATRIXS("jointTransforms");
const std::string ATTRIBUTE_JOINT_TRANSFORM_SIZE("jointTransformsSize");

ShadowShader::ShadowShader() :
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationM(-1),
mLocationV(-1),
mLocationP(-1),
mLocationTextureCoords(-1),
mLocationTexture(-1),
mLocationTile(-1),
mLocationJointTransformMatrixs(-1),
mLocationJointTransformSize(-1)
{
}


ShadowShader::~ShadowShader()
{
}

void ShadowShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	LoadMatrix4(mLocationV, const_cast<ICamera*>(camera)->GetViewMatrix());
	LoadMatrix4(mLocationP, camera->GetProjectionMatrix());

	MaterialEffectDiffuseTexture* effectDiffuse = material->GetEffect<MaterialEffectDiffuseTexture>();
	if (effectDiffuse != nullptr)
	{
		LoadTexture(mLocationTexture, effectDiffuse->GetDiffuseTexture()->GetUnit());
		LoadFloat(mLocationTile, effectDiffuse->GetTile());
	}

	MaterialEffectMatrix4Array* effectMatrixArray = material->GetEffect<MaterialEffectMatrix4Array>();
	if (effectMatrixArray != nullptr)
	{
		std::vector<glm::mat4x4> values = effectMatrixArray->GetValues();
		LoadMatrix4Array(mLocationJointTransformMatrixs, values);
		LoadInteger(mLocationJointTransformSize, static_cast<int>(values.size()));
	}
	else
	{
		LoadInteger(mLocationJointTransformSize, 0);
	}
}

void ShadowShader::BindAttributes()
{
	BindAttribute(mLocationM, ATTRIBUTE_MODEL_MATRIX);
	BindAttribute(mLocationJointTransformMatrixs, ATTRIBUTE_JOINT_TRANSFORM_MATRIXS);
	BindAttribute(mLocationJointTransformSize, ATTRIBUTE_JOINT_TRANSFORM_SIZE);
}

void ShadowShader::GetAllUniformLocations()
{
	mLocationTextureCoords = GetAttributeLocation(ATTRIBUTE_TEXTURE_COORDS);
	mLocationP = GetUniformLocation(ATTRIBUTE_PROJECTION_MATRIX);
	mLocationV = GetUniformLocation(ATTRIBUTE_VIEW_MATRIX);
	mLocationTexture = GetUniformLocation(ATTRIBUTE_TEXTURE);
	mLocationTile = GetUniformLocation(ATTRIBUTE_TILE);
	mLocationJointTransformMatrixs = GetUniformLocation(ATTRIBUTE_JOINT_TRANSFORM_MATRIXS);
	mLocationJointTransformSize = GetUniformLocation(ATTRIBUTE_JOINT_TRANSFORM_SIZE);
}

IShaderProgram* ShadowShader::CreateShader()
{
	return new ShadowShader();
}