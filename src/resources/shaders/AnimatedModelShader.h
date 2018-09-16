#pragma once
#include "ModelShader.h"

class AnimatedModelShader : public ModelShader
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

	int mLocationJointTransformMatrixs;

public:
	AnimatedModelShader();
	~AnimatedModelShader();

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;
	
	void BindAttributes();
	void GetAllUniformLocations();
};

