#include "stdafx.h"
#include "AnimatedModelShader.h"
#include "ModelShader.h"
#include "../textures/ITexture.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectMatrix4Array.h"

const std::string AnimatedModelShader::VERTEX_FILE = "data/shaders/vertex/v_model_animated.cg";
const std::string AnimatedModelShader::FRAGMENT_FILE = "data/shaders/fragment/f_model.cg";

const std::string ATTRIBUTE_JOINT_TRANSFORM_MATRIXS("jointTransforms");
const std::string ATTRIBUTE_JOINT_TRANSFORM_SIZE("jointTransformsSize");

AnimatedModelShader::AnimatedModelShader() :
ModelShader(VERTEX_FILE, FRAGMENT_FILE),
mLocationJointTransformMatrixs(-1),
mLocationJointTransformSize(-1)
{
}

void AnimatedModelShader::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	ModelShader::LoadData(camera, transformation, material);

	MaterialEffectMatrix4Array* effectMatrixArray = material->GetEffect<MaterialEffectMatrix4Array>();
	if (effectMatrixArray != nullptr)
	{
		std::vector<glm::mat4x4> values = effectMatrixArray->GetValues();
		LoadMatrix4Array(mLocationJointTransformMatrixs, values);
		LoadInteger(mLocationJointTransformSize, static_cast<int>(values.size()));
	}
	else
	{
		LoadInteger(mLocationJointTransformSize, 0);
	}
}

void AnimatedModelShader::BindAttributes()
{
	ModelShader::BindAttributes();
	BindAttribute(mLocationJointTransformMatrixs, ATTRIBUTE_JOINT_TRANSFORM_MATRIXS);
	BindAttribute(mLocationJointTransformSize, ATTRIBUTE_JOINT_TRANSFORM_SIZE);
}

void AnimatedModelShader::GetAllUniformLocations()
{
	ModelShader::GetAllUniformLocations();

	mLocationJointTransformMatrixs = GetUniformLocation(ATTRIBUTE_JOINT_TRANSFORM_MATRIXS);
	mLocationJointTransformSize = GetUniformLocation(ATTRIBUTE_JOINT_TRANSFORM_SIZE);
}

IShaderProgram * AnimatedModelShader::CreateShader()
{
	return new AnimatedModelShader();
}