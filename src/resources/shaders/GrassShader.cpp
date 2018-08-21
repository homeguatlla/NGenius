#include "stdafx.h"
#include "GrassShader.h"
#include "../Transformation.h"
#include "../camera/ICamera.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectNormalTexture.h"
#include "../materials/effects/MaterialEffectFogProperties.h"
#include "../materials/effects/MaterialEffectFloat.h"
#include "../materials/effects/MaterialEffectFloat2.h"
#include "../materials/effects/MaterialEffectShadowProperties.h"
#include "../materials/effects/MaterialEffectFloat3Array.h"
#include "../materials/effects/MaterialEffectParticle.h"
#include "../materials/effects/MaterialEffectClippingPlane.h"
#include "../textures/ITexture.h"



const std::string GrassShader::VERTEX_FILE = "data/shaders/vertex/v_grass.cg";
const std::string GrassShader::FRAGMENT_FILE = "data/shaders/fragment/f_grass.cg";
const std::string GrassShader::GEOMETRY_FILE = "data/shaders/geometry/g_grass.cg";

const std::string ATTRIBUTE_VIEW_MATRIX("V");
const std::string ATTRIBUTE_PROJECTION_MATRIX("P");

const std::string ATTRIBUTE_TILE("tile");
const std::string ATTRIBUTE_TEXTURE("texture");

const std::string ATTRIBUTE_FOG_DENSITY("fogDensity");
const std::string ATTRIBUTE_FOG_GRADIENT("fogGradient");
const std::string ATTRIBUTE_FOG_COLOR("fogColor");

const std::string ATTRIBUTE_SIZE("size");

const std::string ATTRIBUTE_SHADOW_SPACE_MATRIX("toShadowMapSpace");
const std::string ATTRIBUTE_SHADOW_TEXTURE("shadowMap");
const std::string ATTRIBUTE_SHADOW_TEXTURE_WIDTH("shadowMapSize");
const std::string ATTRIBUTE_SHADOW_PFC("pfcCount");

const std::string ATTRIBUTE_TIMER("timer");
const std::string ATTRIBUTE_TEXTURE_WIND("textureWind");

const std::string ATTRIBUTE_DEPTH_TEXTURE("depthTexture");
const std::string ATTRIBTUTE_SCREEN_SIZE("screenSize");

const std::string ATTRIBUTE_WIND_MODIFICATORS_ARRAY("windModificators");

GrassShader::GrassShader() :
	IShaderProgram(VERTEX_FILE, FRAGMENT_FILE, GEOMETRY_FILE),
	mLocationViewMatrix(-1),
	mLocationProjectionMatrix(-1),
	mLocationTexture(-1),
	mLocationTile(-1),
	mLocationFogDensity(-1),
	mLocationFogGradient(-1),
	mLocationFogColor(-1),
	mLocationSize(-1),
	mLocationShadowSpaceMatrix(-1),
	mLocationShadowMapTexture(-1),
	mLocationShadowMapTextureWidth(-1),
	mLocationShadowMapPFC(-1),
	mLocationTimer(-1),
	mLocationWindDirections(-1),
	mLocationDepthTexture(-1),
	mLocationScreenSize(-1),
	mLocationWindModificatorsArray(-1)
{
}


GrassShader::~GrassShader()
{
}

void GrassShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	IShaderProgram::LoadData(camera, transformation, material);

	LoadMatrix4(mLocationViewMatrix, const_cast<ICamera*>(camera)->GetViewMatrix());
	LoadMatrix4(mLocationProjectionMatrix, camera->GetProjectionMatrix());

	MaterialEffectNormalTexture* effectWindDirections = material->GetEffect<MaterialEffectNormalTexture>();
	if (effectWindDirections != nullptr)
	{
		LoadTexture(mLocationWindDirections, effectWindDirections->GetNormalTexture()->GetUnit());
	}

	MaterialEffectFogProperties* effectFog = material->GetEffect<MaterialEffectFogProperties>();
	if (effectFog != nullptr)
	{
		LoadVector3(mLocationFogColor, effectFog->GetColor());
		LoadFloat(mLocationFogDensity, effectFog->GetDensity());
		LoadFloat(mLocationFogGradient, effectFog->GetGradient());
	}

	MaterialEffectFloat2* effectFloat2 = material->GetEffect<MaterialEffectFloat2>();
	if (effectFloat2 != nullptr)
	{
		LoadVector2(mLocationSize, effectFloat2->GetValue());
	}

	MaterialEffectShadowProperties* effectShadow = material->GetEffect<MaterialEffectShadowProperties>();
	if (effectShadow != nullptr && effectShadow->GetDepthTexture() != nullptr)
	{
		LoadMatrix4(mLocationShadowSpaceMatrix, effectShadow->GetMatrix());
		LoadTexture(mLocationShadowMapTexture, effectShadow->GetDepthTexture()->GetUnit());
		LoadInteger(mLocationShadowMapTextureWidth, effectShadow->GetDepthTexture()->GetWidth());
		LoadInteger(mLocationShadowMapPFC, effectShadow->GetPFCCounter());
	}

	MaterialEffectFloat* effectTimer = material->GetEffect<MaterialEffectFloat>();
	if (effectTimer != nullptr)
	{
		LoadFloat(mLocationTimer, effectTimer->GetValue());
	}
	MaterialEffectParticle* effectParticle = material->GetEffect<MaterialEffectParticle>();
	if (effectParticle != nullptr)
	{
		LoadTexture(mLocationTexture, effectParticle->GetTexture()->GetUnit());
		LoadFloat(mLocationTile, effectParticle->GetTile());
		LoadTexture(mLocationDepthTexture, effectParticle->GetDepthTexture()->GetUnit());
		LoadVector2(mLocationScreenSize, effectParticle->GetScreenSize());
	}

	MaterialEffectFloat3Array* effectModificators = material->GetEffect<MaterialEffectFloat3Array>();
	if (effectModificators != nullptr)
	{
		LoadVector3Array(mLocationWindModificatorsArray, effectModificators->GetValues());
	}
}

void GrassShader::BindAttributes()
{

}

void GrassShader::GetAllUniformLocations()
{
	mLocationProjectionMatrix = GetUniformLocation(ATTRIBUTE_PROJECTION_MATRIX);
	mLocationViewMatrix = GetUniformLocation(ATTRIBUTE_VIEW_MATRIX);
	
	mLocationTexture = GetUniformLocation(ATTRIBUTE_TEXTURE);
	mLocationTile = GetUniformLocation(ATTRIBUTE_TILE);

	mLocationFogDensity = GetUniformLocation(ATTRIBUTE_FOG_DENSITY);
	mLocationFogGradient = GetUniformLocation(ATTRIBUTE_FOG_GRADIENT);
	mLocationFogColor = GetUniformLocation(ATTRIBUTE_FOG_COLOR);

	mLocationSize = GetUniformLocation(ATTRIBUTE_SIZE);

	mLocationShadowSpaceMatrix = GetUniformLocation(ATTRIBUTE_SHADOW_SPACE_MATRIX);
	mLocationShadowMapTexture = GetUniformLocation(ATTRIBUTE_SHADOW_TEXTURE);
	mLocationShadowMapTextureWidth = GetUniformLocation(ATTRIBUTE_SHADOW_TEXTURE_WIDTH);
	mLocationShadowMapPFC = GetUniformLocation(ATTRIBUTE_SHADOW_PFC);

	mLocationTimer = GetUniformLocation(ATTRIBUTE_TIMER);
	mLocationWindDirections = GetUniformLocation(ATTRIBUTE_TEXTURE_WIND);

	mLocationDepthTexture = GetUniformLocation(ATTRIBUTE_DEPTH_TEXTURE);
	mLocationScreenSize = GetUniformLocation(ATTRIBTUTE_SCREEN_SIZE);

	mLocationWindModificatorsArray = GetUniformLocation(ATTRIBUTE_WIND_MODIFICATORS_ARRAY);
}