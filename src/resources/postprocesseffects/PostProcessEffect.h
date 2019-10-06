#pragma once

#include "../Transformation.h"

class IMaterial;
class ImageRenderer;
class IFrameBuffer;
class ITexture;
class Texture;
class MaterialEffectDiffuseTexture;
class RenderSystem;

class PostProcessEffect
{
public:
	enum class PostProcessEffectType
	{
		POSTPROCESS_TO_COLOR_BUFFER,
		POSTPROCESS_TO_SCREEN
	};

private:
	unsigned int mWidth;
	unsigned int mHeight;
	IFrameBuffer* mFrameBuffer;
	Texture* mOutTexture;
	MaterialEffectDiffuseTexture* mMaterialEffectDiffuseTexture;
	ImageRenderer* mRenderer;
	PostProcessEffectType mType;

public:
	PostProcessEffect(PostProcessEffectType type, Texture* outTexture, ImageRenderer* imageRenderer);
	virtual ~PostProcessEffect();

	void Init();
	void SetBufferSize(unsigned int width, unsigned int height);
	ITexture* Render(RenderSystem* renderSystem, ITexture* inTexture);
	bool HasFrameBuffer();
};

