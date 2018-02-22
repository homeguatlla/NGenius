#include "stdafx.h"
#include "WaterShader.h"
#include "../camera/ICamera.h"
#include "../Transformation.h"
#include "../textures/ITexture.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectFogProperties.h"
#include "../materials/effects/MaterialEffectLightProperties.h"
#include "../materials/effects/MaterialEffectWater.h"

const std::string WaterShader::VERTEX_FILE = "data/shaders/vertex/v_water.cg";
const std::string WaterShader::FRAGMENT_FILE = "data/shaders/fragment/f_water.cg";

const std::string ATTRIBUTE_MODEL_MATRIX("M");
const std::string ATTRIBUTE_VIEW_MATRIX("V");
const std::string ATTRIBUTE_PROJECTION_MATRIX("P");
const std::string ATTRIBUTE_TEXTURE_COORDS("textureCoordsModelspace");
const std::string ATTRIBUTE_REFLECTION_TEXTURE("reflectionTexture");
const std::string ATTRIBUTE_REFRACTION_TEXTURE("refractionTexture");
const std::string ATTRIBUTE_DISTORSION_TEXTURE("distorsionTexture");
const std::string ATTRIBUTE_NORMAL_TEXTURE("normalTexture");
const std::string ATTRIBUTE_DEPTH_TEXTURE("depthTexture");
const std::string ATTRIBUTE_WATER_SPEED("waterSpeed");
const std::string ATTRIBUTE_WATER_COLOR("waterColor");
const std::string ATTRIBUTE_CAMERA_POSITION("cameraPosition");
const std::string ATTRIBUTE_LIGHT_POSITION("lightPosition");
const std::string ATTRIBUTE_LIGHT_COLOR("lightColor");
const std::string ATTRIBUTE_FOG_DENSITY("fogDensity");
const std::string ATTRIBUTE_FOG_GRADIENT("fogGradient");
const std::string ATTRIBUTE_FOG_COLOR("fogColor");

WaterShader::WaterShader() :
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationModelMatrix(-1),
mLocationViewMatrix(-1),
mLocationProjectionMatrix(-1),
mLocationReflectionTexture(-1),
mLocationRefractionTexture(-1),
mLocationDistorsionTexture(-1),
mLocationNormalTexture(-1),
mLocationDepthTexture(-1),
mLocationTextureCoords(-1),
mLocationWaterSpeed(-1),
mLocationWaterColor(-1),
mLocationCameraPosition(-1),
mLocationLightPosition(-1),
mLocationLightColor(-1),
mLocationFogDensity(-1),
mLocationFogGradient(-1),
mLocationFogColor(-1)
{
}


WaterShader::~WaterShader()
{
}

void WaterShader::BindAttributes()
{
	BindAttribute(mLocationTextureCoords, ATTRIBUTE_TEXTURE_COORDS);
	BindAttribute(mLocationModelMatrix, ATTRIBUTE_MODEL_MATRIX);
}

void WaterShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	LoadMatrix4(mLocationViewMatrix, const_cast<ICamera*>(camera)->GetViewMatrix());
	LoadMatrix4(mLocationProjectionMatrix, camera->GetProjectionMatrix());
	LoadVector3(mLocationCameraPosition, camera->GetPosition());

	if (material->HasEffect<MaterialEffectWater>())
	{
		MaterialEffectWater* effect = material->GetEffect<MaterialEffectWater>();
		LoadTexture(mLocationReflectionTexture, effect->GetReflectionTexture()->GetUnit());
		LoadTexture(mLocationRefractionTexture, effect->GetRefractionTexture()->GetUnit());
		LoadTexture(mLocationDistorsionTexture, effect->GetDistorsionTexture()->GetUnit());
		LoadTexture(mLocationNormalTexture, effect->GetNormalTexture()->GetUnit());
		LoadTexture(mLocationDepthTexture, effect->GetDepthTexture()->GetUnit());
		LoadFloat(mLocationWaterSpeed, effect->GetSpeed());
		LoadVector4(mLocationWaterColor, effect->GetColor());
	}

	if (material->HasEffect<MaterialEffectLightProperties>())
	{
		MaterialEffectLightProperties* effect = material->GetEffect<MaterialEffectLightProperties>();
		LoadVector3(mLocationLightPosition, effect->GetPosition());
		LoadVector3(mLocationLightColor, effect->GetColor());
	}

	if (material->HasEffect<MaterialEffectFogProperties>())
	{
		MaterialEffectFogProperties* effect = material->GetEffect<MaterialEffectFogProperties>();
		LoadVector3(mLocationFogColor, effect->GetColor());
		LoadFloat(mLocationFogDensity, effect->GetDensity());
		LoadFloat(mLocationFogGradient, effect->GetGradient());
	}
}

void WaterShader::GetAllUniformLocations()
{
	mLocationTextureCoords = GetAttributeLocation(ATTRIBUTE_TEXTURE_COORDS);
	mLocationModelMatrix = GetAttributeLocation(ATTRIBUTE_MODEL_MATRIX);
	mLocationProjectionMatrix = GetUniformLocation(ATTRIBUTE_PROJECTION_MATRIX);
	mLocationViewMatrix = GetUniformLocation(ATTRIBUTE_VIEW_MATRIX);
	mLocationReflectionTexture = GetUniformLocation(ATTRIBUTE_REFLECTION_TEXTURE);
	mLocationRefractionTexture = GetUniformLocation(ATTRIBUTE_REFRACTION_TEXTURE);
	mLocationDistorsionTexture = GetUniformLocation(ATTRIBUTE_DISTORSION_TEXTURE);
	mLocationNormalTexture = GetUniformLocation(ATTRIBUTE_NORMAL_TEXTURE);
	mLocationDepthTexture = GetUniformLocation(ATTRIBUTE_DEPTH_TEXTURE);
	mLocationWaterSpeed = GetUniformLocation(ATTRIBUTE_WATER_SPEED);
	mLocationWaterColor = GetUniformLocation(ATTRIBUTE_WATER_COLOR);
	mLocationCameraPosition = GetUniformLocation(ATTRIBUTE_CAMERA_POSITION);
	mLocationLightPosition = GetUniformLocation(ATTRIBUTE_LIGHT_POSITION);
	mLocationLightColor = GetUniformLocation(ATTRIBUTE_LIGHT_COLOR);

	mLocationFogDensity = GetUniformLocation(ATTRIBUTE_FOG_DENSITY);
	mLocationFogGradient = GetUniformLocation(ATTRIBUTE_FOG_GRADIENT);
	mLocationFogColor = GetUniformLocation(ATTRIBUTE_FOG_COLOR);
}