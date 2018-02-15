#include "stdafx.h"
#include "NormalMapShader.h"
#include "ModelShader.h"

const std::string NormalMapShader::VERTEX_FILE = "data/shaders/vertex/v_model_normalmap.cg";
const std::string NormalMapShader::FRAGMENT_FILE = "data/shaders/fragment/f_model_normalmap.cg";

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
const std::string ATTRIBUTE_TILING("tile");
const std::string ATTRIBUTE_VERTEX_TANGENT("tangentModelspace");
const std::string ATTRIBUTE_NORMALMAP_TEXTURE("normalmap");
const std::string ATTRIBUTE_SHADOW_SPACE_MATRIX("toShadowMapSpace");
const std::string ATTRIBUTE_SHADOW_TEXTURE("shadowMap");
const std::string ATTRIBUTE_SHADOW_TEXTURE_WIDTH("shadowMapSize");
const std::string ATTRIBUTE_SHADOW_PFC("pfcCount");

NormalMapShader::NormalMapShader() : 
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
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
mLocationTiling(-1),
mLocationTangent(-1),
mLocationNormalmapTexture(-1),
mLocationShadowSpaceMatrix(-1),
mLocationShadowMapTexture(-1),
mLocationShadowMapTextureWidth(-1),
mLocationShadowMapPFC(-1)
{
}


NormalMapShader::~NormalMapShader()
{
}

void NormalMapShader::LoadData(const ICamera* camera, IMaterial* material)
{

}

void NormalMapShader::BindAttributes()
{
	BindAttribute(mLocationTextureCoords, ATTRIBUTE_TEXTURE_COORDS);
	BindAttribute(mLocationNormal, ATTRIBUTE_VERTEX_NORMAL);
	BindAttribute(mLocationModelMatrix, ATTRIBUTE_MODEL_MATRIX);
	BindAttribute(mLocationTangent, ATTRIBUTE_VERTEX_TANGENT);
}

void NormalMapShader::GetAllUniformLocations()
{
	mLocationTextureCoords = GetAttributeLocation(ATTRIBUTE_TEXTURE_COORDS);
	mLocationNormal = GetAttributeLocation(ATTRIBUTE_VERTEX_NORMAL);
	mLocationModelMatrix = GetAttributeLocation(ATTRIBUTE_MODEL_MATRIX);

	mLocationProjectionMatrix = GetUniformLocation(ATTRIBUTE_PROJECTION_MATRIX);
	mLocationViewMatrix = GetUniformLocation(ATTRIBUTE_VIEW_MATRIX);
	mLocationLightPosition = GetUniformLocation(ATTRIBUTE_LIGHT_POSITION);
	mLocationCameraPosition = GetUniformLocation(ATTRIBUTE_CAMERA_POSITION);
	mLocationTexture = GetUniformLocation(ATTRIBUTE_TEXTURE);
	mLocationFogDensity = GetUniformLocation(ATTRIBUTE_FOG_DENSITY);
	mLocationFogGradient = GetUniformLocation(ATTRIBUTE_FOG_GRADIENT);
	mLocationFogColor = GetUniformLocation(ATTRIBUTE_FOG_COLOR);
	mLocationTiling = GetUniformLocation(ATTRIBUTE_TILING);

	mLocationTangent = GetAttributeLocation(ATTRIBUTE_VERTEX_TANGENT);
	mLocationNormalmapTexture = GetUniformLocation(ATTRIBUTE_NORMALMAP_TEXTURE);

	mLocationShadowSpaceMatrix = GetUniformLocation(ATTRIBUTE_SHADOW_SPACE_MATRIX);
	mLocationShadowMapTexture = GetUniformLocation(ATTRIBUTE_SHADOW_TEXTURE);
	mLocationShadowMapTextureWidth = GetUniformLocation(ATTRIBUTE_SHADOW_TEXTURE_WIDTH);
	mLocationShadowMapPFC = GetUniformLocation(ATTRIBUTE_SHADOW_PFC);
}
void NormalMapShader::LoadLight(const Light& light)
{
	LoadVector3(mLocationLightPosition, light.GetPosition());
	//LoadVector3(mLocationLightColor, light.GetColor());
}

void NormalMapShader::LoadViewMatrix(const glm::mat4& viewmatrix)
{
	LoadMatrix4(mLocationViewMatrix, viewmatrix);
}

void NormalMapShader::LoadProjectionMatrix(const glm::mat4& projectionMatrix)
{
	LoadMatrix4(mLocationProjectionMatrix, projectionMatrix);
}

void NormalMapShader::LoadCameraPosition(const glm::vec3& position)
{
	LoadVector3(mLocationCameraPosition, position);
}

void NormalMapShader::LoadModelTexture(int unit)
{
	LoadTexture(mLocationTexture, unit);
}

void NormalMapShader::LoadFogParameters(const glm::vec3& color, float density, float gradient)
{
	LoadVector3(mLocationFogColor, color);
	LoadFloat(mLocationFogDensity, density);
	LoadFloat(mLocationFogGradient, gradient);
}

void NormalMapShader::LoadTiling(float tile)
{
	LoadFloat(mLocationTiling, tile);
}
void NormalMapShader::LoadModelNormalmap(int unit)
{
	LoadTexture(mLocationNormalmapTexture, unit);
}

void NormalMapShader::LoadShadowMapSpaceMatrix(const glm::mat4& matrix)
{
	LoadMatrix4(mLocationShadowSpaceMatrix, matrix);
}

void NormalMapShader::LoadShadowMapTexture(int unit, int width)
{
	LoadTexture(mLocationShadowMapTexture, unit);
	LoadInteger(mLocationShadowMapTextureWidth, width);
}

void NormalMapShader::LoadShadowMapPFC(int pfcCounter)
{
	LoadInteger(mLocationShadowMapPFC, pfcCounter);
}