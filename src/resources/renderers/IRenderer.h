#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "src/BitNumber.h"
#include "src/VertexBuffersManager.h"
#include "src/AABB.h"
#include "src/resources/Transformation.h"

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
	std::shared_ptr<IGameEntity> mParent;
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
	bool mIsBlendingEnabled;
	int mBlendingSFactor;
	int mBlendingDFactor;
	bool mIsDepthBufferEnabled;
	int mDepthBufferMask;
	int mDepthBufferFunc;

	//virtual void PreRender(VertexBuffersManager& vertexBufferManager) = 0;
	virtual void Draw();

	void CheckError();

public:
	explicit IRenderer(Model* model, IMaterial* material);
	virtual ~IRenderer();

	//virtual const std::string GetName() const = 0;

	void SetParent(std::shared_ptr<IGameEntity> parent);
	std::shared_ptr<IGameEntity> GetParent();

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

	bool IsBlendingEnabled() const { return mIsBlendingEnabled; }
	void SetBlendingEnabled(bool blending);
	void SetBlendingFactors(int sFactor, int dFactor);
	int GetBlendingSFactor() const { return mBlendingSFactor; }
	int GetBlendingDFactor() const { return mBlendingDFactor; }

	bool IsDepthBufferEnabled() const { return mIsDepthBufferEnabled; }
	void SetDepthBufferEnabled(bool enabled) { mIsDepthBufferEnabled = enabled; }
	void SetDepthBufferValues(int mask, int func);
	int GetDepthBufferMask() const { return mDepthBufferMask; }
	int GetDepthBufferFunc() const { return mDepthBufferFunc; }
	const AABB& GetModelAABB() const;
	const AABB GetAABB() const;

	virtual unsigned int GetNumberTrianglesRendered() const;

	virtual IRenderer* Clone() const;

protected:
	virtual IRenderer* DoClone() const = 0;
private:
	void ModifyModelMatrixToAvoidRotations(const glm::mat4& viewMatrix, const glm::vec3& scale, float angleZ, glm::mat4& modelMatrix);
};

