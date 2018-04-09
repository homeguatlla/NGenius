#pragma once

class Texture;

class IFrameBuffer
{
	enum FrameBufferType
	{
		FRAME_BUFFER_INVALID = 0,
		FRAME_BUFFER_COLOR_TEXTURE = 1,
		FRAME_BUFFER_DEPTH_TEXTURE = 2,
		FRAME_BUFFER_DEPTH = 4
	};

	unsigned int mFrameBufferID;
	unsigned int mDepthBufferID;
	Texture* mColorTexture;
	Texture* mDepthTexture;
	int mScreenWidth;
	int mScreenHeight;
	unsigned int mColorAttachmentNumber;
	char mType;
	unsigned int mRenderBufferWidth;
	unsigned int mRenderBufferHeight;

	unsigned int mCopyBufferWidth;
	unsigned int mCopyBufferHeight;
	unsigned int mCopyBufferX;
	unsigned int mCopyBufferY;
	Texture* mCopyBufferTexture;

	int mNumDrawBuffers = 0;
	unsigned int mDrawBuffersList[2];

public:
	explicit IFrameBuffer(int screenWidth, int screenHeight);
	virtual ~IFrameBuffer();

	void Init();
	void SetColorTextureAttachment(unsigned int colorAttachmentNumber, Texture* texture);
	void SetDepthTextureAttachment(Texture* texture);
	void SetDepthAttachment(unsigned int width, unsigned int height);
	void SetCopyBufferToTexture(Texture* texture, int x, int y, int imageWidth, int imageHeight);
	void BindBuffer() const;
	void UnbindBuffer() const;
	Texture* CopyDepthBuffer();
	Texture* CopyColorBuffer();

private:
	void CreateBuffer();
	void CopyDepthBufferIntoTexture(Texture* texture, int x, int y, int imageWidth, int imageHeight) const;
	void CopyColorBufferIntoTexture(Texture* texture, int x, int y, int imageWidth, int imageHeight) const;
	void PrintFrameBufferInfo(unsigned int target, unsigned int fbo) const;
};

