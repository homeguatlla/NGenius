#pragma once
#include "TextRenderer.h"
#include <string>

class IShaderProgram;
class FontType;

class GUITextRenderer :	public TextRenderer
{
public:
	GUITextRenderer(IShaderProgram* shader, FontType* font, const glm::vec4& color, const int textID);
	~GUITextRenderer();

	const std::string GetName() const override;
	void Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager);
};

