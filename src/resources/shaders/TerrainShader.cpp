#include "stdafx.h"
#include "TerrainShader.h"
#include "../camera/ICamera.h"
#include "../Transformation.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/DiffuseTexture.h"
#include "../materials/effects/LightProperties.h"
#include "../materials/effects/FogProperties.h"
#include "../materials/effects/ShadowProperties.h"
#include "../materials/effects/HeightMapTexture.h"
#include "../materials/effects/TextureArrayMaterialEffect.h"
#include "../materials/effects/ClippingPlaneMaterialEffect.h"

#include "../textures/ITexture.h"
#include "../textures/TextureArray.h"

const std::string TerrainShader::VERTEX_FILE = "data/shaders/vertex/v_test_terrain_1.cg";
const std::string TerrainShader::FRAGMENT_FILE = "data/shaders/fragment/f_test_terrain_1.cg";

const std::string ATTRIBUTE_CAMERA_POSITION("cameraPosition");
const std::string ATTRIBUTE_LIGHT_POSITION("lightPositionWorldspace");
const std::string ATTRIBUTE_LIGHT_COLOR("lightColor");
const std::string ATTRIBUTE_SCALE("scale");
const std::string ATTRIBUTE_MVP_MATRIX("MVP");
const std::string ATTRIBUTE_MODEL_MATRIX("M");
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
const std::string ATTRIBUTE_SHADOW_PFC("pfcCount");

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
mLocationShadowMapTextureWidth(-1),
mLocationShadowMapPFC(-1)
{
}


TerrainShader::~TerrainShader()
{
}

void TerrainShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	glm::mat4 MVP = camera->GetProjectionMatrix() * 
					const_cast<ICamera*>(camera)->GetViewMatrix() * 
					const_cast<Transformation*>(transformation)->GetModelMatrix();
	LoadMatrix4(mLocationMVPMatrix, MVP);
	LoadMatrix4(mLocationModelMatrix, const_cast<Transformation*>(transformation)->GetModelMatrix());

	LoadVector3(mLocationCameraPosition, camera->GetPosition());

	LoadFloat(mLocationScale, 0.0f);//7.5f);

	if (material->HasEffect<DiffuseTexture>())
	{
		DiffuseTexture* effect = material->GetEffect<DiffuseTexture>();
		LoadTexture(mLocationBlendMapTexture, effect->GetDiffuseTexture()->GetUnit());
		LoadFloat(mLocationTile, effect->GetTile());
	}

	if (material->HasEffect<HeightMapTexture>())
	{
		HeightMapTexture* effect = material->GetEffect<HeightMapTexture>();
		LoadTexture(mLocationHeightMapTexture, effect->GetHeightMapTexture()->GetUnit());
	}

	if (material->HasEffect<TextureArrayMaterialEffect>())
	{
		TextureArrayMaterialEffect* effect = material->GetEffect<TextureArrayMaterialEffect>();
		LoadTexture(mLocationArrayTexture, effect->GetTextureArray()->GetUnit());
	}

	if (material->HasEffect<ClippingPlaneMaterialEffect>())
	{
		ClippingPlaneMaterialEffect* effect = material->GetEffect<ClippingPlaneMaterialEffect>();
		LoadVector4(mLocationClippingPlane, effect->GetClippingPlane());
	}

	if (material->HasEffect<LightProperties>())
	{
		LightProperties* effect = material->GetEffect<LightProperties>();
		LoadVector3(mLocationLightPosition, effect->GetPosition());
		LoadVector3(mLocationLightColor, effect->GetColor());
	}

	if (material->HasEffect<FogProperties>())
	{
		FogProperties* effect = material->GetEffect<FogProperties>();
		LoadVector3(mLocationFogColor, effect->GetColor());
		LoadFloat(mLocationFogDensity, effect->GetDensity());
		LoadFloat(mLocationFogGradient, effect->GetGradient());
	}

	if (material->HasEffect<ShadowProperties>())
	{
		ShadowProperties* effect = material->GetEffect<ShadowProperties>();
		LoadMatrix4(mLocationShadowSpaceMatrix, effect->GetMatrix());
		LoadTexture(mLocationShadowMapTexture, effect->GetDepthTexture()->GetUnit());
		LoadInteger(mLocationShadowMapTextureWidth, effect->GetDepthTexture()->GetWidth());
		LoadInteger(mLocationShadowMapPFC, effect->GetPFCCounter());
	}
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
	mLocationBlendMapTexture = GetUniformLocation(ATTRIBUTE_BLENDMAP_TEXTURE);
	mLocationArrayTexture = GetUniformLocation(ATTRIBUTE_ARRAY_TEXTURE);
	mLocationHeightMapTexture = GetUniformLocation(ATTRIBUTE_HEIGHTMAP_TEXTURE);
	mLocationTile = GetUniformLocation(ATTRIBUTE_TILE);
	mLocationScale = GetUniformLocation(ATTRIBUTE_SCALE);
	mLocationClippingPlane = GetUniformLocation(ATTRIBUTE_CLIP_PLANE);
	mLocationFogDensity = GetUniformLocation(ATTRIBUTE_FOG_DENSITY);
	mLocationFogGradient = GetUniformLocation(ATTRIBUTE_FOG_GRADIENT);
	mLocationFogColor = GetUniformLocation(ATTRIBUTE_FOG_COLOR);
	mLocationShadowSpaceMatrix = GetUniformLocation(ATTRIBUTE_SHADOW_SPACE_MATRIX);
	mLocationShadowMapTexture = GetUniformLocation(ATTRIBUTE_SHADOW_TEXTURE);
	mLocationShadowMapTextureWidth = GetUniformLocation(ATTRIBUTE_SHADOW_TEXTURE_WIDTH);
	mLocationShadowMapPFC = GetUniformLocation(ATTRIBUTE_SHADOW_PFC);
}