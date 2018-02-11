#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../BitNumber.h"
#include "../AABB.h"
#include "../VertexBuffersManager.h"

class IShaderProgram;
class ICamera;
class GameEntity;
class Texture;
class Model;

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
	IShaderProgram* mShaderProgram;

	bool mIsPrerendered;
	bool mIsInstancingEnabled;
	bool mIsUsingShaderRenderPass;

	char mLayer;

	unsigned int mVAO;
	unsigned int mVertexVBO;
	unsigned int mIndexVBO;
	BitNumber mBitRenderInformation;
	AABB mBoundingBox;

	float mFogDensity;
	float mFogGradient;
	glm::vec3 mFogColor;
	bool mIsFogEnabled;
	bool mIsVisible;

	float mTile;

	virtual void PreRender(VertexBuffersManager& vertexBufferManager) = 0;
	virtual void Draw() = 0;
	virtual void LoadData(const ICamera* camera, VertexBuffersManager& vertexBufferManager) = 0;
	virtual int GetRenderShaderPassTextureUnit() const = 0;
	void LoadDataQuadShader(const ICamera* camera, VertexBuffersManager& vertexBufferManager, int textureUnit);

	long GetNumberIndexes() const;
	long GetNumberVertexs() const;
	long GetNumberTextureCoords() const;
	long GetNumberInstances() const;

	void CheckError();

public:
	explicit IRenderer(IShaderProgram* shader);
	virtual ~IRenderer();

	virtual const std::string GetName() const = 0;

	void SetParent(GameEntity* parent);
	GameEntity* GetParent();

	virtual void Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager) = 0;
	void Render(const ICamera* camera, VertexBuffersManager& vertexBufferManager, IShaderProgram* shaderRenderPass);
	
	void SetModel(Model* model);
	const BitNumber& GetBitRendererInformation() const;

	bool IsPrerendered() const;

	void SetLayer(char layer);
	char IRenderer::GetLayer() const;

	void SetTransparency(bool transparent);
	void SetDistance(unsigned int distance);

	bool IsVisible() const;
	void SetVisibility(bool visible);

	virtual bool HasFog() const = 0;
	virtual void EnableFog(bool enable) {};

	virtual bool HasClippingPlane() const = 0;
	virtual void SetClippingPlane(const glm::vec4& plane) {};

	void SetTile(float tile);

	virtual bool IsInstancingAllowed() const = 0;
	void EnableInstancing(bool enable);
	void SetInstances(std::vector<IRenderer*> instances);

	const AABB& GetBoundingBox() const;

	void SetFogParameters(const glm::vec3& color, float density, float gradient);

	virtual bool IsCastingShadows() const = 0;
	virtual void SetShadowMapParameters(const Texture* shadowMap, const glm::mat4& matrix, int pfcCounter) {};
	//virtual void SetShadowMapMatrix(const glm::mat4& shadowMatrix) {};
	//virtual void SetTextureShadowMap(const Texture* shadowMap) {};

	//TODO esto no mola, se usa en el instancing
	glm::mat4 GetModelMatrix();

	virtual IRenderer* Clone() const;

protected:
	virtual IRenderer* DoClone() const = 0;

private:
	void CalculateBoundingBox();
};

