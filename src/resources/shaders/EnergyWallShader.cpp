#include "stdafx.h"
#include "EnergyWallShader.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectDepthTexture.h"
#include "../materials/effects/MaterialEffectFloat3.h"
#include "../materials/effects/MaterialEffectFloat2.h"
#include "../materials/effects/MaterialEffectFloat.h"
#include "../textures/ITexture.h"

const std::string EnergyWallShader::VERTEX_FILE = "data/shaders/vertex/v_energy_wall.cg";
const std::string EnergyWallShader::FRAGMENT_FILE = "data/shaders/fragment/f_energy_wall.cg";

const std::string ATTRIBUTE_DEPTH_TEXTURE("depthTexture");
const std::string ATTRIBUTE_CONTACT_POINT("contactPoint");
const std::string ATTRIBUTE_SCREEN_SIZE("screenSize");
const std::string ATTRIBUTE_TIMER("timer");

EnergyWallShader::EnergyWallShader() : 
ModelShader(EnergyWallShader::VERTEX_FILE, EnergyWallShader::FRAGMENT_FILE),
mLocationDepthTexture(-1),
mLocationContactPoint(-1),
mLocationScreenSize(-1),
mLocationTimer(-1)
{
}

void EnergyWallShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	ModelShader::LoadData(camera, transformation, material);

	MaterialEffectDepthTexture* effectDepth = material->GetEffect<MaterialEffectDepthTexture>();
	if (effectDepth != nullptr)
	{
		LoadTexture(mLocationDepthTexture, effectDepth->GetDepthTexture()->GetUnit());
	}

	MaterialEffectFloat* effectTimer = material->GetEffect<MaterialEffectFloat>();
	if (effectTimer != nullptr)
	{
		LoadFloat(mLocationTimer, effectTimer->GetValue());
	}

	MaterialEffectFloat2* effectFloat2 = material->GetEffect<MaterialEffectFloat2>();
	if (effectFloat2 != nullptr)
	{
		LoadVector2(mLocationScreenSize, effectFloat2->GetValue());
	}

	MaterialEffectFloat3* effectFloat3 = material->GetEffect<MaterialEffectFloat3>();
	if (effectFloat3 != nullptr)
	{
		LoadVector3(mLocationContactPoint, effectFloat3->GetValue());
	}
}

void EnergyWallShader::GetAllUniformLocations()
{
	ModelShader::GetAllUniformLocations();
	mLocationDepthTexture = GetUniformLocation(ATTRIBUTE_DEPTH_TEXTURE);
	mLocationContactPoint = GetUniformLocation(ATTRIBUTE_CONTACT_POINT);
	mLocationScreenSize = GetUniformLocation(ATTRIBUTE_SCREEN_SIZE);
	mLocationTimer = GetUniformLocation(ATTRIBUTE_TIMER);
}