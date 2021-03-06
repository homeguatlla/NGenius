#include "stdafx.h"
#include "IFrameBuffer.h"
#include "RenderSystem.h"
#include "../../textures/Texture.h"
#include "../../../utils/serializer/XMLDeserializer.h"
#include "../../../utils/serializer/IDeserializer.h"
#include <GL/glew.h>

IFrameBuffer::IFrameBuffer(int screenWidth, int screenHeight) :
mFrameBufferID(-1),
mDepthBufferID(-1),
mColorTexture(nullptr),
mDepthTexture(nullptr),
mScreenWidth(screenWidth),
mScreenHeight(screenHeight),
mColorAttachmentNumber(0),
mType(FRAME_BUFFER_INVALID),
mRenderBufferWidth(0),
mRenderBufferHeight(0),
mNumDrawBuffers(0),
mCopyBufferHeight(0),
mCopyBufferWidth(0),
mCopyBufferX(0),
mCopyBufferY(0),
mCopyBufferTexture(nullptr),
mIsCopyBuffer(false)
{
	
}

void IFrameBuffer::Init()
{
	CreateBuffer();
}

IFrameBuffer::~IFrameBuffer()
{
	glDeleteFramebuffers(1, &mFrameBufferID);
	glDeleteRenderbuffers(1, &mDepthBufferID);
}

void IFrameBuffer::BindBuffer(RenderSystem* renderSystem) const
{	
	if (mFrameBufferID != -1)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);

		if (mNumDrawBuffers > 0)
		{
			glDrawBuffers(mNumDrawBuffers, mDrawBuffersList);
		}
		else
		{
			glDrawBuffer(GL_NONE);
		}

		renderSystem->ApplyDepthTest(true, true, GL_LESS);
		
		if (mType & FRAME_BUFFER_COLOR_TEXTURE & FRAME_BUFFER_DEPTH_TEXTURE)
		{
			glViewport(0, 0, mDepthTexture->GetWidth(), mDepthTexture->GetHeight());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else if (mType & FRAME_BUFFER_COLOR_TEXTURE)
		{
			glViewport(0, 0, mColorTexture->GetWidth(), mColorTexture->GetHeight());
			glClear(GL_COLOR_BUFFER_BIT);
			mColorTexture->SetActive(true);
		}

		if (mType & FRAME_BUFFER_DEPTH_TEXTURE)
		{
			glViewport(0, 0, mDepthTexture->GetWidth(), mDepthTexture->GetHeight());
			glClear(GL_DEPTH_BUFFER_BIT);
			mDepthTexture->SetActive(true);
		}

		if (mType & FRAME_BUFFER_DEPTH)
		{
			glViewport(0, 0, mRenderBufferWidth, mRenderBufferHeight);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}
}

void IFrameBuffer::UnbindBuffer() const
{
	if (mFrameBufferID != -1)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);
		glViewport(0, 0, mScreenWidth, mScreenHeight);
	}
}

Texture* IFrameBuffer::CopyDepthBuffer()
{
	if (mCopyBufferTexture != nullptr)
	{
		CopyDepthBufferIntoTexture(mCopyBufferTexture, mCopyBufferX, mCopyBufferY, mCopyBufferWidth, mCopyBufferHeight);
		return mCopyBufferTexture;
	}
	else
	{
		return nullptr;
	}
}

Texture* IFrameBuffer::CopyColorBuffer()
{
	if (mCopyBufferTexture != nullptr)
	{
		CopyColorBufferIntoTexture(mCopyBufferTexture, mCopyBufferX, mCopyBufferY, mCopyBufferWidth, mCopyBufferHeight);
		return mCopyBufferTexture;
	}
	else
	{
		return nullptr;
	}
}

void IFrameBuffer::Build(RenderSystem* renderSystem)
{
	if (!mTextureName.empty())
	{
		if (mIsCopyBuffer)
		{
			mCopyBufferTexture = static_cast<Texture*>(renderSystem->GetTexture(mTextureName));
		}
	}
}

void IFrameBuffer::SetColorTextureAttachment(unsigned int colorAttachmentNumber, Texture* texture)
{
	mColorAttachmentNumber = colorAttachmentNumber;
	mColorTexture = texture;
	mType |= FRAME_BUFFER_COLOR_TEXTURE;
}

void IFrameBuffer::SetDepthTextureAttachment(Texture* texture)
{
	mDepthTexture = texture;
	mType |= FRAME_BUFFER_DEPTH_TEXTURE;
}

void IFrameBuffer::SetDepthAttachment(unsigned int width, unsigned int height)
{
	mType |= FRAME_BUFFER_DEPTH;
	mRenderBufferWidth = width;
	mRenderBufferHeight = height;
}

void IFrameBuffer::CreateBuffer()
{
	if (mType & FRAME_BUFFER_DEPTH)
	{
		glGenRenderbuffers(1, &mDepthBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, mDepthBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, mRenderBufferWidth, mRenderBufferHeight);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}

	glGenFramebuffers(1, &mFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, mFrameBufferID);

	if (mType & FRAME_BUFFER_DEPTH_TEXTURE)
	{
		assert(mDepthTexture != nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture->GetID(), 0);
	}

	if (mType & FRAME_BUFFER_COLOR_TEXTURE)
	{
		assert(mColorTexture != nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + mColorAttachmentNumber, GL_TEXTURE_2D, mColorTexture->GetID(), 0);
		mDrawBuffersList[mNumDrawBuffers] = GL_COLOR_ATTACHMENT0 + mColorAttachmentNumber;
		mNumDrawBuffers++;
	}
	
	if (mType & FRAME_BUFFER_DEPTH)
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBufferID);
	}
	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	assert(result == GL_FRAMEBUFFER_COMPLETE);

	UnbindBuffer();
}

void IFrameBuffer::SetCopyBufferToTexture(Texture* texture,  int x, int y, int imageWidth, int imageHeight)
{
	mCopyBufferHeight = imageHeight;
	mCopyBufferWidth = imageWidth;
	mCopyBufferX = x;
	mCopyBufferY = y;
	mCopyBufferTexture = texture;
}

void IFrameBuffer::CopyDepthBufferIntoTexture(Texture* texture, int x, int y, int imageWidth, int imageHeight) const
{
	texture->SetActive(true);
	
	glReadBuffer(GL_BACK); // Ensure we are reading from the back buffer.
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, x, y, imageWidth, imageHeight, 0);
}

void IFrameBuffer::CopyColorBufferIntoTexture(Texture* texture, int x, int y, int imageWidth, int imageHeight) const
{
	texture->SetActive(true);

	glReadBuffer(GL_BACK); // Ensure we are reading from the back buffer.
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, imageWidth, imageHeight, 0);
}

void IFrameBuffer::PrintFrameBufferInfo(unsigned int target, unsigned int fbo) const
{

	int res, i = 0;
	GLint buffer;

	glBindFramebuffer(target, fbo);

	do {
		glGetIntegerv(GL_DRAW_BUFFER0 + i, &buffer);

		if (buffer != GL_NONE) {

			printf("Shader Output Location %d - color attachment %d", i, buffer - GL_COLOR_ATTACHMENT0);

			glGetFramebufferAttachmentParameteriv(target, buffer, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &res);
			printf("\tAttachment Type: %s",	res == GL_TEXTURE ? "Texture" : "Render Buffer");
			glGetFramebufferAttachmentParameteriv(target, buffer, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &res);
			printf("\tAttachment object name: %d\n", res);
		}
		++i;

	} while (buffer != GL_NONE);
}

void IFrameBuffer::ReadFrom(core::utils::IDeserializer* source)
{
	source->ReadParameter("copy_buffer_to_texture", &mIsCopyBuffer);
	source->ReadParameter("texture", mTextureName);

	if (mIsCopyBuffer)
	{
		source->ReadParameter("copy_x", &mCopyBufferX);
		source->ReadParameter("copy_y", &mCopyBufferY);
		source->ReadParameter("copy_width", &mCopyBufferWidth);
		source->ReadParameter("copy_height", &mCopyBufferHeight);
	}
}

void IFrameBuffer::WriteTo(core::utils::ISerializer* destination)
{
}
