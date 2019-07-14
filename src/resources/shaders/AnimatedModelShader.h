#pragma once
#include "ModelShader.h"

class AnimatedModelShader : public ModelShader
{
	static const std::string VERTEX_FILE;
	static const std::string FRAGMENT_FILE;

	int mLocationJointTransformMatrixs;
	int mLocationJointTransformSize;

public:
	AnimatedModelShader();
	virtual ~AnimatedModelShader() = default;

	void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) override;
	
	void BindAttributes();
	void GetAllUniformLocations();

	static std::string GetClassName() { return std::string("AnimatedModelShader"); }
	static IShaderProgram* Create() { return DBG_NEW AnimatedModelShader(); }
};

