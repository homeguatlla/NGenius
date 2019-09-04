#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../../BitNumber.h"
#include "../../VertexBuffersManager.h"
#include "../../AABB.h"
#include "../Transformation.h"

class ICamera;
class IGameEntity;
class Texture;
class Model;
class IMaterial;

class IRenderer
{
public:
	enum RENDER_LAYER
	{
		LAYER_GUI = 1,
		LAYER_PARTICLES = 2,
		LAYER_TRANSPARENT = 4,
		LAYER_OTHER = 8,
		LAYER_WATER = 16,
		LAYER_TERRAIN = 32,
		LAYER_REFLEXION = 64,

		LAYER_DEBUG = 128
	};

protected:
	IGameEntity* mParent;
	Model* mModel;
	std::vector<IRenderer*> mInstances;
	IMaterial* mMaterial;
	Transformation mTransformation;
	bool mHasTransformation;
	bool mIsPrerendered;
	bool mIsInstancingEnabled;
	
	int mLayer;

	BitNumber mBitRenderInformation;

	bool mIsVisible;
	bool mIsBillboard;
	bool mIsCullingEnabled;

	//virtual void PreRender(VertexBuffersManager& vertexBufferManager) = 0;
	virtual void Draw();

	void CheckError();

public:
	explicit IRenderer(Model* model, IMaterial* material);
	virtual ~IRenderer();

	//virtual const std::string GetName() const = 0;

	void SetParent(IGameEntity* parent);
	IGameEntity* GetParent();

	IMaterial* GetMaterial();
	const IMaterial* GetMaterial() const;
	void SetMaterial(IMaterial* material);

	Model* GetModel();
	void SetModel(Model* model);

	bool HasTransformation() const;
	void SetTransformation(Transformation& transformation);
	Transformation GetTransformation() const;

	void Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager, IMaterial* material);
	
	const BitNumber& GetBitRendererInformation() const;

	bool IsPrerendered() const;

	void SetLayer(int layer);
	int GetLayer() const;

	void SetTransparency(bool transparent);
	void SetDistance(unsigned int distance);

	bool IsVisible() const;
	void SetVisibility(bool visible);

	virtual bool IsInstancingAllowed() const = 0;
	void EnableInstancing(bool enable);
	void SetInstances(std::vector<IRenderer*>& instances);
	
	void SetBillboard(bool billboard);

	bool IsCullingEnabled() const;
	void SetCullingEnabled(bool culling);

	const AABB& GetModelAABB() const;
	const AABB GetAABB() const;

	virtual unsigned int GetNumberTrianglesRendered() const;

	virtual IRenderer* Clone() const;

protected:
	virtual IRenderer* DoClone() const = 0;
private:
	void ModifyModelMatrixToAvoidRotations(const glm::mat4& viewMatrix, const glm::vec3& scale, float angleZ, glm::mat4& modelMatrix);
};

