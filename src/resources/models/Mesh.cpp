#include "stdafx.h"
#include "Mesh.h"

#include <algorithm>

int Mesh::IDCounter = 0;
const float EPSILON = 0.0001f;
const float EPSILON2 = EPSILON * EPSILON;

Mesh::Mesh()
{
	mMeshID = ++IDCounter;
}

Mesh::Mesh(const std::vector<glm::vec3>& vertexs,
	const std::vector<glm::vec2>& textureCoords,
	const std::vector<unsigned int>& indexes) :
	mVertexs(vertexs),
	mTextureCoords(textureCoords),
	mIndexes(indexes)
{
	mMeshID = ++IDCounter;
	assert(vertexs.size() > 0);
}

Mesh::Mesh(const std::vector<glm::vec3>& vertexs,
	const std::vector<glm::vec2>& textureCoords,
	const std::vector<unsigned int>& indexes,
	const std::vector<glm::vec3>& normals) :
	mVertexs(vertexs),
	mTextureCoords(textureCoords),
	mIndexes(indexes),
	mNormals(normals)
{
	mMeshID = ++IDCounter;
	assert(vertexs.size() > 0);
}

Mesh::~Mesh()
{
}

unsigned int Mesh::GetID() const
{
	return mMeshID;
}

void Mesh::SetVertexs(std::vector<glm::vec3>& vertexs)
{
	mVertexs = vertexs;
}

std::vector<glm::vec3>& Mesh::GetVertexs()
{
	return mVertexs;
}

long Mesh::GetNumberOfVertexs() const
{
	return mVertexs.size();
}

void Mesh::SetTextureCoords(std::vector<glm::vec2>& textureCoords)
{
	mTextureCoords = textureCoords;
}

std::vector<glm::vec2>& Mesh::GetTextureCoords()
{
	return mTextureCoords;
}

long Mesh::GetNumberOfTextureCoords() const
{
	return mTextureCoords.size();
}

std::vector<glm::vec3>& Mesh::GetNormals()
{
	return mNormals;
}

long Mesh::GetNumberOfNormals() const
{
	return mNormals.size();
}

std::vector<glm::vec3>& Mesh::GetTangents()
{
	return mTangents;
}

long Mesh::GetNumberOfTangents() const
{
	return mTangents.size();
}

void Mesh::SetIndexes(std::vector<unsigned int>& indexes)
{
	mIndexes = indexes;
}

std::vector<unsigned int>& Mesh::GetIndexes()
{
	return mIndexes;
}

long Mesh::GetNumberOfIndexes() const
{
	return mIndexes.size();
}

void Mesh::Build(bool calculateNormals, bool calculateTangents)
{
	if (calculateNormals)
	{
		CalculateNormals();
	}
	if (calculateTangents && mTextureCoords.size() > 0)
	{
		CalculateTangents();
	}

	CreateWeightsAndJointsVectors();
}

void  Mesh::SetMaterialName(const std::string& name)
{
	mMaterialName = name;
}

const std::string&  Mesh::GetMaterialName() const
{
	return mMaterialName;
}

void Mesh::SetDiffuseTextureName(const std::string& name)
{
	mDiffuseTextureName = name;
}

const std::string& Mesh::GetDiffuseTextureName() const
{
	return mDiffuseTextureName;
}

void Mesh::SetNormalMapTextureName(const std::string& name)
{
	mNormalMapTextureName = name;
}

const std::string& Mesh::GetNormalMapTextureName() const
{
	return mNormalMapTextureName;
}


void Mesh::AddVertexWeightToVertex(int vertexIndex, float weight)
{
	if (mTempVertexWeights.size() == 0)
	{
		mTempVertexWeights.reserve(mVertexs.size());
		for (unsigned int i = 0; i < mVertexs.size(); ++i)
		{
			mTempVertexWeights.push_back(std::vector<float>());
		}
	}
	mTempVertexWeights[vertexIndex].push_back(weight);
	/*if (mVertexWeights[vertexIndex].x == 0.0f) mVertexWeights[vertexIndex].x = weight;
	else if (mVertexWeights[vertexIndex].y == 0.0f) mVertexWeights[vertexIndex].y = weight;
	else if (mVertexWeights[vertexIndex].z == 0.0f) mVertexWeights[vertexIndex].z = weight;
	else if (mVertexWeights[vertexIndex].w == 0.0f) mVertexWeights[vertexIndex].w = weight;*/
}

void Mesh::AddJointIdToVertex(int vertexIndex, int jointId)
{
	if (mTempVertexJointsIds.size() == 0)
	{
		mTempVertexJointsIds.reserve(mVertexs.size());
		for (unsigned int i = 0; i < mVertexs.size(); ++i)
		{
			mTempVertexJointsIds.push_back(std::vector<int>());
		}
	}
	mTempVertexJointsIds[vertexIndex].push_back(jointId);
	/*
	if (mVertexJointsIds[vertexIndex].x == -1) mVertexJointsIds[vertexIndex].x = jointId;
	else if (mVertexJointsIds[vertexIndex].y == -1) mVertexJointsIds[vertexIndex].y = jointId;
	else if (mVertexJointsIds[vertexIndex].z == -1) mVertexJointsIds[vertexIndex].z = jointId;
	else if (mVertexJointsIds[vertexIndex].w == -1) mVertexJointsIds[vertexIndex].w = jointId;
	*/
}

std::vector<glm::vec4>& Mesh::GetVertexsWeights()
{
	return mVertexWeights;
}

std::vector<glm::ivec4>& Mesh::GetVertexsJointsIDs()
{
	return mVertexJointsIds;
}

void Mesh::CreateWeightsAndJointsVectors()
{
	for (unsigned int i = 0; i < mTempVertexWeights.size(); ++i)
	{
		if (mTempVertexWeights[i].size() > 4)
		{
			ReorderVertexWeightAndJointIdsToHaveGreaterFirst(mTempVertexWeights[i], mTempVertexJointsIds[i]);
		}

		float totalValue = 0.0f;
		for (unsigned int j = 0; j < mTempVertexWeights[i].size() && j < 4; ++j)
		{
			totalValue += mTempVertexWeights[i][j];
		}

		glm::vec4 weight;
		glm::ivec4 joint;
		if (mTempVertexWeights[i].size() == 1)
		{
			weight = glm::vec4(mTempVertexWeights[i][0], 0.0f, 0.0f, 0.0f);
			joint = glm::ivec4(mTempVertexJointsIds[i][0], -1, -1, -1);
		}
		else if (mTempVertexWeights[i].size() == 2)
		{
			weight = glm::vec4(mTempVertexWeights[i][0], mTempVertexWeights[i][1], 0.0f, 0.0f);
			joint = glm::ivec4(mTempVertexJointsIds[i][0], mTempVertexJointsIds[i][1], -1, -1);
		}
		else if (mTempVertexWeights[i].size() == 3)
		{
			weight = glm::vec4(mTempVertexWeights[i][0], mTempVertexWeights[i][1], mTempVertexWeights[i][2], 0.0f);
			joint = glm::ivec4(mTempVertexJointsIds[i][0], mTempVertexJointsIds[i][1], mTempVertexJointsIds[i][2], -1);
		}
		else if (mTempVertexWeights[i].size() > 3)
		{
			weight = glm::vec4(mTempVertexWeights[i][0], mTempVertexWeights[i][1], mTempVertexWeights[i][2], mTempVertexWeights[i][3]);
			joint = glm::ivec4(mTempVertexJointsIds[i][0], mTempVertexJointsIds[i][1], mTempVertexJointsIds[i][2], mTempVertexJointsIds[i][3]);
		}
		mVertexWeights.push_back(weight / totalValue);
		mVertexJointsIds.push_back(joint);
	}
}

void Mesh::ReorderVertexWeightAndJointIdsToHaveGreaterFirst(std::vector<float>& weights, std::vector<int>& joints)
{
	std::vector<std::pair<float, int>> tempVector;

	for (unsigned int i = 0; i < weights.size(); ++i)
	{
		tempVector.push_back(std::pair<float, int>(weights[i], joints[i]));
	}

	weights.clear();
	joints.clear();
	std::sort(tempVector.begin(), tempVector.end(),
		[](const std::pair<float, int> & a, const std::pair<float, int> & b) -> bool
	{
		return a.first > b.first;
	});

	for (unsigned int i = 0; i < tempVector.size(); ++i)
	{
		weights.push_back(tempVector[i].first);
		joints.push_back(tempVector[i].second);
	}
}

void Mesh::CalculateNormals()
{
	struct Face 
	{
		glm::vec3 vertex[3];
		glm::vec3 normal;
	};
	
	unsigned int numFaces = mVertexs.size() / 3;
	std::vector<Face> faces;
	faces.resize(numFaces);

	int vertex = 0;
	for (unsigned int face = 0 ; face < numFaces; ++face)
	{
		faces[face].vertex[0] = mVertexs[vertex];
		faces[face].vertex[1] = mVertexs[vertex + 1];
		faces[face].vertex[2] = mVertexs[vertex + 2];

		glm::vec3 normal = CalculateTriangleNormalFromVertex(faces[face].vertex[0], faces[face].vertex[1], faces[face].vertex[2]);
		faces[face].normal = normal;
		vertex += 3;
	}

	for (unsigned int i = 0; i < mVertexs.size(); ++i)
	{
		glm::vec3 normal(0.0f);
		for (unsigned int face = 0; face < faces.size(); ++face)
		{
			if (glm::length(faces[face].vertex[0] - mVertexs[i]) < EPSILON || 
				glm::length(faces[face].vertex[1] - mVertexs[i]) < EPSILON ||
				glm::length(faces[face].vertex[2] - mVertexs[i]) < EPSILON)
			{
				normal += faces[face].normal;
			}
		}
		normal = glm::normalize(normal);
		mNormals[i] = normal;
	}
}

glm::vec3 Mesh::CalculateTriangleNormalFromVertex(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
	//This assumes that A->B->C is a counter-clockwise ordering
	glm::vec3 n = glm::cross(b - a, c - a);
	float alpha = glm::length(n) / (length(b - a) * length(c - a));

	return glm::normalize(n) * glm::asin(alpha);
}

void Mesh::CalculateTangents()
{
	assert(mVertexs.size() > 0);
	assert(mTextureCoords.size() > 0);

	std::vector<glm::vec3> tan1(0.0);
	std::vector<glm::vec3> tan2(0.0);

	tan1.resize(mVertexs.size());
	tan2.resize(mVertexs.size());

	for (unsigned int i = 0; i < mVertexs.size() - 2; i += 3)
	{
		int index1 = i;
		int index2 = i + 1;
		int index3 = i + 2;
		glm::vec3 v1 = mVertexs[index1];
		glm::vec3 v2 = mVertexs[index2];
		glm::vec3 v3 = mVertexs[index3];

		glm::vec2 w1= mTextureCoords[index1];
		glm::vec2 w2= mTextureCoords[index2];
		glm::vec2 w3 = mTextureCoords[index3];

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		float r = 1.0f / (s1 * t2 - s2 * t1);
		glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
		glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

		tan1[index1] += sdir;
		tan1[index2] += sdir;
		tan1[index3] += sdir;

		tan2[index1] += tdir;
		tan2[index2] += tdir;
		tan2[index3] += tdir;
	}

	if (mNormals.size() > 0)
	{
		for (unsigned int a = 0; a < mVertexs.size(); a++)
		{
			const glm::vec3& n = mNormals[a];
			const glm::vec3& t = tan1[a];

			// Gram-Schmidt orthogonalize
			glm::vec3 tangent = glm::normalize(t - n * glm::dot(n, t));

			// Calculate handedness
			//tangent.w = (glm::dot(glm::cross(n, t), tan2[a]) < 0.0f) ? -1.0f : 1.0f;
			mTangents.push_back(tangent);
		}
	}
}