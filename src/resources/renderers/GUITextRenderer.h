#pragma once
#include "TextRenderer.h"
#include <string>

class Model;
class IMaterial;

class GUITextRenderer :	public TextRenderer
{
	int mSourceFactor;
	int mDestinationFactor;

public:
	GUITextRenderer(Model* model, IMaterial* material);
	~GUITextRenderer();

	void SetBlendFactors(int source, int destination);
private:
	void Draw() override;
};

