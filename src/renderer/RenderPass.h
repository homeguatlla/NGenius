#pragma once
#include <glm/glm.hpp>

class ICamera;
class IFrameBuffer;

class RenderPass
{
	char mLayersMask;
	const ICamera* mCamera;
	const IFrameBuffer* mFrameBufferOutput;
	bool mIsEnabled;
	bool mIsFogEnabled;
	bool mIsClippingEnabled;
	bool mHasToCalculateDistanceToCamera;
	int mClippingPlaneNumber;
	glm::vec4 mClippingPlane;

public:
	explicit RenderPass(const ICamera* camera, char layersMask);
	~RenderPass();

	bool HasFrameBufferOutput() const;
	void SetFrameBufferOutput(const IFrameBuffer* output);
	void BindOutput() const;
	void UnbindOutput() const;
	void CopyBuffer() const;

	const ICamera* GetCamera() const;
	char GetLayersMask() const;
	
	bool IsEnabled() const;
	void SetEnable(bool enable);
	
	bool IsFogEnabled() const;
	void EnableFog(bool enable);

	void SetCalculateDistanceToCamera(bool calculate);
	bool HasToCalculateDistanceToCamera() const;

	void EnableClipping(bool enable);
	void SetClippingPlaneNumber(int clippingPlaneNumber);
	bool IsClippingEnabled() const;
	int GetClippingPlaneNumber() const;
	void SetClippingPlane(const glm::vec4& clippingPlane);
	const glm::vec4 GetClippingPlane() const;
};

