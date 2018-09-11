#include "stdafx.h"
#include "Animator.h"
#include "Animation.h"
#include "AnimatedModel.h"
#include "Joint.h"
#include "KeyFrame.h"
#include "JointTransform.h"
#include "Joint.h"



Animator::Animator(AnimatedModel* entity) :
	mEntity(entity),
	mAnimationTime(0.0f)
{
}


Animator::~Animator()
{
}

void Animator::PlayAnimation(Animation* animation)
{
	mAnimationTime = 0.0f;
	mCurrentAnimation = animation;
}

void Animator::Update(float elapsedTime)
{
	if (mCurrentAnimation != nullptr)
	{
		IncreaseAnimationTime(elapsedTime);
		std::map<std::string, const glm::mat4x4> currentPose;
		FillWithCalculatedCurrentAnimationPose(currentPose);
		ApplyPoseToJoints(currentPose, mEntity->GetRootJoint(), glm::mat4x4(1.0f));
	}
}

void Animator::IncreaseAnimationTime(float elapsedTime)
{
	mAnimationTime += elapsedTime;

	float duration = mCurrentAnimation->GetDuration(); 
	if (mAnimationTime > duration)
	{
		mAnimationTime = glm::mod(mAnimationTime, duration);
	}
}

void Animator::FillWithCalculatedCurrentAnimationPose(std::map<std::string, const glm::mat4x4>& jointTransforms)
{
	std::pair<KeyFrame*, KeyFrame*> frames = GetPreviousAndNextFrames();
	float progression = CalculateProgression(frames.first, frames.second);
	
	FillWithInterpolateFrames(jointTransforms, frames.first, frames.second, progression);
}

void Animator::ApplyPoseToJoints(std::map<std::string, const glm::mat4x4>& currentPose, Joint* joint, glm::mat4x4& parentTransform)
{
	glm::mat4x4 currentLocalTransform = currentPose[joint->GetName()];
	glm::mat4x4 currentTransform = parentTransform * currentLocalTransform;
	const std::vector<Joint*>& joints = joint->GetJoints();
	for (Joint* child : joints)
	{
		ApplyPoseToJoints(currentPose, child, currentTransform);
	}
	currentTransform = currentTransform * joint->GetInverseBindTransform();
	joint->SetAnimatedTransform(currentTransform);
}

std::pair<KeyFrame*, KeyFrame*> Animator::GetPreviousAndNextFrames()
{
	std::vector<KeyFrame*> frames = mCurrentAnimation->GetKeyframes();
	KeyFrame* previousFrame = frames[0];
	KeyFrame* nextFrame = frames[0];

	//TODO optimización, nos podemos guardar el current keyframe quizá el total y así empezar directamente 
	//en ese en lugar que buscar desde el principio
	for (unsigned int i = 1; i < frames.size(); ++i)
	{
		nextFrame = frames[i];
		if (nextFrame->GetTimestamp() > mAnimationTime)
		{
			break;
		}
		previousFrame = nextFrame;
	}

	return std::pair<KeyFrame*, KeyFrame*>(previousFrame, nextFrame);
}

float Animator::CalculateProgression(KeyFrame* previousFrame, KeyFrame* nextFrame) const
{
	float totalTime = nextFrame->GetTimestamp() - previousFrame->GetTimestamp();
	float currentTime = mAnimationTime - previousFrame->GetTimestamp();
	return currentTime / totalTime;
}

void Animator::FillWithInterpolateFrames(std::map<std::string, const glm::mat4x4>& jointTransforms, KeyFrame* previousFrame, KeyFrame* nextFrame, float lambda) const
{
	jointTransforms.clear();
	std::map<std::string, JointTransform*>& previousFrameTransforms = previousFrame->GetJointKeyframes();
	std::map<std::string, JointTransform*>& nextFrameTransforms = nextFrame->GetJointKeyframes();

	std::map<std::string, JointTransform*>::iterator it;
	for (it = previousFrameTransforms.begin(); it != previousFrameTransforms.end(); ++it)
	{
		JointTransform* previousTransform = previousFrameTransforms[it->first];
		JointTransform* nextTransform = nextFrameTransforms[it->first];
		JointTransform currentTransform = JointTransform::Interpolate(previousTransform, nextTransform, lambda);
		jointTransforms.insert(std::pair<std::string, const glm::mat4x4>(it->first, currentTransform.GetLocalTransform()));
	}
}