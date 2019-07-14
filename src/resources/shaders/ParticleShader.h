#pragma once
#include "IShaderProgram.h"
#include "../Memory.h"

class ParticleShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

	int mLocationModelMatrix;
	int mLocationViewMatrix;
	int mLocationProjectionMatrix;
	int mLocationTexture;
	int mLocationDepthTexture;
	int mLocationTextureCoords;
	int mLocationTile;
	int mLocationTime;
	int mLocationScreenSize;

public:

	ParticleShader();
	ParticleShader(const std::string& vertexFile, const std::string& fragmentFile);
	virtual ~ParticleShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes() override;
	void GetAllUniformLocations() override;

	static std::string GetClassName() { return std::string("ParticleShader"); }
	static IShaderProgram* Create() { return DBG_NEW ParticleShader(); }
};

