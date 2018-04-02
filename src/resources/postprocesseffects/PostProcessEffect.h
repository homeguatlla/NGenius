#pragma once

class IMaterial;
class ImageRenderer;

class PostProcessEffect
{
	unsigned int mWidth;
	unsigned int mHeight;
	ImageRenderer* mRenderer;

public:
	PostProcessEffect(ImageRenderer* imageRenderer, unsigned int width, unsigned int height);
	virtual ~PostProcessEffect();

	void Init();
	unsigned int Render(unsigned int texture);
};

