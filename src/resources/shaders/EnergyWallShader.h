#pragma once
#include "ModelShader.h"
#include "../Memory.h"

class EnergyWallShader : public ModelShader
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;
	int mLocationDepthTexture;
	int mLocationContactPoint;
	int mLocationScreenSize;
	int mLocationTimer;

public:
	explicit EnergyWallShader();
	virtual ~EnergyWallShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void GetAllUniformLocations();

	static std::string GetClassName() { return std::string("EnergyWallShader"); }
	static IShaderProgram* Create() { return DBG_NEW EnergyWallShader(); }
};

