#pragma once
#include "IRenderer_.h"
#include <string>

class IShaderProgram;
class FontType;
class Model;

class TextRenderer : public IRenderer_
{

public:
	explicit TextRenderer(Model* model, IMaterial* material);
	~TextRenderer();

	TextRenderer* DoClone() const override { return nullptr; }
	bool IsInstancingAllowed() const override;

private:
	void Draw() override;
};

