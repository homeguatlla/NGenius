#include "stdafx.h"
#include "Model.h"

int Model::IDCounter = 0;

Model::Model(const std::vector<glm::vec3>& vertexs,
	const std::vector<glm::vec2>& textureCoords,
	const std::vector<glm::vec3>& normals,
	const std::vector<unsigned int>& indexes) :
	mVertexs(vertexs),
	mTextureCoords(textureCoords),
	mNormals(normals),
	mIndexes(indexes)
{
	mModelID = ++IDCounter;
	assert(vertexs.size() > 0);
	CalculateTangents();
}


Model::~Model()
{
}

unsigned int Model::GetID() const
{
	return mModelID;
}

std::vector<glm::vec3>& Model::GetVertexs()
{
	return mVertexs;
}

std::vector<glm::vec2>& Model::GetTextureCoords()
{
	return mTextureCoords;
}

std::vector<glm::vec3>& Model::GetNormals()
{
	return mNormals;
}

std::vector<glm::vec3>& Model::GetTangents()
{
	return mTangents;
}

std::vector<unsigned int> Model::GetIndexes()
{
	return mIndexes;
}

void  Model::SetMaterialName(const std::string& name)
{
	mMaterialName = name;
}

const std::string&  Model::GetMaterialName() const
{
	return mMaterialName;
}

void Model::SetDiffuseTextureName(const std::string& name)
{
	mDiffuseTextureName = name;
}

const std::string& Model::GetDiffuseTextureName() const
{
	return mDiffuseTextureName;
}

void Model::SetNormalMapTextureName(const std::string& name)
{
	mNormalMapTextureName = name;
}

const std::string& Model::GetNormalMapTextureName() const
{
	return mNormalMapTextureName;
}

void Model::CalculateTangents()
{
	std::vector<glm::vec3> tan1(0.0f);
	std::vector<glm::vec3> tan2(0.0f);

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
		for (long a = 0; a < mVertexs.size(); a++)
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