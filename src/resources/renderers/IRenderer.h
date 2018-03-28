#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../../BitNumber.h"
#include "../../VertexBuffersManager.h"
#include "../../AABB.h"
#include "../Transformation.h"


class ICamera;
class GameEntity;
class Texture;
class Model;
class IMaterial;

class IRenderer
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
	std::vector<IRenderer*> mInstances;
	IMaterial* mMaterial;
	Transformation mTransformation;
	bool mHasTransformation;
	bool mIsPrerendered;
	bool mIsInstancingEnabled;
	
	char mLayer;

	BitNumber mBitRenderInformation;

	bool mIsVisible;
	bool mIsBillboard;

	//virtual void PreRender(VertexBuffersManager& vertexBufferManager) = 0;
	virtual void Draw();

	void CheckError();

public:
	explicit IRenderer(Model* model, IMaterial* material);
	virtual ~IRenderer();

	//virtual const std::string GetName() const = 0;

	void SetParent(GameEntity* parent);
	GameEntity* GetParent();

	IMaterial* GetMaterial();
	Model* GetModel();

	bool HasTransformation() const;
	void SetTransformation(Transformation& transformation);
	Transformation GetTransformation() const;

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
	void SetInstances(std::vector<IRenderer*>& instances);
	
	void SetBillboard(bool billboard);

	const AABB& GetAABB() const;

	virtual IRenderer* Clone() const;

protected:
	virtual IRenderer* DoClone() const = 0;
private:
	void ModifyModelMatrixToAvoidRotations(const glm::mat4& viewMatrix, const glm::vec3& scale, float angleZ, glm::mat4& modelMatrix);
};

