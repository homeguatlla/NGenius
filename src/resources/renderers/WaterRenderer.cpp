#include "stdafx.h"
#include "WaterRenderer.h"

#include <GL/glew.h>

WaterRenderer::WaterRenderer(Model* model, IMaterial* material) :
	IndicesRenderer(model, material)
{
}

WaterRenderer::~WaterRenderer()
{
}

void WaterRenderer::Draw()
{
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	IndicesRenderer::Draw();

	//glDisable(GL_BLEND);
}