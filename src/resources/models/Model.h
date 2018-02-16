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

public:
	Model(ModelGeometry* model);
	~Model();

	unsigned int GetID() const;
	const AABB& GetAABB() const;
	int GetVAOID() const;

	int GetNumberOfVertexs() const;

	void Apply(std::vector<glm::mat4>& matrices);

	bool IsBuilt() const;
	void Build(VertexBuffersManager& vertexBufferManager, IMaterial* material);
	
private:
	void CreateVertexsVBO(VertexBuffersManager& vertexBufferManager, int location);
	void CreateTextureCoordsVBO(VertexBuffersManager& vertexBufferManager, int location);
	void CreateNormalsVBO(VertexBuffersManager& vertexBufferManager, int location);
	void CreateModelMatrixVBO(VertexBuffersManager& vertexBufferManager, int location);
	void CalculateAABB();
};

