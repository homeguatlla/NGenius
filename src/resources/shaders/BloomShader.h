#pragma once
#include "IShaderProgram.h"
#include "../IFactory.h"

class BloomShader : public IShaderProgram
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

public:
	BloomShader();
	virtual ~BloomShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;
	
	void BindAttributes() override;
	void GetAllUniformLocations() override;

	static std::string GetClassName() { return std::string("BloomShader"); }
	static IShaderProgram* Create()	{ return new BloomShader();	}

private:
	int mLocationTexture;
	int mLocationBrightTexture;
};

