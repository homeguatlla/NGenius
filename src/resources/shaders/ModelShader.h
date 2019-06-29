#pragma once
#include "IShaderProgram.h"

class ModelShader :	public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	ModelShader();
	ModelShader(const std::string& vertexFile, const std::string& fragmentFile);
	virtual ~ModelShader() = default;

	virtual void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes();
	void GetAllUniformLocations();

	static std::string GetClassName() { return std::string("ModelShader"); }
	static IShaderProgram* Create() { return new ModelShader(); }

private:
	int mLocationModelMatrix;
	int mLocationViewMatrix;
	int mLocationProjectionMatrix;
	int mLocationCameraPosition;
	int mLocationTextureCoords;
	int mLocationNormal;
	int mLocationTexture;
	int mLocationTile;

};

