#include "stdafx.h"
#include "GrassShader.h"
#include "../Transformation.h"
#include "../camera/ICamera.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDiffuseTexture.h"
#include "../materials/effects/MaterialEffectFogProperties.h"
#include "../materials/effects/MaterialEffectFloat2.h"
#include "../textures/ITexture.h"

const std::string GrassShader::VERTEX_FILE = "data/shaders/vertex/v_grass.cg";
const std::string GrassShader::FRAGMENT_FILE = "data/shaders/fragment/f_grass.cg";
const std::string GrassShader::GEOMETRY_FILE = "data/shaders/geometry/g_grass.cg";

const std::string ATTRIBUTE_VIEW_MATRIX("V");
const std::string ATTRIBUTE_PROJECTION_MATRIX("P");
const std::string ATTRIBUTE_MODEL_MATRIX("M");

const std::string ATTRIBUTE_TILE("tile");
const std::string ATTRIBUTE_TEXTURE("texture");

const std::string ATTRIBUTE_FOG_DENSITY("fogDensity");
const std::string ATTRIBUTE_FOG_GRADIENT("fogGradient");
const std::string ATTRIBUTE_FOG_COLOR("fogColor");

const std::string ATTRIBUTE_SIZE("size");

GrassShader::GrassShader() :
	IShaderProgram(VERTEX_FILE, FRAGMENT_FILE, GEOMETRY_FILE),
	mLocationModelMatrix(-1),
	mLocationViewMatrix(-1),
	mLocationProjectionMatrix(-1),
	mLocationTexture(-1),
	mLocationTile(-1),
	mLocationFogDensity(-1),
	mLocationFogGradient(-1),
	mLocationFogColor(-1),
	mLocationSize(-1)
{
}


GrassShader::~GrassShader()
{
}

void GrassShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	LoadMatrix4(mLocationModelMatrix, const_cast<Transformation*>(transformation)->GetModelMatrix());
	LoadMatrix4(mLocationViewMatrix, const_cast<ICamera*>(camera)->GetViewMatrix());
	LoadMatrix4(mLocationProjectionMatrix, camera->GetProjectionMatrix());

	MaterialEffectDiffuseTexture* effectDiffuse = material->GetEffect<MaterialEffectDiffuseTexture>();
	if (effectDiffuse != nullptr)
	{
		LoadTexture(mLocationTexture, effectDiffuse->GetDiffuseTexture()->GetUnit());
		LoadFloat(mLocationTile, effectDiffuse->GetTile());
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
}

void GrassShader::BindAttributes()
{
	BindAttribute(mLocationModelMatrix, ATTRIBUTE_MODEL_MATRIX);
}

void GrassShader::GetAllUniformLocations()
{
	mLocationProjectionMatrix = GetUniformLocation(ATTRIBUTE_PROJECTION_MATRIX);
	mLocationViewMatrix = GetUniformLocation(ATTRIBUTE_VIEW_MATRIX);
	mLocationModelMatrix = GetAttributeLocation(ATTRIBUTE_MODEL_MATRIX);

	mLocationTexture = GetUniformLocation(ATTRIBUTE_TEXTURE);
	mLocationTile = GetUniformLocation(ATTRIBUTE_TILE);

	mLocationFogDensity = GetUniformLocation(ATTRIBUTE_FOG_DENSITY);
	mLocationFogGradient = GetUniformLocation(ATTRIBUTE_FOG_GRADIENT);
	mLocationFogColor = GetUniformLocation(ATTRIBUTE_FOG_COLOR);

	mLocationSize = GetUniformLocation(ATTRIBUTE_SIZE);
}