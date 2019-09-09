#pragma once
#include "TextRenderer.h"
#include <string>

class Model;
class IMaterial;

class GUITextRenderer :	public TextRenderer
{
public:
	GUITextRenderer(Model* model, IMaterial* material);
	~GUITextRenderer();
};

