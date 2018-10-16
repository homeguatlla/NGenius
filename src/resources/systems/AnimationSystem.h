#pragma once
#include "../../visitor/BaseVisitable.h"
#include "../scene/IGameSceneListener.h"

#include "glm/glm.hpp"

#include <vector>
#include <map>

class GameEntity;
class Animator;
class Animation;

class AnimationSystem : public BaseVisitable<>, public IGameSceneListener
{
	std::vector<std::pair<GameEntity*, Animator*>> mEntities;
	//std::map<std::string, std::vector<Animation*>> mAnimations;

public:
	AnimationSystem();
	~AnimationSystem();

	void Update(float deltaTime);

	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

private:
	void AddEntity(GameEntity* entity);
	void RemoveEntity(GameEntity* entity);
	bool HasAnimationComponents(const GameEntity* entity) const;

	void SetAnimationData(GameEntity* entity, const std::vector<glm::mat4x4>& data);

	void OnGameEntityAdded(GameEntity* entity) override;
	void OnGameEntityRemoved(GameEntity* entity) override;
};
