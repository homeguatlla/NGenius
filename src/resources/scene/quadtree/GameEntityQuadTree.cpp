#include "stdafx.h"
#include "GameEntityQuadTree.h"
#include "../../GameEntity.h"
#include "../../renderers/IRenderer.h"
#include "../../Transformation.h"

const int MAX_QUADTREE_LEVELS = 3;

GameEntityQuadTree::GameEntityQuadTree(const AABB& boundingBox)
{
	glm::vec2 regionMin(boundingBox.GetVertexMin().x, boundingBox.GetVertexMin().z);
	glm::vec2 regionMax(boundingBox.GetVertexMax().x, boundingBox.GetVertexMax().z);
	mQuadTree = new QuadTree<GameEntity*>(regionMin, regionMax, MAX_QUADTREE_LEVELS);
}


GameEntityQuadTree::~GameEntityQuadTree()
{
	delete mQuadTree;
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
	//mQuadTree->Remove(entity);
}

std::vector<GameEntity*>& GameEntityQuadTree::Query(const AABB& aabb)
{
	return mQuadTree->Query(glm::vec2(aabb.GetVertexMin().x, aabb.GetVertexMin().z),
							glm::vec2(aabb.GetVertexMax().x, aabb.GetVertexMax().z));
}