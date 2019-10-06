#include "stdafx.h"
#include "RenderPass.h"
#include "IFrameBuffer.h"
#include "../../renderers/IRenderer.h"
#include "../../IGameEntity.h"
#include "../../components/SpacePartitionComponent.h"
#include "../../../utils/serializer/XMLDeserializer.h"
#include "../Memory.h"

RenderPass::RenderPass(const std::string& name, const ICamera* camera, int layersMask) :
mLayersMask(layersMask),
mCamera(camera),
mFrameBufferOutput(nullptr),
mIsEnabled(true),
mIsFogEnabled(false),
mIsClippingEnabled(false),
mHasToCalculateDistanceToCamera(false),
mCanAcceptSpacePartitionRenderersOnly(false),
mClippingPlaneNumber(0),
mClippingPlane(0.0f, 0.0f, 0.0f, 0.0f),
mMaterial(nullptr),
mName(name)
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

void RenderPass::SetFrameBufferOutput(IFrameBuffer* output)
{
	mFrameBufferOutput = output;
}

void RenderPass::BindOutput(RenderSystem* renderSystem) const
{
	mFrameBufferOutput->BindBuffer(renderSystem);
}

void RenderPass::UnbindOutput() const
{
	mFrameBufferOutput->UnbindBuffer();
}

Texture* RenderPass::CopyDepthBuffer()
{
	return mFrameBufferOutput->CopyDepthBuffer();
}

void RenderPass::SetCamera(const ICamera* camera)
{
	mCamera = camera;
}

const ICamera* RenderPass::GetCamera() const
{
	return mCamera;
}

int RenderPass::GetLayersMask() const
{
	return mLayersMask;
}

void RenderPass::SetAcceptSpacePartitionOnly(bool accept)
{
	mCanAcceptSpacePartitionRenderersOnly = accept;
}

bool RenderPass::CanAcceptRenderer(IRenderer* renderer) const
{
	if (mCanAcceptSpacePartitionRenderersOnly)
	{
		const SpacePartitionComponent* spacePartitionComponent = renderer->GetParent()->GetComponent<SpacePartitionComponent>();
		if (spacePartitionComponent != nullptr && spacePartitionComponent->IsEnabled())
		{
			if (!spacePartitionComponent->IsVisible())
			{
				return false;
			}
		}
	}
	
	const char layer = renderer->GetLayer();
	char result = GetLayersMask() & layer;
	if (result != 0)
	{
		return true;
	}

	return false;
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

void RenderPass::SetMaterial(IMaterial* material)
{
	mMaterial = material;
}

IMaterial* RenderPass::GetMaterial()
{
	return mMaterial;
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

void RenderPass::Build(RenderSystem* renderSystem)
{
	if (mFrameBufferOutput != nullptr)
	{
		mFrameBufferOutput->Build(renderSystem);
	}
}

void RenderPass::ReadFrom(core::utils::IDeserializer* source)
{
	source->ReadParameter("is_enabled", &mIsEnabled);
	source->ReadParameter("is_fog_enabled", &mIsFogEnabled);
	source->ReadParameter("is_clipping_enabled", &mIsClippingEnabled);
	source->ReadParameter("clipping_plane_number", &mClippingPlaneNumber);
	source->ReadParameter("has_to_calculate_distance_to_camera", &mHasToCalculateDistanceToCamera);
	source->ReadParameter("can_accept_space_partition_renderers_only", &mCanAcceptSpacePartitionRenderersOnly);
	ReadFrameBuffersFrom(source);	
}

void RenderPass::ReadFrameBuffersFrom(core::utils::IDeserializer* source)
{
	if (source->HasAttribute(std::string("frame_buffers")))
	{
		source->BeginAttribute(std::string("frame_buffers"));
		unsigned int numElements = source->ReadNumberOfElements();

		source->BeginAttribute(std::string("frame_buffer"));
		do
		{
			ReadFrameBufferFrom(source);

			source->NextAttribute();
			numElements--;

		} while (numElements > 0);

		source->EndAttribute();
		source->EndAttribute();
	}
}

void RenderPass::ReadFrameBufferFrom(core::utils::IDeserializer* source)
{
	float width = 0.0f, height = 0.0f;
	source->ReadParameter("width", &width);
	source->ReadParameter("height", &height);

	/*IFrameBuffer* buffer = DBG_NEW IFrameBuffer(static_cast<int>(width), static_cast<int>(height));
	buffer->ReadFrom(source);
	SetFrameBufferOutput(buffer);*/
}

void RenderPass::WriteTo(core::utils::ISerializer* destination)
{
}
