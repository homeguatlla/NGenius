#pragma once
#include "IShaderProgram.h"
#include "../Memory.h"

class GUIShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

	int mLocationModelMatrix;
	int mLocationViewMatrix;
	int mLocationProjectionMatrix;
	int mLocationTexture;
	int mLocationTextureCoords;
public:

	GUIShader();
	virtual ~GUIShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;

	void BindAttributes() override;
	void GetAllUniformLocations() override;

	static std::string GetClassName() { return std::string("GUIShader"); }
	static IShaderProgram* Create() { return DBG_NEW GUIShader(); }
};

