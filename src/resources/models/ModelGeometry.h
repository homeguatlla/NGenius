#pragma once

#include <vector>
#include <glm/glm.hpp>

class ModelGeometry
{
	static int IDCounter;

	std::vector<glm::vec3> mVertexs; 
	std::vector<glm::vec2> mTextureCoords;
	std::vector<glm::vec3> mNormals;
	std::vector<glm::vec3> mTangents;
	std::vector<unsigned int> mIndexes;
	int mModelID;
	std::string mMaterialName;
	std::string mDiffuseTextureName;
	std::string mNormalMapTextureName;

public:
	ModelGeometry();
	ModelGeometry(	const std::vector<glm::vec3>& vertexs,
			const std::vector<glm::vec2>& textureCoords,
			const std::vector<unsigned int>& indexes,
			const std::vector<glm::vec3>& normals);
	ModelGeometry(	const std::vector<glm::vec3>& vertexs,
			const std::vector<glm::vec2>& textureCoords,
			const std::vector<unsigned int>& indexes);
	~ModelGeometry();

	unsigned int GetID() const;
	void SetVertexs(std::vector<glm::vec3>& vertexs);
	std::vector<glm::vec3>& GetVertexs();
	void SetTextureCoords(std::vector<glm::vec2>& textureCoords);
	std::vector<glm::vec2>& GetTextureCoords();
	std::vector<glm::vec3>& GetNormals();
	void SetIndexes(std::vector<unsigned int>& indexes);
	std::vector<unsigned int>& GetIndexes();
	std::vector<glm::vec3>& GetTangents();

	long GetNumberOfVertexs() const;
	long GetNumberOfTextureCoords() const;
	long GetNumberOfNormals() const;
	long GetNumberOfIndexes() const;
	long GetNumberOfTangents() const;

	void Build(bool calculateNormals, bool calculateTangents);
	void SetMaterialName(const std::string& name);
	const std::string& GetMaterialName() const;
	void SetDiffuseTextureName(const std::string& name);
	const std::string& GetDiffuseTextureName() const;
	void SetNormalMapTextureName(const std::string& name);
	const std::string& GetNormalMapTextureName() const;	

private:
	void CalculateNormals();
	void CalculateTangents();
	glm::vec3 CalculateTriangleNormalFromVertex(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
};

