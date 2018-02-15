#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../../BitNumber.h"
#include "../../VertexBuffersManager.h"
#include "../../AABB.h"


class ICamera;
class GameEntity;
class Texture;
class Model;
class IMaterial;

class IRenderer_
{
public:
	enum RENDER_LAYER
	{
		LAYER_GUI = 1,
		LAYER_OTHER = 2,
		LAYER_PARTICLES = 4,
		LAYER_WATER = 8,
		LAYER_TERRAIN = 16,

		LAYER_DEBUG = 32
	};

protected:
	GameEntity* mParent;
	Model* mModel;
	std::vector<IRenderer_*> mInstances;
	IMaterial* mMaterial;

	bool mIsPrerendered;
	bool mIsInstancingEnabled;
	
	char mLayer;

	BitNumber mBitRenderInformation;

	bool mIsVisible;

	//virtual void PreRender(VertexBuffersManager& vertexBufferManager) = 0;
	virtual void Draw() = 0;

	void CheckError();

public:
	explicit IRenderer_(Model* model, IMaterial* material);
	virtual ~IRenderer_();

	virtual const std::string GetName() const = 0;

	void SetParent(GameEntity* parent);
	GameEntity* GetParent();

	IMaterial* GetMaterial();

	void Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager, IMaterial* material);
	
	const BitNumber& GetBitRendererInformation() const;

	bool IsPrerendered() const;

	void SetLayer(char layer);
	char GetLayer() const;

	void SetTransparency(bool transparent);
	void SetDistance(unsigned int distance);

	bool IsVisible() const;
	void SetVisibility(bool visible);

	virtual bool IsInstancingAllowed() const = 0;
	void EnableInstancing(bool enable);
	void SetInstances(std::vector<IRenderer_*> instances);

	const AABB& GetAABB() const;

	virtual IRenderer_* Clone() const;

protected:
	virtual IRenderer_* DoClone() const = 0;
};

