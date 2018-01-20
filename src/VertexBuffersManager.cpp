#include "stdafx.h"
#include "VertexBuffersManager.h"

#include <GL/glew.h>

VertexBuffersManager::VertexBuffersManager()
{
}


VertexBuffersManager::~VertexBuffersManager()
{
	for (VAOIterator iterator = mVBOs.begin(); iterator != mVBOs.end(); iterator++)
	{
		unsigned int id = iterator->second;
		glDeleteBuffers(1, &id);
	}

	for (VAOIterator iterator = mVAOs.begin(); iterator != mVAOs.end(); iterator++)
	{
		unsigned int id = iterator->second;
		glDeleteVertexArrays(1, &id);
	}
}

unsigned int VertexBuffersManager::GetVAO(const std::string& key)
{
	if (mVAOs.find(key) != mVAOs.end())
	{
		return mVAOs[key];
	}
	else
	{
		return 0u;
	}
}

void VertexBuffersManager::RemoveVAO(const std::string& key)
{
	if (HasVBO(key))
	{
		glDeleteBuffers(1, &mVBOs[key]);
		mVBOs.erase(key);
	}

	if (HasVAO(key))
	{
		glDeleteVertexArrays(1, &mVAOs[key]);
		mVAOs.erase(key);
	}
}

void VertexBuffersManager::RemoveVBO(const std::string& key)
{
	if (HasVBO(key))
	{
		glDeleteBuffers(1, &mVBOs[key]);
		mVBOs.erase(key);
	}
}

bool VertexBuffersManager::HasVAO(const std::string& key) const
{
	return mVAOs.find(key) != mVAOs.end();
}

unsigned int VertexBuffersManager::CreateVAO(const std::string& key)
{
	if (mVAOs.find(key) == mVAOs.end())
	{
		unsigned int vao;
		glGenVertexArrays(1, &vao);
		mVAOs[key] = vao;
	}
	
	return mVAOs[key];
}

unsigned int VertexBuffersManager::GetVBO(const std::string& key)
{
	if (mVBOs.find(key) != mVBOs.end())
	{
		return mVBOs[key];
	}
	else
	{
		return 0u;
	}
}

bool VertexBuffersManager::HasVBO(const std::string& key) const
{
	return mVBOs.find(key) != mVBOs.end();
}

unsigned int VertexBuffersManager::CreateVBO(const std::string& key)
{
	if (mVBOs.find(key) == mVBOs.end())
	{
		unsigned int vbo;
		glGenBuffers(1, &vbo);
		mVBOs[key] = vbo;
	}

	return mVBOs[key];
}