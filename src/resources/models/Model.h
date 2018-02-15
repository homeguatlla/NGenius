#pragma once

#include "../../AABB.h"
#include "../../VertexBuffersManager.h"

class ModelGeometry;
class IMaterial;

class Model
{
	ModelGeometry* mModelGeometry;
	AABB mAABB;
	int mVAO;

public:
	Model(ModelGeometry* model);
	~Model();

	unsigned int GetID() const;
	const AABB& GetAABB() const;
	int GetVAOID() const;

	int GetNumberOfVertexs() const;

	bool IsBuilt() const;
	void Build(VertexBuffersManager& vertexBufferManager, IMaterial* material);
	
private:
	void CreateVertexsBuffer(VertexBuffersManager& vertexBufferManager, int location);
	void CalculateAABB();
};

