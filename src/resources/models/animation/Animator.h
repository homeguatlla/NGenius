#pragma once

#include "glm/glm.hpp"
#include <string>
#include <map>
#include <utility>
#include <vector>


class AnimatedModel;
class Animation;
class Joint;
class KeyFrame;


class Animator
{
public:
	Animator(AnimatedModel* model);
	~Animator();

	void PlayAnimation(Animation* animation);
	void Update(float elapsedTime);
	const std::vector <glm::mat4>& GetJointTransforms() const;

private:
	void IncreaseAnimationTime(float elapsedTime);
	void FillWithCalculatedCurrentAnimationPose(std::map<std::string, const glm::mat4x4>& jointTransforms);
	void ApplyPoseToJoints(std::map<std::string, const glm::mat4x4>& currentPose, Joint* joint, glm::mat4x4& parentTransform);
	std::pair<KeyFrame*, KeyFrame*> GetPreviousAndNextFrames();
	float CalculateProgression(KeyFrame* previousFrame, KeyFrame* nextFrame) const;
	void FillWithInterpolateFrames(std::map<std::string, const glm::mat4x4>& jointTransforms, KeyFrame* previousFrame, KeyFrame* nextFrame, float lambda) const;

private:
	AnimatedModel* mModel;
	Animation* mCurrentAnimation;
	float mAnimationTime;
	std::vector<glm::mat4> mJointTransforms;
};

