#include "stdafx.h"
#include "GameEntityQuadTree.h"
#include "../../GameEntity.h"
#include "../../renderers/IRenderer.h"
#include "../../Transformation.h"

const int GameEntityQuadTree::MAX_QUADTREE_LEVELS = 6;

GameEntityQuadTree::GameEntityQuadTree(const AABB& boundingBox)
{
	glm::vec2 regionMin(boundingBox.GetVertexMin().x, boundingBox.GetVertexMin().z);
	glm::vec2 regionMax(boundingBox.GetVertexMax().x, boundingBox.GetVertexMax().z);
	mQuadTree = new DirectQuadTree<GameEntity>(regionMin, regionMax, MAX_QUADTREE_LEVELS);
}


GameEntityQuadTree::~GameEntityQuadTree()
{
	delete mQuadTree;
}

unsigned int GameEntityQuadTree::GetNumEntities() const
{
	return mQuadTree->GetNumElements();
}

void GameEntityQuadTree::AddGameEntity(GameEntity* entity)
{
	IRenderer* renderer = entity->GetRenderer();
	const AABB boundingBox = renderer->GetAABB();
	glm::vec3 min = boundingBox.GetVertexMin();
	glm::vec3 max = boundingBox.GetVertexMax();

	mQuadTree->Add(	glm::vec2(min.x, min.z), 
					glm::vec2(max.x, max.z), entity);
}

void GameEntityQuadTree::RemoveGameEntity(GameEntity* entity)
{
	IRenderer* renderer = entity->GetRenderer();
	const AABB boundingBox = renderer->GetAABB();
	glm::vec3 min = boundingBox.GetVertexMin();
	glm::vec3 max = boundingBox.GetVertexMax();

	mQuadTree->Remove(	glm::vec2(min.x, min.z),
						glm::vec2(max.x, max.z), entity);
}

void GameEntityQuadTree::Query(const AABB& aabb, std::vector<GameEntity*>& result)
{
	mQuadTree->Query(	glm::vec2(aabb.GetVertexMin().x, aabb.GetVertexMin().z),
						glm::vec2(aabb.GetVertexMax().x, aabb.GetVertexMax().z), result);
}

void GameEntityQuadTree::Query(const AABB& aabb, const Frustum& frustum, std::vector<GameEntity*>& result)
{
	mQuadTree->Query(	glm::vec2(aabb.GetVertexMin().x, aabb.GetVertexMin().z),
						glm::vec2(aabb.GetVertexMax().x, aabb.GetVertexMax().z), frustum, result);
}
