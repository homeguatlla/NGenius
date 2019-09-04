#pragma once
#include "IRenderer.h"
#include "../Memory.h"
#include<string>


class Model;
class IMaterial;

class IndicesRenderer : public IRenderer
{
	unsigned int mMatrixVBO;

public:
	IndicesRenderer(Model* model, IMaterial* material);
	~IndicesRenderer();

	IndicesRenderer* DoClone() const override { return nullptr; }
	static std::string GetClassName() { return std::string("IndicesRenderer"); }
	static IRenderer* Create(Model* model, IMaterial* material)
	{
		return DBG_NEW IndicesRenderer(model, material);
	}

private:
	bool IsInstancingAllowed() const override;
};

