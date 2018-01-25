#pragma once
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include "../../VertexBuffersManager.h"
class IRenderer;
class ICamera;
class RenderPass;
class Texture;

class RenderSystem
{
	VertexBuffersManager mVertexsBuffersManager;
	typedef std::vector<IRenderer*> RenderersList;

	std::map<char, RenderersList> mRenderersPerPass;
	std::vector<IRenderer*> mInstances;
	std::vector<const RenderPass*> mRenderPasses;
	glm::mat4 mShadowMapMatrix;
	const Texture* mShadowMapTexture;

	int mLastClipPlaneNumberUsed;

public:
	RenderSystem();
	~RenderSystem();
	void Render();
	void AddToRender(IRenderer* renderer);
	void AddRenderPass(const RenderPass* renderPass);
	void SetCameraCastingShadows(const ICamera* camera);
	void SetTextureShadowMap(const Texture* texture);

private:
	void Render(const RenderPass* renderPass);
	void UpdateDistancesToCamera(const ICamera* camera, RenderersList* renderers);
	void RenderInstances(const RenderPass* renderPass, IRenderer* renderer, std::vector<IRenderer*>& instances);
	glm::mat4 CalculateShadowMapMatrix(const ICamera* camera);
};

