#pragma once

#include "../../AABB.h"
#include "../../VertexBuffersManager.h"
#include <vector>

class Mesh;
class IMaterial;

class Model
{
protected:
	Mesh* mMesh;

private:
	std::string mName;
	AABB mAABB;
	int mVAO;
	int mMatrixVBO;
	int mVertexsVBO;
	int mIndexesVBO;
	int mTextureCoordsVBO;
	int mColorVBO;

public:
	Model(const std::string& name, Mesh* model);
	~Model();

	std::string GetName() const { return mName; }
	unsigned int GetID() const;
	const AABB& GetAABB() const;
	int GetVAOID() const;

	int GetNumberOfVertexs() const;
	int GetNumberOfIndexes() const;

	void Apply(const glm::mat4& matrix);
	void Apply(const std::vector<glm::mat4>& matrices);
	void Apply(const std::vector<glm::vec4>& colors);

	bool IsBuilt() const;
	void Build(VertexBuffersManager& vertexBufferManager, IMaterial* material);
	virtual void BuildInternal(VertexBuffersManager& vertexBufferManager, IMaterial* material) {};
	void UpdateVBOs();

	virtual bool IsAnimatedModel() const;

private:
	void CreateVertexsVBO(VertexBuffersManager& vertexBufferManager, int location);
	void CreateIndexesVBO(VertexBuffersManager& vertexBufferManager);
	void CreateTextureCoordsVBO(VertexBuffersManager& vertexBufferManager, int location);
	void CreateNormalsVBO(VertexBuffersManager& vertexBufferManager, int location);
	void CreateTangentsVBO(VertexBuffersManager& vertexBufferManager, int location);
	void CreateModelMatrixVBO(VertexBuffersManager& vertexBufferManager, int location);
	void CreateColorVBO(VertexBuffersManager& vertexBufferManager, int location);
	void CalculateAABB();
};

