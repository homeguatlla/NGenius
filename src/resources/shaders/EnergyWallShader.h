#pragma once
#include "ModelShader.h"

class EnergyWallShader : public ModelShader
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;
	int mLocationDepthTexture;
	int mLocationContactPoint;

public:
	explicit EnergyWallShader();
	~EnergyWallShader();

	void GetAllUniformLocations();

	void LoadDepthTexture(int unit);
	void LoadContactPosition(glm::vec3 contact);
};

