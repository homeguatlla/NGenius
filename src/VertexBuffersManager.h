#pragma once
#include <string>
#include <map>

class VertexBuffersManager
{
	typedef std::map<const std::string, unsigned int>::iterator VAOIterator;
	std::map<const std::string, unsigned int> mVAOs;
	std::map<const std::string, unsigned int> mVBOs;

public:
	VertexBuffersManager();
	~VertexBuffersManager();

	unsigned int GetVAO(const std::string& key);
	bool HasVAO(const std::string& key) const;
	unsigned int CreateVAO(const std::string& key);

	unsigned int GetVBO(const std::string& key);
	bool HasVBO(const std::string& key) const;
	unsigned int CreateVBO(const std::string& key);

	void RemoveVAO(const std::string& key);
	void RemoveVBO(const std::string& key);
};

