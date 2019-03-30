#include "stdafx.h"
#include "TerrainShader.h"
#include "../camera/ICamera.h"
#include "../Transformation.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../materials/effects/MaterialEffectHeightMapTexture.h"
#include "../materials/effects/MaterialEffectTextureArray.h"
#include "../materials/effects/MaterialEffectFloat.h"

#include "../textures/ITexture.h"
#include "../textures/TextureArray.h"

const std::string TerrainShader::VERTEX_FILE = "data/shaders/vertex/v_test_terrain_1.cg";
const std::string TerrainShader::FRAGMENT_FILE = "data/shaders/fragment/f_test_terrain_1.cg";

const std::string ATTRIBUTE_CAMERA_POSITION("cameraPosition");
const std::string ATTRIBUTE_SCALE("scale");
const std::string ATTRIBUTE_MVP_MATRIX("MVP");
const std::string ATTRIBUTE_MODEL_MATRIX("M");
const std::string ATTRIBUTE_TEXTURE_COORDS("textureCoordsModelspace");
const std::string ATTRIBUTE_ARRAY_TEXTURE("textureArray");
const std::string ATTRIBUTE_HEIGHTMAP_TEXTURE("heightMap");
const std::string ATTRIBUTE_BLENDMAP_TEXTURE("blendMap");
const std::string ATTRIBUTE_TILE("tile");

TerrainShader::TerrainShader() : 
IShaderProgram(VERTEX_FILE, FRAGMENT_FILE),
mLocationMVPMatrix(-1),
mLocationModelMatrix(-1),
mLocationCameraPosition(-1),
mLocationScale(-1),
mLocationTextureCoords(-1),
mLocationHeightMapTexture(-1),
mLocationBlendMapTexture(-1),
mLocationArrayTexture(-1),
mLocationTile(-1)
{
}


TerrainShader::~TerrainShader()
{
}

void TerrainShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	IShaderProgram::LoadData(camera, transformation, material);

	LoadMatrix4(mLocationModelMatrix, const_cast<Transformation*>(transformation)->GetModelMatrix());
	glm::mat4 MVP = camera->GetProjectionMatrix() * 
					const_cast<ICamera*>(camera)->GetViewMatrix() * 
					const_cast<Transformation*>(transformation)->GetModelMatrix();
	LoadMatrix4(mLocationMVPMatrix, MVP);
	LoadVector3(mLocationCameraPosition, camera->GetPosition());

	MaterialEffectFloat* effect = material->GetEffect<MaterialEffectFloat>();
	if (effect != nullptr)
	{
		LoadFloat(mLocationScale, effect->GetValue());
	}

	MaterialEffectDiffuseTexture* effectDiffuse = material->GetEffect<MaterialEffectDiffuseTexture>();
	if (effectDiffuse != nullptr)
	{
		LoadTexture(mLocationBlendMapTexture, effectDiffuse->GetDiffuseTexture()->GetUnit());
		LoadFloat(mLocationTile, effectDiffuse->GetTile());
	}

	MaterialEffectHeightMapTexture* effectHeightMap = material->GetEffect<MaterialEffectHeightMapTexture>();
	if (effectHeightMap != nullptr)
	{
		LoadTexture(mLocationHeightMapTexture, effectHeightMap->GetHeightMapTexture()->GetUnit());
	}

	MaterialEffectTextureArray* effectTextureArray = material->GetEffect<MaterialEffectTextureArray>();
	if (effectTextureArray != nullptr)
	{
		LoadTexture(mLocationArrayTexture, effectTextureArray->GetTextureArray()->GetUnit());
	}
}

void TerrainShader::BindAttributes()
{
	BindAttribute(mLocationTextureCoords, ATTRIBUTE_TEXTURE_COORDS);
	BindAttribute(mLocationModelMatrix, ATTRIBUTE_MODEL_MATRIX);
}

void TerrainShader::GetAllUniformLocations()
{
	mLocationModelMatrix = GetAttributeLocation(ATTRIBUTE_MODEL_MATRIX); 
	mLocationTextureCoords = GetAttributeLocation(ATTRIBUTE_TEXTURE_COORDS);
	
	mLocationMVPMatrix = GetUniformLocation(ATTRIBUTE_MVP_MATRIX);
	mLocationCameraPosition = GetUniformLocation(ATTRIBUTE_CAMERA_POSITION);
	mLocationBlendMapTexture = GetUniformLocation(ATTRIBUTE_BLENDMAP_TEXTURE);
	mLocationArrayTexture = GetUniformLocation(ATTRIBUTE_ARRAY_TEXTURE);
	mLocationHeightMapTexture = GetUniformLocation(ATTRIBUTE_HEIGHTMAP_TEXTURE);
	mLocationTile = GetUniformLocation(ATTRIBUTE_TILE);
	mLocationScale = GetUniformLocation(ATTRIBUTE_SCALE);
}

IShaderProgram* TerrainShader::CreateShader()
{
	return new TerrainShader();
}