#pragma once
#include "IShaderProgram.h"
#include "../IFactory.h"

class ModelShader :	public IShaderProgram, public IFactory
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	ModelShader();
	ModelShader(const std::string& vertexFile, const std::string& fragmentFile);
	~ModelShader();

	virtual void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes();
	void GetAllUniformLocations();

private:
	int mLocationModelMatrix;
	int mLocationViewMatrix;
	int mLocationProjectionMatrix;
	int mLocationCameraPosition;
	int mLocationTextureCoords;
	int mLocationNormal;
	int mLocationTexture;
	int mLocationTile;

	// Heredado vía IFactory
	virtual IShaderProgram* CreateShader() override;
};

