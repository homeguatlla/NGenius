#pragma once
#include "../../visitor/BaseVisitable.h"
#include "../scene/IGameSceneListener.h"

#include "glm/glm.hpp"

#include <vector>
#include <map>

class IGameEntity;
class Animator;
class Animation;

class AnimationSystem : public BaseVisitable<>, public IGameSceneListener
{
	std::vector<std::pair<IGameEntity*, Animator*>> mEntities;
	//std::map<std::string, std::vector<Animation*>> mAnimations;

public:
	AnimationSystem();
	~AnimationSystem();

	void Update(float deltaTime);

	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

private:
	void AddEntity(IGameEntity* entity);
	void RemoveEntity(IGameEntity* entity);
	bool HasAnimationComponents(const IGameEntity* entity) const;

	void SetAnimationData(IGameEntity* entity, const std::vector<glm::mat4x4>& data);

	void OnGameEntityAdded(IGameEntity* entity) override;
	void OnGameEntityRemoved(IGameEntity* entity) override;
};

