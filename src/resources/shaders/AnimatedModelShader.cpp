#include "stdafx.h"
#include "AnimatedModelShader.h"
#include "ModelShader.h"
#include "../textures/ITexture.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectMatrix4Array.h"

const std::string AnimatedModelShader::VERTEX_FILE = "data/shaders/vertex/v_model_animated.cg";
const std::string AnimatedModelShader::FRAGMENT_FILE = "data/shaders/fragment/f_model.cg";

const std::string ATTRIBUTE_JOINT_TRANSFORM_MATRIXS("jointTransforms");

AnimatedModelShader::AnimatedModelShader() :
ModelShader(VERTEX_FILE, FRAGMENT_FILE),
mLocationJointTransformMatrixs(-1)
{
}

AnimatedModelShader::~AnimatedModelShader()
{
}

void AnimatedModelShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	ModelShader::LoadData(camera, transformation, material);

	MaterialEffectMatrix4Array* effectMatrixArray = material->GetEffect<MaterialEffectMatrix4Array>();
	if (effectMatrixArray != nullptr)
	{
		LoadMatrix4Array(mLocationJointTransformMatrixs, effectMatrixArray->GetValues());
	}
}

void AnimatedModelShader::BindAttributes()
{
	ModelShader::BindAttributes();
	BindAttribute(mLocationJointTransformMatrixs, ATTRIBUTE_JOINT_TRANSFORM_MATRIXS);
}

void AnimatedModelShader::GetAllUniformLocations()
{
	ModelShader::GetAllUniformLocations();

	mLocationJointTransformMatrixs = GetUniformLocation(ATTRIBUTE_JOINT_TRANSFORM_MATRIXS);
}