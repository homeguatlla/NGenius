#include "stdafx.h"
#include "IShaderProgram.h"
#include "../materials/IMaterial.h"
#include "../materials/effects/MaterialEffectClippingPlane.h"
#include "../materials/effects/MaterialEffectDirectionalLightProperties.h"
#include "../materials/effects/MaterialEffectFogProperties.h"
#include "../materials/effects/MaterialEffectShadowProperties.h"
#include "../textures/ITexture.h"
#include <glm/glm.hpp>

#include "../../utils/Log.h"

#include <GL/glew.h>
#include <fstream>
#include <vector>
#include <iostream>

const std::string ATTRIBUTE_VERTEX_POSITION("vertexModelspace");
const std::string ATTRIBUTE_CLIP_PLANE("clippingPlane");

const std::string ATTRIBUTE_FOG_DENSITY("fogDensity");
const std::string ATTRIBUTE_FOG_GRADIENT("fogGradient");
const std::string ATTRIBUTE_FOG_COLOR("fogColor");

const std::string ATTRIBUTE_SHADOW_SPACE_MATRIX("toShadowMapSpace");
const std::string ATTRIBUTE_SHADOW_TEXTURE("shadowMap");
const std::string ATTRIBUTE_SHADOW_TEXTURE_WIDTH("shadowMapSize");
const std::string ATTRIBUTE_SHADOW_PFC("pfcCount");

const std::string ATTRIBUTE_LIGHT_POSITION("lightPositionWorldspace");
const std::string ATTRIBUTE_LIGHT_COLOR("lightColor");
const std::string ATTRIBUTE_HAS_LIGHT_ENABLED("hasLightEnabled");

IShaderProgram::IShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) : 
mLocationPosition(-1),
mLocationClippingPlane(-1),
mLocationFogDensity(-1),
mLocationFogGradient(-1),
mLocationFogColor(-1),
mLocationShadowSpaceMatrix(-1),
mLocationShadowMapTexture(-1),
mLocationShadowMapTextureWidth(-1),
mLocationShadowMapPFC(-1),
mLocationLightPosition(-1),
mLocationLightColor(-1),
mLocationHasLightEnabled(-1),
mGeometryShaderID(-1)
{
	mVertexShaderID = Load(vertexShaderFilename, GL_VERTEX_SHADER);
	mFragmentShaderID = Load(fragmentShaderFilename, GL_FRAGMENT_SHADER);
}

IShaderProgram::IShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename, const std::string& geometryShaderFilename) :
	IShaderProgram(vertexShaderFilename, fragmentShaderFilename)
{
	mGeometryShaderID = Load(geometryShaderFilename, GL_GEOMETRY_SHADER);
}

IShaderProgram::~IShaderProgram()
{
	UnUse();

	glDetachShader(mProgramID, mVertexShaderID);
	glDetachShader(mProgramID, mFragmentShaderID);
	glDetachShader(mProgramID, mGeometryShaderID);
	glDeleteShader(mVertexShaderID);
	glDeleteShader(mFragmentShaderID);
	glDeleteShader(mGeometryShaderID);
	glDeleteProgram(mProgramID);
}

void IShaderProgram::Init()
{
	// Link the program
	Log(Log::LOG_INFO) << "Linking program\n";
	mProgramID = glCreateProgram();
	glAttachShader(mProgramID, mVertexShaderID);
	glAttachShader(mProgramID, mFragmentShaderID);
	if (mGeometryShaderID != -1)
	{
		glAttachShader(mProgramID, mGeometryShaderID);
	}

	glLinkProgram(mProgramID);
	glValidateProgram(mProgramID);

	// Check the program
	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage(glm::max(InfoLogLength, int(1)));
	glGetProgramInfoLog(mProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	Log(Log::LOG_ERROR) << &ProgramErrorMessage[0] << "\n";

	glDeleteShader(mVertexShaderID);
	glDeleteShader(mFragmentShaderID);
	if (mGeometryShaderID != -1)
	{
		glDeleteShader(mGeometryShaderID);
	}

	Use();
	GetAllUniformLocationsInternal();
	BindAttributesInternal();
	UnUse();
}

void IShaderProgram::Use()
{
	glUseProgram(mProgramID);
}

void IShaderProgram::UnUse()
{
	//glUseProgram(0);
}

void IShaderProgram::BindAttributesInternal()
{
	BindAttribute(mLocationPosition, ATTRIBUTE_VERTEX_POSITION);
	BindAttributes();
}

void IShaderProgram::LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material)
{
	MaterialEffectClippingPlane* effectClipping = material->GetEffect<MaterialEffectClippingPlane>();
	if (effectClipping != nullptr)
	{
		LoadVector4(mLocationClippingPlane, effectClipping->GetClippingPlane());
	}
	MaterialEffectDirectionalLightProperties* effectLight = material->GetEffect<MaterialEffectDirectionalLightProperties>();
	if (effectLight != nullptr)
	{
		LoadVector3(mLocationLightPosition, effectLight->GetSunLightDirection());
		LoadVector3(mLocationLightColor, effectLight->GetSunLightColor());
		LoadBool(mLocationHasLightEnabled, true);
	}
	else
	{
		LoadBool(mLocationHasLightEnabled, false);
	}

	MaterialEffectFogProperties* effectFog = material->GetEffect<MaterialEffectFogProperties>();
	if (effectFog != nullptr)
	{
		LoadVector3(mLocationFogColor, effectFog->GetColor());
		LoadFloat(mLocationFogDensity, effectFog->GetDensity());
		LoadFloat(mLocationFogGradient, effectFog->GetGradient());
	}

	MaterialEffectShadowProperties* effectShadow = material->GetEffect<MaterialEffectShadowProperties>();
	if (effectShadow != nullptr && effectShadow->GetDepthTexture() != nullptr)
	{
		LoadMatrix4(mLocationShadowSpaceMatrix, effectShadow->GetMatrix());
		LoadTexture(mLocationShadowMapTexture, effectShadow->GetDepthTexture()->GetUnit());
		LoadInteger(mLocationShadowMapTextureWidth, effectShadow->GetDepthTexture()->GetWidth());
		LoadInteger(mLocationShadowMapPFC, effectShadow->GetPFCCounter());
	}
}

void IShaderProgram::BindAttribute(int attribute, const std::string& variableName)
{
	glBindAttribLocation(mProgramID, attribute, variableName.c_str());
}

void IShaderProgram::GetAllUniformLocationsInternal()
{
	mLocationPosition = GetAttributeLocation(ATTRIBUTE_VERTEX_POSITION);
	mLocationClippingPlane = GetUniformLocation(ATTRIBUTE_CLIP_PLANE);
	mLocationFogDensity = GetUniformLocation(ATTRIBUTE_FOG_DENSITY);
	mLocationFogGradient = GetUniformLocation(ATTRIBUTE_FOG_GRADIENT);
	mLocationFogColor = GetUniformLocation(ATTRIBUTE_FOG_COLOR);
	mLocationShadowSpaceMatrix = GetUniformLocation(ATTRIBUTE_SHADOW_SPACE_MATRIX);
	mLocationShadowMapTexture = GetUniformLocation(ATTRIBUTE_SHADOW_TEXTURE);
	mLocationShadowMapTextureWidth = GetUniformLocation(ATTRIBUTE_SHADOW_TEXTURE_WIDTH);
	mLocationShadowMapPFC = GetUniformLocation(ATTRIBUTE_SHADOW_PFC);
	mLocationLightPosition = GetUniformLocation(ATTRIBUTE_LIGHT_POSITION);
	mLocationLightColor = GetUniformLocation(ATTRIBUTE_LIGHT_COLOR);
	mLocationHasLightEnabled = GetUniformLocation(ATTRIBUTE_HAS_LIGHT_ENABLED);

	GetAllUniformLocations();
}

int IShaderProgram::GetUniformLocation(const std::string& uniformName)
{
	return glGetUniformLocation(mProgramID, uniformName.c_str());
}

int IShaderProgram::GetAttributeLocation(const std::string& name)
{
	return glGetAttribLocation(mProgramID, name.c_str());
}

int IShaderProgram::GetAttributePosition()
{
	return mLocationPosition;
}

int IShaderProgram::GetGeometryShaderNumTriangles() const
{ 
	return 0; 
}

void IShaderProgram::LoadFloat(int location, float value)
{
	glUniform1f(location, value);
}

void IShaderProgram::LoadTexture(int location, int unit)
{
	glUniform1i(location, unit);
}

void IShaderProgram::LoadInteger(int location, int value)
{
	glUniform1i(location, value);
}

void IShaderProgram::LoadVector2(int location, const glm::vec2& value)
{
	glUniform2f(location, value.x, value.y);
}

void IShaderProgram::LoadVector3(int location, const glm::vec3& value)
{
	glUniform3f(location, value.x, value.y, value.z);
}

void IShaderProgram::LoadVector4(int location, const glm::vec4& value)
{
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void IShaderProgram::LoadBool(int location, bool value)
{
	glUniform1i(location, value ? 1 : 0);
}

void IShaderProgram::LoadMatrix4(int location, const glm::mat4& value)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

void IShaderProgram::LoadVector3Array(int location, std::vector<glm::vec3>& values)
{
	if (values.size() > 0)
	{
		glUniform3fv(location, values.size(), reinterpret_cast<float*>(&values[0]));
	}
}

void IShaderProgram::LoadMatrix4Array(int location, std::vector<glm::mat4>& values)
{
	if (values.size() > 0)
	{
		glUniformMatrix4fv(location, values.size(), GL_FALSE, reinterpret_cast<float*>(&values[0]));
	}
}

int IShaderProgram::Load(const std::string& filename, int type)
{
		// Create the shaders
		GLuint shaderID = glCreateShader(type);
		
		// Read the Vertex Shader code from the file
		std::string ShaderCode;
		std::ifstream ShaderStream(filename.c_str(), std::ios::in);
		if (ShaderStream.is_open())
		{
			std::string Line = "";
			while (getline(ShaderStream, Line))
				ShaderCode += "\n" + Line;
			ShaderStream.close();
		}
		else
		{
			Log(Log::LOG_ERROR) << "Failed to open " << filename << "\n";
			return -1;
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Compile Vertex Shader
		Log(Log::LOG_INFO) << "Compiling shader :" << filename << "\n";
		char const *SourcePointer = ShaderCode.c_str();
		glShaderSource(shaderID, 1, &SourcePointer, NULL);
		glCompileShader(shaderID);

		// Check Vertex Shader
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0)
		{
			std::vector<char> ShaderErrorMessage(InfoLogLength);
			glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
			Log(Log::LOG_ERROR) << &ShaderErrorMessage[0] << "\n";
		}

		return shaderID;
}
