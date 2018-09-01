#pragma once

#include "glm/glm.hpp"
#include <string>
#include <map>
#include <utility>


class AnimateModel;
class Animation;
class Joint;
class KeyFrame;


class Animator
{
public:
	Animator(AnimateModel* entity);
	~Animator();

	void PlayAnimation(Animation* animation);
	void Update(float elapsedTime);

private:
	void IncreaseAnimationTime(float elapsedTime);
	void FillWithCalculatedCurrentAnimationPose(std::map<std::string, const glm::mat4x4>& jointTransforms);
	void ApplyPoseToJoints(std::map<std::string, const glm::mat4x4>& currentPose, Joint* joint, glm::mat4x4& parentTransform);
	std::pair<KeyFrame*, KeyFrame*>& GetPreviousAndNextFrames();
	float CalculateProgression(KeyFrame* previousFrame, KeyFrame* nextFrame) const;
	void FillWithInterpolateFrames(std::map<std::string, const glm::mat4x4>& jointTransforms, KeyFrame* previousFrame, KeyFrame* nextFrame, float lambda) const;

private:
	AnimateModel* mEntity;
	Animation* mCurrentAnimation;
	float mAnimationTime;
};

