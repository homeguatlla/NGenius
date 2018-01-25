#include "stdafx.h"
#include "RenderSystem.h"
#include "../GameEntity.h"
#include "../camera/ICamera.h"
#include "../../renderer/IRenderer.h"
#include "../../renderer/RenderPass.h"
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../../BitNumber.h"
#include <glm/gtx/norm.hpp>

using namespace std;

RenderSystem::RenderSystem() :
mLastClipPlaneNumberUsed(0)
{
	BitNumber bit;
	bit.Test();
}


RenderSystem::~RenderSystem()
{
	for (const RenderPass* pass : mRenderPasses)
	{
		delete pass;
	}
}

void RenderSystem::Render()
{
	for (const RenderPass* pass : mRenderPasses)
	{
		if (pass->IsEnabled())
		{
			if (pass->HasFrameBufferOutput())
			{
				pass->BindOutput();
				Render(pass);
				pass->UnbindOutput();
				pass->CopyBuffer();
			}
			else
			{
				Render(pass);
			}
		}
	}
}

void RenderSystem::Render(const RenderPass* renderPass)
{
	RenderersList renderers = mRenderersPerPass[renderPass->GetLayersMask()];

	//STEP 1 CALUCLATE DISTANCE TO CAMERA FOR EACH RENDERER
	if (renderPass->HasToCalculateDistanceToCamera())
	{
		UpdateDistancesToCamera(renderPass->GetCamera(), &renderers);
	}

	//STEP 2 ORDER RENDERERS FOLLOWING THE BITRENDERERINFORMATION
	//double time = glfwGetTime();
	sort(renderers.begin(), renderers.end(), [](IRenderer* a, IRenderer* b)
	{
		assert(a != nullptr && b != nullptr);
		return a->GetBitRendererInformation().GetValue() > b->GetBitRendererInformation().GetValue();
	});
	
	//cout << "sort time: " << (glfwGetTime() - time) * 1000.0f << "\n";

	//STEP 3 JOIN ALL RENDERERS WITH THE SAME BITRENDERINFORMATION TO USE INSTANCING
	unsigned int i = 0;
	while (i < renderers.size())
	{
		mInstances.clear();
		if (renderers[i]->IsVisible())
		{
			mInstances.push_back(renderers[i]);
			//std::cout << mInstances.front()->GetBitRendererInformation().GetValue() << " : " << mInstances.front()->GetBitRendererInformation().GetString() << "\n";
			++i;

			while (i < renderers.size() &&
				mInstances.front()->IsInstancingAllowed() &&
				mInstances.front()->GetBitRendererInformation().GetValueWithoutDistance() == renderers[i]->GetBitRendererInformation().GetValueWithoutDistance())
			{
				if (renderers[i]->IsVisible())
				{
					if (renderers[i]->IsPrerendered())
					{
						mInstances.insert(mInstances.begin(), renderers[i]);
					}
					else
					{
						mInstances.push_back(renderers[i]);
					}
				}
				++i;
			}
			RenderInstances(renderPass, mInstances[0], mInstances);
		}
		else
		{
			++i;
		}
	}

	mInstances.clear();
	mRenderersPerPass[renderPass->GetLayersMask()].clear();
}

void RenderSystem::AddRenderPass(const RenderPass* renderPass)
{
	mRenderPasses.push_back(renderPass);
}

void RenderSystem::AddToRender(IRenderer* renderer)
{
	//assert(renderer != nullptr);
	if (renderer != nullptr)
	{
		char layer = renderer->GetLayer();
		for (const RenderPass* pass : mRenderPasses)
		{
			char result = pass->GetLayersMask() & layer;
			if ( result != 0)
			{
				mRenderersPerPass[pass->GetLayersMask()].push_back(renderer);
			}
		}
	}
}

void RenderSystem::RenderInstances(const RenderPass* renderPass, IRenderer* renderer, std::vector<IRenderer*>& instances)
{
	//std::cout << instances.front()->GetBitRendererInformation().GetValue() << " : " << instances.size() << "\n";
	//Apply fog
	if (renderer->HasFog())
	{
		renderer->EnableFog(renderPass->IsFogEnabled());
	}

	//Apply clipping planes
	if (renderPass->IsClippingEnabled())
	{
		mLastClipPlaneNumberUsed = renderPass->GetClippingPlaneNumber();
		glEnable(mLastClipPlaneNumberUsed);
		if (renderer->HasClippingPlane())
		{
			renderer->SetClippingPlane(renderPass->GetClippingPlane());
		}
	}
	else
	{
		glDisable(mLastClipPlaneNumberUsed);
	}

	if (instances.size() > 1)
	{
		renderer->SetInstances(instances);
		renderer->EnableInstancing(true);
	}

	renderer->Render(renderPass->GetCamera(), mVertexsBuffersManager, renderPass->GetShader());
}

void RenderSystem::UpdateDistancesToCamera(const ICamera* camera, RenderersList* renderers)
{
	glm::vec3 cameraPosition = camera->GetPosition();
	for (IRenderer* renderer : *renderers)
	{
		glm::vec3 position = renderer->GetParent()->GetTransformation()->GetPosition();
		unsigned int distanceToCamera = glm::length2(cameraPosition - position);
		distanceToCamera = glm::min<unsigned int>(glm::max<unsigned int>(0, distanceToCamera), UINT_MAX);
		renderer->SetDistance(distanceToCamera);
	}
}
