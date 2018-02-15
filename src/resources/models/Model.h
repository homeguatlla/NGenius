#pragma once

#include "../../AABB.h"
#include "../../VertexBuffersManager.h"

class ModelGeometry;
class IMaterial;

class Model
{
	ModelGeometry* mModel;
	AABB mAABB;

public:
	Model(ModelGeometry* model);
	~Model();

	unsigned int GetID() const;
	const AABB& GetAABB() const;

	void Build(VertexBuffersManager& vertexBufferManager, IMaterial* material);

private:
	void CreateVertexsBuffer(VertexBuffersManager& vertexBufferManager, IMaterial* material);
	void CalculateAABB();
};

