#include "stdafx.h"
#include "EnergyWallShader.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDepthTexture.h"
#include "../materials/effects/MaterialEffectFloat3.h"
#include "../materials/effects/MaterialEffectFloat2.h"
#include "../textures/ITexture.h"

const std::string EnergyWallShader::VERTEX_FILE = "data/shaders/vertex/v_energy_wall.cg";
const std::string EnergyWallShader::FRAGMENT_FILE = "data/shaders/fragment/f_energy_wall.cg";

const std::string ATTRIBUTE_DEPTH_TEXTURE("depthTexture");
const std::string ATTRIBUTE_CONTACT_POINT("contactPoint");
const std::string ATTRIBUTE_SCREEN_SIZE("screenSize");

EnergyWallShader::EnergyWallShader() : 
ModelShader(EnergyWallShader::VERTEX_FILE, EnergyWallShader::FRAGMENT_FILE),
mLocationDepthTexture(-1),
mLocationContactPoint(-1),
mLocationScreenSize(-1)
{
}


EnergyWallShader::~EnergyWallShader()
{
}

void EnergyWallShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	ModelShader::LoadData(camera, transformation, material);

	if (material->HasEffect<MaterialEffectDepthTexture>())
	{
		MaterialEffectDepthTexture* effect = material->GetEffect<MaterialEffectDepthTexture>();
		LoadTexture(mLocationDepthTexture, effect->GetDepthTexture()->GetUnit());
	}

	if (material->HasEffect<MaterialEffectFloat2>())
	{
		MaterialEffectFloat2* effect = material->GetEffect<MaterialEffectFloat2>();
		LoadVector2(mLocationScreenSize, effect->GetValue());
	}

	if (material->HasEffect<MaterialEffectFloat3>())
	{
		MaterialEffectFloat3* effect = material->GetEffect<MaterialEffectFloat3>();
		LoadVector3(mLocationContactPoint, effect->GetValue());
	}
}

void EnergyWallShader::GetAllUniformLocations()
{
	ModelShader::GetAllUniformLocations();
	mLocationDepthTexture = GetUniformLocation(ATTRIBUTE_DEPTH_TEXTURE);
	mLocationContactPoint = GetUniformLocation(ATTRIBUTE_CONTACT_POINT);
	mLocationScreenSize = GetUniformLocation(ATTRIBUTE_SCREEN_SIZE);
}