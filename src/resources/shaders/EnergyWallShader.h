#pragma once
#include "ModelShader.h"

class EnergyWallShader : public ModelShader
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;
	int mLocationDepthTexture;
	int mLocationContactPoint;
	int mLocationScreenSize;

public:
	explicit EnergyWallShader();
	~EnergyWallShader();

	void LoadData(const ICamera* camera, IMaterial* material) override;

	void GetAllUniformLocations();

	void LoadDepthTexture(int unit);
	void LoadContactPosition(const glm::vec3& contact);
	void LoadScreenSize(const glm::vec2& screenSize);
};

