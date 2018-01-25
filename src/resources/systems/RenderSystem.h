#pragma once
#include <vector>
#include <map>
#include "../../VertexBuffersManager.h"
class IRenderer;
class ICamera;
class RenderPass;

class RenderSystem
{
	VertexBuffersManager mVertexsBuffersManager;
	typedef std::vector<IRenderer*> RenderersList;

	std::map<char, RenderersList> mRenderersPerPass;
	std::vector<IRenderer*> mInstances;
	std::vector<const RenderPass*> mRenderPasses;

	int mLastClipPlaneNumberUsed;

public:
	RenderSystem();
	~RenderSystem();
	void Render();
	void AddToRender(IRenderer* renderer);
	void AddRenderPass(const RenderPass* renderPass);

private:
	void Render(const RenderPass* renderPass);
	void UpdateDistancesToCamera(const ICamera* camera, RenderersList* renderers);
	void RenderInstances(const RenderPass* renderPass, IRenderer* renderer, std::vector<IRenderer*>& instances);
};

