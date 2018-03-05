#pragma once
#include <string>
#include <glm/glm.hpp>

class IMaterial;
class ICamera;
class Transformation;

class IShaderProgram
{
protected:
	IShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

public:
	virtual ~IShaderProgram();

	void Init();
	int GetProgramID() { return mProgramID; }
	void Use();
	void UnUse();

	virtual void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material) = 0;

	void BindAttribute(int attribute, const std::string& variableName);
	int GetUniformLocation(const std::string& uniformName);
	int GetAttributeLocation(const std::string& name);
	void LoadFloat(int location, float value);
	void LoadVector2(int location, const glm::vec2& value);
	void LoadVector3(int location, const glm::vec3& value);
	void LoadVector4(int location, const glm::vec4& value);
	void LoadBool(int location, bool value);
	void LoadMatrix4(int location, const glm::mat4& value);
	void LoadTexture(int location, int unit);
	void LoadInteger(int location, int value);

	virtual void BindAttributes() = 0;
	virtual void GetAllUniformLocations() = 0;

	int GetAttributePosition();

protected:
	int mLocationPosition;
	
private:
	void BindAttributesInternal();
	void GetAllUniformLocationsInternal();
	int Load(const std::string& filename, int type);

	int mProgramID;
	int mVertexShaderID;
	int mFragmentShaderID;
};

