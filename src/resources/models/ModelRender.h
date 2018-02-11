#pragma once

#include "../../AABB.h"
#include "../../VertexBuffersManager.h"

class Model;
class IMaterial;

class ModelRender
{
	Model* mModel;
	AABB mAABB;

public:
	ModelRender(Model* model);
	~ModelRender();

	unsigned int GetID() const;
	const AABB& GetAABB() const;

	void Build(VertexBuffersManager& vertexBufferManager, IMaterial* material);

private:
	void CreateVertexsBuffer(VertexBuffersManager& vertexBufferManager, IMaterial* material);
	void CalculateAABB();
};

