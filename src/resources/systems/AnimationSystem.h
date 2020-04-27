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
	std::vector<std::pair<std::shared_ptr<IGameEntity>, Animator*>> mEntities;
	//std::map<std::string, std::vector<Animation*>> mAnimations;

public:
	AnimationSystem();
	~AnimationSystem();

	void Update(float deltaTime);
	void Reload();

	virtual BaseVisitable<>::ReturnType Accept(BaseVisitor& guest);

private:
	void Release();
	void AddEntity(std::shared_ptr<IGameEntity> entity);
	void RemoveEntity(std::shared_ptr<IGameEntity> entity);
	bool HasAnimationComponents(const std::shared_ptr<IGameEntity> entity) const;

	void SetAnimationData(std::shared_ptr<IGameEntity> entity, const std::vector<glm::mat4x4>& data);

	void OnGameEntityAdded(std::shared_ptr<IGameEntity> entity) override;
	void OnGameEntityRemoved(std::shared_ptr<IGameEntity> entity) override;
};

