#pragma once

#include "../Transformation.h"

class IMaterial;
class ImageRenderer;
class IFrameBuffer;
class Texture;

class PostProcessEffect
{
	unsigned int mWidth;
	unsigned int mHeight;
	Transformation mTransformation;
	IFrameBuffer* mFrameBuffer;
	Texture* mTexture;
	ImageRenderer* mRenderer;	

public:
	PostProcessEffect(Texture* texture, ImageRenderer* imageRenderer, unsigned int width, unsigned int height);
	virtual ~PostProcessEffect();

	void Init();
	unsigned int Render(unsigned int texture);
};

