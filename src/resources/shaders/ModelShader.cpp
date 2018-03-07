#include "stdafx.h"
#include "ModelShader.h"
#include "../camera/ICamera.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../materials/effects/MaterialEffectLightProperties.h"
#include "../materials/effects/MaterialEffectFogProperties.h"
#include "../materials/effects/MaterialEffectShadowProperties.h"
#include "../textures/ITexture.h"

const std::string ModelShader::VERTEX_FILE = "data/shaders/vertex/v_model.cg";
const std::string ModelShader::FRAGMENT_FILE = "data/shaders/fragment/f_model.cg";

const std::string ATTRIBUTE_CAMERA_POSITION("cameraPosition");
const std::string ATTRIBUTE_LIGHT_POSITION("lightPositionWorldspace");
const std::string ATTRIBUTE_LIGHT_COLOR("lightColor");
const std::string ATTRIBUTE_MODEL_MATRIX("M");
const std::string ATTRIBUTE_VIEW_MATRIX("V");
const std::string ATTRIBUTE_PROJECTION_MATRIX("P");
const std::string ATTRIBUTE_TEXTURE_COORDS("textureCoordsModelspace");
const std::string ATTRIBUTE_VERTEX_NORMAL("normalModelspace");
const std::string ATTRIBUTE_TEXTURE("texture");
const std::string ATTRIBUTE_FOG_DENSITY("fogDensity");
const std::string ATTRIBUTE_FOG_GRADIENT("fogGradient");
const std::string ATTRIBUTE_FOG_COLOR("fogColor");
const std::string ATTRIBUTE_TILE("tile");
const std::string ATTRIBUTE_SHADOW_SPACE_MATRIX("toShadowMapSpace");
const std::string ATTRIBUTE_SHADOW_TEXTURE("shadowMap");
const std::string ATTRIBUTE_SHADOW_TEXTURE_WIDTH("shadowMapSize");
const std::string ATTRIBUTE_SHADOW_PFC("pfcCount");

ModelShader::ModelShader() :
ModelShader(VERTEX_FILE, FRAGMENT_FILE)
{
}

ModelShader::ModelShader(const std::string& vertexFile, const std::string& fragmentFile) :
IShaderProgram(vertexFile, fragmentFile),
mLocationModelMatrix(-1),
mLocationLightPosition(-1),
mLocationLightColor(-1),
mLocationCameraPosition(-1),
mLocationTextureCoords(-1),
mLocationNormal(-1),
mLocationTexture(-1),
mLocationFogDensity(-1),
mLocationFogGradient(-1),
mLocationFogColor(-1),
mLocationTile(-1),
mLocationShadowSpaceMatrix(-1),
mLocationShadowMapTexture(-1),
mLocationShadowMapTextureWidth(-1),
mLocationShadowMapPFC(-1)
{
}


ModelShader::~ModelShader()
{
}

void ModelShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	LoadMatrix4(mLocationViewMatrix, const_cast<ICamera*>(camera)->GetViewMatrix());
	LoadMatrix4(mLocationProjectionMatrix, camera->GetProjectionMatrix());
	LoadVector3(mLocationCameraPosition, camera->GetPosition());

	MaterialEffectDiffuseTexture* effectDiffuse = material->GetEffect<MaterialEffectDiffuseTexture>();
	if (effectDiffuse != nullptr)
	{
		LoadTexture(mLocationTexture, effectDiffuse->GetDiffuseTexture()->GetUnit());
		LoadFloat(mLocationTile, effectDiffuse->GetTile());
	}

	MaterialEffectLightProperties* effectLight = material->GetEffect<MaterialEffectLightProperties>();
	if (effectLight != nullptr)
	{
		LoadVector3(mLocationLightPosition, effectLight->GetPosition());
		LoadVector3(mLocationLightColor, effectLight->GetColor());
	}

	MaterialEffectFogProperties* effectFog = material->GetEffect<MaterialEffectFogProperties>();
	if (effectFog != nullptr)
	{
		LoadVector3(mLocationFogColor, effectFog->GetColor());
		LoadFloat(mLocationFogDensity, effectFog->GetDensity());
		LoadFloat(mLocationFogGradient, effectFog->GetGradient());
	}

	MaterialEffectShadowProperties* effectShadow = material->GetEffect<MaterialEffectShadowProperties>();
	if (effectShadow != nullptr && effectShadow->GetDepthTexture() != nullptr)
	{
		LoadMatrix4(mLocationShadowSpaceMatrix, effectShadow->GetMatrix());
		LoadTexture(mLocationShadowMapTexture, effectShadow->GetDepthTexture()->GetUnit());
		LoadInteger(mLocationShadowMapTextureWidth, effectShadow->GetDepthTexture()->GetWidth());
		LoadInteger(mLocationShadowMapPFC, effectShadow->GetPFCCounter());
	}
}

void ModelShader::BindAttributes()
{
	BindAttribute(mLocationTextureCoords, ATTRIBUTE_TEXTURE_COORDS);
	BindAttribute(mLocationNormal, ATTRIBUTE_VERTEX_NORMAL);
	BindAttribute(mLocationModelMatrix, ATTRIBUTE_MODEL_MATRIX);
}

void ModelShader::GetAllUniformLocations()
{
	mLocationTextureCoords = GetAttributeLocation(ATTRIBUTE_TEXTURE_COORDS);
	mLocationNormal = GetAttributeLocation(ATTRIBUTE_VERTEX_NORMAL);
	mLocationModelMatrix = GetAttributeLocation(ATTRIBUTE_MODEL_MATRIX);

	mLocationProjectionMatrix = GetUniformLocation(ATTRIBUTE_PROJECTION_MATRIX);
	mLocationViewMatrix = GetUniformLocation(ATTRIBUTE_VIEW_MATRIX);
	mLocationLightPosition = GetUniformLocation(ATTRIBUTE_LIGHT_POSITION);
	mLocationLightColor = GetUniformLocation(ATTRIBUTE_LIGHT_COLOR);
	mLocationCameraPosition = GetUniformLocation(ATTRIBUTE_CAMERA_POSITION);
	mLocationTexture = GetUniformLocation(ATTRIBUTE_TEXTURE);
	mLocationFogDensity = GetUniformLocation(ATTRIBUTE_FOG_DENSITY);
	mLocationFogGradient = GetUniformLocation(ATTRIBUTE_FOG_GRADIENT);
	mLocationFogColor = GetUniformLocation(ATTRIBUTE_FOG_COLOR);
	
	mLocationTile = GetUniformLocation(ATTRIBUTE_TILE);
	
	mLocationShadowSpaceMatrix = GetUniformLocation(ATTRIBUTE_SHADOW_SPACE_MATRIX);
	mLocationShadowMapTexture = GetUniformLocation(ATTRIBUTE_SHADOW_TEXTURE);
	mLocationShadowMapTextureWidth = GetUniformLocation(ATTRIBUTE_SHADOW_TEXTURE_WIDTH);
	mLocationShadowMapPFC = GetUniformLocation(ATTRIBUTE_SHADOW_PFC);
}