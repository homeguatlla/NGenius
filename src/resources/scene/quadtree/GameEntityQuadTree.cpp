#include "stdafx.h"
#include "GameEntityQuadTree.h"
#include "../../GameEntity.h"
#include "../../renderers/IRenderer.h"

const int MAX_QUADTREE_LEVELS = 8;

GameEntityQuadTree::GameEntityQuadTree(const AABB& boundingBox)
{
	glm::vec2 regionMin(boundingBox.GetVertexMin().x, boundingBox.GetVertexMin().z);
	glm::vec2 regionMax(boundingBox.GetVertexMax().x, boundingBox.GetVertexMax().z);
	mQuadTree = new QuadTree<const GameEntity*>(regionMin, regionMax, MAX_QUADTREE_LEVELS);
}


GameEntityQuadTree::~GameEntityQuadTree()
{
	delete mQuadTree;
}

void GameEntityQuadTree::AddGameEntity(const GameEntity* entity)
{
	IRenderer* renderer = entity->GetRenderer();
	const AABB boundingBox = renderer->GetAABB();
	mQuadTree->Add(	glm::vec2(boundingBox.GetVertexMin().x, boundingBox.GetVertexMin().z), 
					glm::vec2(boundingBox.GetVertexMax().x, boundingBox.GetVertexMax().z), entity);
}

void GameEntityQuadTree::RemoveGameEntity(const GameEntity* entity)
{
	mQuadTree->Remove(entity);
}
