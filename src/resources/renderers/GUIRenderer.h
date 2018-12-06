#pragma once
#include "TextRenderer.h"
#include <string>

class Model;
class IMaterial;

class GUIRenderer :	public TextRenderer
{
	int mSourceFactor;
	int mDestinationFactor;
	bool mIsBlendingEnabled;
	bool mIsDepthTestEnabled;

public:
	GUIRenderer(Model* model, IMaterial* material);
	~GUIRenderer();

	void SetBlendFactors(int source, int destination);
	bool IsBlendingEnabled() const;
	void SetBlendEnabled(bool enabled);
	void SetDepthTestEnabled(bool enabled);
	bool IsDepthTestEnabled() const;
private:
	void Draw() override;
};

