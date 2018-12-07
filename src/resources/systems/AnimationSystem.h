#pragma once
#include "ISystem.h"

#include "glm/glm.hpp"

#include <vector>
#include <map>

class GameEntity;
class Animator;
class Animation;

class AnimationSystem : public ISystem
{
	std::vector<std::pair<GameEntity*, Animator*>> mEntities;
	//std::map<std::string, std::vector<Animation*>> mAnimations;

public:
	AnimationSystem();
	~AnimationSystem();

	void Update(float deltaTime) override;
	bool HasToBeRegisteredToGameScene() const override;
	
private:
	void AddEntity(GameEntity* entity) override;
	bool HasSpecificComponents(const GameEntity* entity) const override;

	void SetAnimationData(GameEntity* entity, const std::vector<glm::mat4x4>& data);

	// Heredado vía ISystem
	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor & guest) override;
};

