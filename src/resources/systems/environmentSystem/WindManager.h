#pragma once
#include "glm/glm.hpp"
#include <vector>

class WindManager
{
	typedef struct
	{
		glm::vec3 position;
		float force;
	} WindAttractor;

	typedef struct
	{
		glm::vec3 position;
		float force;
	} WindRepeller;

	std::vector<WindAttractor*> mAttractors;
	std::vector<WindRepeller*> mRepellers;

	unsigned int mGridSize;
	float mSpeed;
	float mAccumulatedTime;
	std::vector<std::vector<glm::vec3>> mWindMap;

public:
	WindManager(int gridSize);
	~WindManager();

	void Update(float deltaTime);

private:
	void CreateAttractors(int numAttractors);
	void CreateRepellers(int numRepellers);

	void ReleaseAttractors();
	void ReleaseRepellers();
};

