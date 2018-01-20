#include "stdafx.h"
#include "GUITextRenderer.h"
#include "../resources/shaders/IShaderProgram.h"
#include "../resources/font/FontType.h"

#include <GL/glew.h>

GUITextRenderer::GUITextRenderer(IShaderProgram* shader, FontType* font, const glm::vec4& color, const int textID) :
	TextRenderer(shader, font, color, textID)
{
	SetLayer(IRenderer::LAYER_GUI);
}


GUITextRenderer::~GUITextRenderer()
{
}

const std::string GUITextRenderer::GetName() const
{
	return "GUITextRenderer_" + std::to_string(mTextID);
}

void GUITextRenderer::Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager)
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(false);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	if (!mIsPrerendered && vertexBufferManager.HasVAO(GetName()))
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVertexVBO);
		glBufferData(GL_ARRAY_BUFFER, mVertexs.size() * sizeof(glm::vec3), &mVertexs[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexVBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndexes.size() * sizeof(unsigned int), &mIndexes[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, mTextureCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, mTextureCoords.size() * sizeof(glm::vec2), &mTextureCoords[0], GL_STATIC_DRAW);
	}
	IRenderer::Render(camera, vertexBufferManager);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
	glEnable(GL_CULL_FACE);
}
