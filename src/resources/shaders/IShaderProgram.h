#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

class IMaterial;
class ICamera;
class Transformation;

class IShaderProgram
{
protected:
	IShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	IShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename, const std::string& geometryShaderFilename);

public:
	virtual ~IShaderProgram();

	void Init();
	int GetProgramID() { return mProgramID; }
	void Use();
	void UnUse();

	
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
	void LoadVector3Array(int location, std::vector<glm::vec3>& values);

	virtual void LoadData(const ICamera* camera, const Transformation* transformation, IMaterial* material);
	virtual void BindAttributes() = 0;
	virtual void GetAllUniformLocations() = 0;

	int GetAttributePosition();
	virtual int GetGeometryShaderNumTriangles() const;

protected:
	int mLocationPosition;
	int mLocationClippingPlane;
	int mLocationFogDensity;
	int	mLocationFogGradient;
	int mLocationFogColor;
	int mLocationShadowSpaceMatrix;
	int mLocationShadowMapTexture;
	int mLocationShadowMapTextureWidth;
	int mLocationShadowMapPFC;
	int mLocationLightPosition;
	int	mLocationLightColor;
	
private:
	void BindAttributesInternal();
	void GetAllUniformLocationsInternal();
	int Load(const std::string& filename, int type);

	int mProgramID;
	int mVertexShaderID;
	int mFragmentShaderID;
	int mGeometryShaderID;
};

