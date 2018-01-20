#include "stdafx.h"
#include "EnergyWallShader.h"

const std::string EnergyWallShader::VERTEX_FILE = "data/shaders/vertex/v_energy_wall.cg";
const std::string EnergyWallShader::FRAGMENT_FILE = "data/shaders/fragment/f_energy_wall.cg";

const std::string ATTRIBUTE_DEPTH_TEXTURE("depthTexture");
const std::string ATTRIBUTE_CONTACT_POINT("contactPoint");

EnergyWallShader::EnergyWallShader() : 
ModelShader(EnergyWallShader::VERTEX_FILE, EnergyWallShader::FRAGMENT_FILE),
mLocationDepthTexture(-1),
mLocationContactPoint(-1)
{
}


EnergyWallShader::~EnergyWallShader()
{
}

void EnergyWallShader::GetAllUniformLocations()
{
	ModelShader::GetAllUniformLocations();
	mLocationDepthTexture = GetUniformLocation(ATTRIBUTE_DEPTH_TEXTURE);
	mLocationContactPoint = GetUniformLocation(ATTRIBUTE_CONTACT_POINT);
}

void EnergyWallShader::LoadDepthTexture(int unit)
{
	LoadTexture(mLocationDepthTexture, unit);
}

void EnergyWallShader::LoadContactPosition(glm::vec3 contact)
{
	LoadVector3(mLocationContactPoint, contact);
}
