#include "stdafx.h"
#include "EnergyWallShader.h"

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

void EnergyWallShader::LoadData(const ICamera* camera, IMaterial* material)
{

}

void EnergyWallShader::GetAllUniformLocations()
{
	ModelShader::GetAllUniformLocations();
	mLocationDepthTexture = GetUniformLocation(ATTRIBUTE_DEPTH_TEXTURE);
	mLocationContactPoint = GetUniformLocation(ATTRIBUTE_CONTACT_POINT);
	mLocationScreenSize = GetUniformLocation(ATTRIBUTE_SCREEN_SIZE);
}

void EnergyWallShader::LoadDepthTexture(int unit)
{
	LoadTexture(mLocationDepthTexture, unit);
}

void EnergyWallShader::LoadContactPosition(const glm::vec3& contact)
{
	LoadVector3(mLocationContactPoint, contact);
}

void EnergyWallShader::LoadScreenSize(const glm::vec2& screenSize)
{
	LoadVector2(mLocationScreenSize, screenSize);
}
