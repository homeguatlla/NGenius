#include "stdafx.h"
#include "TerrainShader.h"

const std::string TerrainShader::VERTEX_FILE = "data/shaders/vertex/v_test_terrain_1.cg";
const std::string TerrainShader::FRAGMENT_FILE = "data/shaders/fragment/f_test_terrain_1.cg";

const std::string ATTRIBUTE_CAMERA_POSITION("cameraPosition");
const std::string ATTRIBUTE_LIGHT_POSITION("lightPositionWorldspace");
const std::string ATTRIBUTE_LIGHT_COLOR("lightColor");
const std::string ATTRIBUTE_SCALE("scale");
const std::string ATTRIBUTE_MVP_MATRIX("MVP");
const std::string ATTRIBUTE_MODEL_MATRIX("M");
const std::string ATTRIBUTE_VIEW_MATRIX("V");
const std::string ATTRIBUTE_TEXTURE_COORDS("textureCoordsModelspace");
const std::string ATTRIBUTE_ARRAY_TEXTURE("textureArray");
const std::string ATTRIBUTE_HEIGHTMAP_TEXTURE("heightMap");
const std::string ATTRIBUTE_BLENDMAP_TEXTURE("blendMap");
const std::string ATTRIBUTE_TILE("tile");
const std::string ATTRIBUTE_FOG_DENSITY("fogDensity");
const std::string ATTRIBUTE_FOG_GRADIENT("fogGradient");
const std::string ATTRIBUTE_FOG_COLOR("fogColor");
const std::string ATTRIBUTE_CLIP_PLANE("clippingPlane");
const std::string ATTRIBUTE_SHADOW_SPACE_MATRIX("toShadowMapSpace");
const std::string ATTRIBUTE_SHADOW_TEXTURE("shadowMap");
const std::string ATTRIBUTE_SHADOW_TEXTURE_WIDTH("shadowMapSize");

TerrainShader::TerrainShader() : 
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationMVPMatrix(-1),
mLocationModelMatrix(-1),
mLocationLightPosition(-1),
mLocationLightColor(-1),
mLocationCameraPosition(-1),
mLocationScale(-1),
mLocationTextureCoords(-1),
mLocationHeightMapTexture(-1),
mLocationBlendMapTexture(-1),
mLocationArrayTexture(-1),
mLocationTile(-1),
mLocationFogDensity(-1),
mLocationFogGradient(-1),
mLocationFogColor(-1),
mLocationClippingPlane(-1),
mLocationShadowSpaceMatrix(-1),
mLocationShadowMapTexture(-1),
mLocationShadowMapTextureWidth(-1)
{
}


TerrainShader::~TerrainShader()
{
}


void TerrainShader::BindAttributes()
{
	BindAttribute(mLocationTextureCoords, ATTRIBUTE_TEXTURE_COORDS);
}

void TerrainShader::GetAllUniformLocations()
{
	mLocationTextureCoords = GetAttributeLocation(ATTRIBUTE_TEXTURE_COORDS);
	mLocationMVPMatrix = GetUniformLocation(ATTRIBUTE_MVP_MATRIX);
	mLocationModelMatrix = GetUniformLocation(ATTRIBUTE_MODEL_MATRIX);
	mLocationLightPosition = GetUniformLocation(ATTRIBUTE_LIGHT_POSITION);
	mLocationLightColor = GetUniformLocation(ATTRIBUTE_LIGHT_COLOR);
	mLocationCameraPosition = GetUniformLocation(ATTRIBUTE_CAMERA_POSITION);
	mLocationScale = GetUniformLocation(ATTRIBUTE_SCALE);
	mLocationArrayTexture = GetUniformLocation(ATTRIBUTE_ARRAY_TEXTURE);
	mLocationHeightMapTexture = GetUniformLocation(ATTRIBUTE_HEIGHTMAP_TEXTURE);
	mLocationBlendMapTexture = GetUniformLocation(ATTRIBUTE_BLENDMAP_TEXTURE);
	mLocationTile = GetUniformLocation(ATTRIBUTE_TILE);
	mLocationFogDensity = GetUniformLocation(ATTRIBUTE_FOG_DENSITY);
	mLocationFogGradient = GetUniformLocation(ATTRIBUTE_FOG_GRADIENT);
	mLocationFogColor = GetUniformLocation(ATTRIBUTE_FOG_COLOR);
	mLocationClippingPlane = GetUniformLocation(ATTRIBUTE_CLIP_PLANE);
	mLocationShadowSpaceMatrix = GetUniformLocation(ATTRIBUTE_SHADOW_SPACE_MATRIX);
	mLocationShadowMapTexture = GetUniformLocation(ATTRIBUTE_SHADOW_TEXTURE);
	mLocationShadowMapTextureWidth = GetUniformLocation(ATTRIBUTE_SHADOW_TEXTURE_WIDTH);
}

void TerrainShader::LoadLight(const Light& light)
{
	LoadVector3(mLocationLightPosition, light.GetPosition());
	LoadVector3(mLocationLightColor, light.GetColor());
}

void TerrainShader::LoadModelMatrix(const glm::mat4& modelmatrix)
{
	LoadMatrix4(mLocationModelMatrix, modelmatrix);
}

void TerrainShader::LoadCameraPosition(const glm::vec3& position)
{
	LoadVector3(mLocationCameraPosition, position);
}

void TerrainShader::LoadScale(float scale)
{
	LoadFloat(mLocationScale, scale);
}

void TerrainShader::LoadHeightMapTexture(int unit)
{
	LoadTexture(mLocationHeightMapTexture, unit);
}

void TerrainShader::LoadBlendMapTexture(int unit)
{
	LoadTexture(mLocationBlendMapTexture, unit);
}

void TerrainShader::LoadArrayTexture(int unit)
{
	LoadTexture(mLocationArrayTexture, unit);
}

void TerrainShader::LoadTile(float tile)
{
	LoadFloat(mLocationTile, tile);
}

void TerrainShader::LoadFogParameters(const glm::vec3& color, float density, float gradient)
{
	LoadFloat(mLocationFogDensity, density);
	LoadFloat(mLocationFogGradient, gradient);
	LoadVector3(mLocationFogColor, color);
}

void TerrainShader::LoadClippingPlane(const glm::vec4& plane)
{
	LoadVector4(mLocationClippingPlane, plane);
}

void TerrainShader::LoadShadowSpaceMatrix(const glm::mat4& matrix)
{
	LoadMatrix4(mLocationShadowSpaceMatrix, matrix);
}

void TerrainShader::LoadShadowMapTexture(int unit, int width)
{
	LoadTexture(mLocationShadowMapTexture, unit);
	LoadInteger(mLocationShadowMapTextureWidth, width);
}