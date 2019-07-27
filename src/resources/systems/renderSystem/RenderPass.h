#pragma once
#include <glm/glm.hpp>
#include "../../../utils/serializer/ISerializable.h"
#include <string>

class ICamera;
class IFrameBuffer;
class IMaterial;
class Texture;
class IRenderer;
class RenderSystem;

class RenderPass : public core::utils::ISerializable
{
	int mLayersMask;
	const ICamera* mCamera;
	IFrameBuffer* mFrameBufferOutput;
	bool mIsEnabled;
	bool mIsFogEnabled;
	bool mIsClippingEnabled;
	bool mHasToCalculateDistanceToCamera;
	bool mCanAcceptSpacePartitionRenderersOnly;
	int mClippingPlaneNumber;
	glm::vec4 mClippingPlane;
	IMaterial* mMaterial;
	std::string mName;

public:
	explicit RenderPass(const std::string& name, const ICamera* camera, int layersMask);
	~RenderPass();

	bool HasFrameBufferOutput() const;
	void SetFrameBufferOutput(IFrameBuffer* output);
	void BindOutput() const;
	void UnbindOutput() const;
	Texture* CopyDepthBuffer();

	const std::string GetName() const { return mName; }
	void SetCamera(const ICamera* camera);
	const ICamera* GetCamera() const;
	int GetLayersMask() const;
	
	void SetAcceptSpacePartitionOnly(bool accept);
	bool CanAcceptRenderer(IRenderer* renderer) const;

	bool IsEnabled() const;
	void SetEnable(bool enable);
	
	bool IsFogEnabled() const;
	void EnableFog(bool enable);

	void SetMaterial(IMaterial* material);
	IMaterial* GetMaterial();

	void SetCalculateDistanceToCamera(bool calculate);
	bool HasToCalculateDistanceToCamera() const;

	void EnableClipping(bool enable);
	void SetClippingPlaneNumber(int clippingPlaneNumber);
	bool IsClippingEnabled() const;
	int GetClippingPlaneNumber() const;
	void SetClippingPlane(const glm::vec4& clippingPlane);
	const glm::vec4 GetClippingPlane() const;

	void Build(RenderSystem* renderSystem);

	// Heredado vía ISerializable
	virtual void ReadFrom(core::utils::IDeserializer* source) override;
	virtual void WriteTo(core::utils::ISerializer* destination) override;
private:
	void ReadFrameBuffersFrom(core::utils::IDeserializer* source);
	void ReadFrameBufferFrom(core::utils::IDeserializer* source);
};

