#include "stdafx.h"
#include "RenderPass.h"
#include "IFrameBuffer.h"

RenderPass::RenderPass(const ICamera* camera, char layersMask) :
mLayersMask(layersMask),
mCamera(camera),
mFrameBufferOutput(nullptr),
mIsEnabled(true),
mIsFogEnabled(true),
mIsClippingEnabled(false),
mHasToCalculateDistanceToCamera(false),
mClippingPlaneNumber(0),
mClippingPlane(0.0f, 0.0f, 0.0f, 0.0f),
mShader(nullptr)
{
}


RenderPass::~RenderPass()
{
	if (HasFrameBufferOutput())
	{
		delete mFrameBufferOutput;
	}
}

bool RenderPass::HasFrameBufferOutput() const
{
	return mFrameBufferOutput != nullptr;
}

void RenderPass::SetFrameBufferOutput(const IFrameBuffer* output)
{
	mFrameBufferOutput = output;
}

void RenderPass::BindOutput() const
{
	mFrameBufferOutput->BindBuffer();
}

void RenderPass::UnbindOutput() const
{
	mFrameBufferOutput->UnbindBuffer();
}

void RenderPass::CopyBuffer() const
{
	mFrameBufferOutput->CopyBuffer();
}

const ICamera* RenderPass::GetCamera() const
{
	return mCamera;
}

char RenderPass::GetLayersMask() const
{
	return mLayersMask;
}

bool RenderPass::IsEnabled() const
{
	return mIsEnabled;
}

void RenderPass::SetEnable(bool enable)
{
	mIsEnabled = enable;
}

bool RenderPass::IsFogEnabled() const
{
	return mIsFogEnabled;
}

void RenderPass::EnableFog(bool enable)
{
	mIsFogEnabled = enable;
}

void RenderPass::SetShader(IShaderProgram* shader)
{
	mShader = shader;
}

IShaderProgram* RenderPass::GetShader() const
{
	return mShader;
}

void RenderPass::SetCalculateDistanceToCamera(bool calculate)
{
	mHasToCalculateDistanceToCamera = calculate;
}

bool RenderPass::HasToCalculateDistanceToCamera() const
{
	return mHasToCalculateDistanceToCamera;
}

void RenderPass::EnableClipping(bool enable)
{
	mIsClippingEnabled = enable;
}

void RenderPass::SetClippingPlaneNumber(int clippingPlaneNumber)
{
	mClippingPlaneNumber = clippingPlaneNumber;
}

bool RenderPass::IsClippingEnabled() const
{
	return mIsClippingEnabled;
}

int RenderPass::GetClippingPlaneNumber() const
{
	return mClippingPlaneNumber;
}

void RenderPass::SetClippingPlane(const glm::vec4& clippingPlane)
{
	mClippingPlane = clippingPlane;
}

const glm::vec4 RenderPass::GetClippingPlane() const
{
	return mClippingPlane;
}