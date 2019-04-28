#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

class Mesh
{
	static int IDCounter;

	std::vector<glm::vec3> mVertexs; 
	std::vector<glm::vec2> mTextureCoords;
	std::vector<glm::vec3> mNormals;
	std::vector<glm::vec3> mTangents;
	std::vector<unsigned int> mIndexes;

	//max of 4 joints per vertex
	std::vector<glm::vec4> mVertexWeights;
	std::vector<glm::ivec4> mVertexJointsIds;

	std::vector<std::vector<float>> mTempVertexWeights;
	std::vector<std::vector<int>> mTempVertexJointsIds;

	int mMeshID;
	std::string mMaterialName;
	std::string mDiffuseTextureName;
	std::string mNormalMapTextureName;

public:
	Mesh();
	Mesh(	const std::vector<glm::vec3>& vertexs,
			const std::vector<glm::vec2>& textureCoords,
			const std::vector<unsigned int>& indexes,
			const std::vector<glm::vec3>& normals);
	Mesh(	const std::vector<glm::vec3>& vertexs,
			const std::vector<glm::vec2>& textureCoords,
			const std::vector<unsigned int>& indexes);
	~Mesh();

	unsigned int GetID() const;
	void SetVertexs(std::vector<glm::vec3>& vertexs);
	void SetTextureCoords(std::vector<glm::vec2>& textureCoords);
	void SetIndexes(std::vector<unsigned int>& indexes);
	
	std::vector<glm::vec3>& GetVertexs();
	std::vector<glm::vec2>& GetTextureCoords();
	std::vector<glm::vec3>& GetNormals();
	std::vector<unsigned int>& GetIndexes();
	std::vector<glm::vec3>& GetTangents();

	long GetNumberOfVertexs() const;
	long GetNumberOfTextureCoords() const;
	long GetNumberOfNormals() const;
	long GetNumberOfIndexes() const;
	long GetNumberOfTangents() const;

	void Build(bool calculateNormals, bool calculateTangents);
	
	void SetMaterialName(const std::string& name);
	void SetDiffuseTextureName(const std::string& name);
	void SetNormalMapTextureName(const std::string& name);
	
	const std::string& GetMaterialName() const;
	const std::string& GetDiffuseTextureName() const;
	const std::string& GetNormalMapTextureName() const;

	void AddVertexWeightToVertex(int vertexIndex, float weight);
	void AddJointIdToVertex(int vertexIndex, int jointId);

	std::vector<glm::vec4>& GetVertexsWeights();
	std::vector<glm::ivec4>& GetVertexsJointsIDs();

private:
	void CalculateNormals();
	void CalculateTangents();
	glm::vec3 CalculateTriangleNormalFromVertex(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c);
	void CreateWeightsAndJointsVectors();
	void ReorderVertexWeightAndJointIdsToHaveGreaterFirst(std::vector<float>& weights, std::vector<int>& joints);
};

