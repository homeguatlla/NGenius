#include "stdafx.h"
#include "WaterShader.h"

#include "../entities/Light.h"

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

void WaterShader::LoadData(const ICamera* camera, IMaterial* material)
{

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

void WaterShader::LoadViewMatrix(const glm::mat4& viewmatrix)
{
	LoadMatrix4(mLocationViewMatrix, viewmatrix);
}

void WaterShader::LoadProjectionMatrix(const glm::mat4& projectionMatrix)
{
	LoadMatrix4(mLocationProjectionMatrix, projectionMatrix);
}

void WaterShader::LoadReflectionTexture(int unit)
{
	LoadTexture(mLocationReflectionTexture, unit);
}

void WaterShader::LoadRefractionTexture(int unit)
{
	LoadTexture(mLocationRefractionTexture, unit);
}

void WaterShader::LoadDistorsionTexture(int unit)
{
	LoadTexture(mLocationDistorsionTexture, unit);
}

void WaterShader::LoadNormalTexture(int unit)
{
	LoadTexture(mLocationNormalTexture, unit);
}

void WaterShader::LoadDepthTexture(int unit)
{
	LoadTexture(mLocationDepthTexture, unit);
}

void WaterShader::LoadWaterSpeed(float speed)
{
	LoadFloat(mLocationWaterSpeed, speed);
}

void WaterShader::LoadWaterColor(const glm::vec4& color)
{
	LoadVector4(mLocationWaterColor, color);
}

void WaterShader::LoadCameraPosition(const glm::vec3& position)
{
	LoadVector3(mLocationCameraPosition, position);
}

void WaterShader::LoadLight(const Light* light)
{
	LoadVector3(mLocationLightPosition, light->GetPosition());
	LoadVector3(mLocationLightColor, light->GetColor());
}

void WaterShader::LoadFogParameters(const glm::vec3& color, float density, float gradient)
{
	LoadVector3(mLocationFogColor, color);
	LoadFloat(mLocationFogDensity, density);
	LoadFloat(mLocationFogGradient, gradient);
}