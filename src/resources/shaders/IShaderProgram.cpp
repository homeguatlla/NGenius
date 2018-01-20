#include "stdafx.h"
#include "IShaderProgram.h"
#include <glm/glm.hpp>

#include <GL/glew.h>
#include <fstream>
#include <vector>
#include <iostream>

const std::string ATTRIBUTE_VERTEX_POSITION("vertexModelspace");

IShaderProgram::IShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) : 
mLocationPosition(-1)
{
	mVertexShaderID = Load(vertexShaderFilename, GL_VERTEX_SHADER);
	mFragmentShaderID = Load(fragmentShaderFilename, GL_FRAGMENT_SHADER);
}


IShaderProgram::~IShaderProgram()
{
	UnUse();

	glDetachShader(mProgramID, mVertexShaderID);
	glDetachShader(mProgramID, mFragmentShaderID);
	glDeleteShader(mVertexShaderID);
	glDeleteShader(mFragmentShaderID);
	glDeleteProgram(mProgramID);
}

void IShaderProgram::Init()
{
	// Link the program
	std::cout << "Linking program\n";
	mProgramID = glCreateProgram();
	glAttachShader(mProgramID, mVertexShaderID);
	glAttachShader(mProgramID, mFragmentShaderID);

	glLinkProgram(mProgramID);

	glValidateProgram(mProgramID);

	// Check the program
	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetProgramiv(mProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(mProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage(glm::max(InfoLogLength, int(1)));
	glGetProgramInfoLog(mProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	std::cout << &ProgramErrorMessage[0] << "\n";

	glDeleteShader(mVertexShaderID);
	glDeleteShader(mFragmentShaderID);

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

void IShaderProgram::BindAttribute(int attribute, const std::string& variableName)
{
	glBindAttribLocation(mProgramID, attribute, variableName.c_str());
}

void IShaderProgram::GetAllUniformLocationsInternal()
{
	mLocationPosition = GetAttributeLocation(ATTRIBUTE_VERTEX_POSITION);
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

void IShaderProgram::LoadFloat(int location, float value)
{
	glUniform1f(location, value);
}

void IShaderProgram::LoadTexture(int location, int unit)
{
	glUniform1i(location, unit);
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
			std::cout << "Failed to open " << filename << "\n";
			return -1;
		}

		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Compile Vertex Shader
		std::cout << "Compiling shader :" << filename << "\n";
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
			std::cout << &ShaderErrorMessage[0] << "\n";
		}

		return shaderID;
}
