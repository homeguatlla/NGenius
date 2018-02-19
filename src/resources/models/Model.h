#pragma once

#include "../../AABB.h"
#include "../../VertexBuffersManager.h"
#include <vector>

class ModelGeometry;
class IMaterial;

class Model
{
	ModelGeometry* mModelGeometry;
	AABB mAABB;
	int mVAO;
	int mMatrixVBO;
	int mVertexsVBO;
	int mIndexesVBO;
	int mTextureCoordsVBO;

public:
	Model(ModelGeometry* model);
	~Model();

	unsigned int GetID() const;
	const AABB& GetAABB() const;
	int GetVAOID() const;

	int GetNumberOfVertexs() const;
	int GetNumberOfIndexes() const;

	void Apply(std::vector<glm::mat4>& matrices);

	bool IsBuilt() const;
	void Build(VertexBuffersManager& vertexBufferManager, IMaterial* material);
	void UpdateVBOs();

private:
	void CreateVertexsVBO(VertexBuffersManager& vertexBufferManager, int location);
	void CreateIndexesVBO(VertexBuffersManager& vertexBufferManager);
	void CreateTextureCoordsVBO(VertexBuffersManager& vertexBufferManager, int location);
	void CreateNormalsVBO(VertexBuffersManager& vertexBufferManager, int location);
	void CreateTangentsVBO(VertexBuffersManager& vertexBufferManager, int location);
	void CreateModelMatrixVBO(VertexBuffersManager& vertexBufferManager, int location);
	void CalculateAABB();
};

