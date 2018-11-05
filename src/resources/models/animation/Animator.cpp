#include "stdafx.h"
#include "Animator.h"
#include "Animation.h"
#include "AnimatedModel.h"
#include "Joint.h"
#include "KeyFrame.h"
#include "JointTransform.h"
#include "Joint.h"

#include <iostream>

Animator::Animator(AnimatedModel* model) :
	mModel(model),
	mAnimationTime(0.0f)
{
}


Animator::~Animator()
{
}

bool Animator::HasAnimation() const
{
	return mCurrentAnimation != nullptr;
}

void Animator::BindAnimation(Animation* animation)
{
	if (mCurrentAnimation == nullptr || animation->GetName() != mCurrentAnimation->GetName())
	{
		mAnimationTime = 0.0f;
		mCurrentAnimation = animation;
	}
}

void Animator::ReleaseAnimation()
{
	mCurrentAnimation = nullptr;
}

void Animator::Update(float elapsedTime)
{
	if (HasAnimation())
	{
		IncreaseAnimationTime(elapsedTime);
		std::map<std::string, const glm::mat4x4> currentPose;
		FillWithCalculatedCurrentAnimationPose(currentPose);
		ApplyPoseToJoints(currentPose, mModel->GetRootJoint(), glm::mat4x4(1.0f));

		mJointTransforms.clear();
		mJointTransforms.resize(currentPose.size(), glm::mat4x4());

		mModel->FillWithJointTransforms(mJointTransforms);
	}
	else if(!mJointTransforms.empty())
	{
		mJointTransforms.clear();
	}
}

const std::vector <glm::mat4>& Animator::GetJointTransforms() const
{
	return mJointTransforms;
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
	//std::cout << "From frame: " << previousFrame->GetTimestamp() << " to frame: " << nextFrame->GetTimestamp() << "\n";
	return std::pair<KeyFrame*, KeyFrame*>(previousFrame, nextFrame);
}

float Animator::CalculateProgression(KeyFrame* previousFrame, KeyFrame* nextFrame) const
{
	float totalTime = nextFrame->GetTimestamp() - previousFrame->GetTimestamp();
	float currentTime = glm::max(mAnimationTime - previousFrame->GetTimestamp(), 0.0f);
	return currentTime / totalTime;
}

void Animator::FillWithInterpolateFrames(std::map<std::string, const glm::mat4x4>& jointTransforms, KeyFrame* previousFrame, KeyFrame* nextFrame, float lambda) const
{
	assert(previousFrame != nullptr);
	assert(nextFrame != nullptr);

	jointTransforms.clear();
	std::map<std::string, JointTransform*>& previousFrameTransforms = previousFrame->GetJointKeyframes();
	std::map<std::string, JointTransform*>& nextFrameTransforms = nextFrame->GetJointKeyframes();

	std::map<std::string, JointTransform*>::iterator it;
	for (it = previousFrameTransforms.begin(); it != previousFrameTransforms.end(); ++it)
	{
		JointTransform* previousTransform = previousFrameTransforms[it->first];
		JointTransform* nextTransform = nextFrameTransforms[it->first];
		if (previousTransform != nullptr && nextTransform != nullptr)
		{
			JointTransform currentTransform = JointTransform::Interpolate(previousTransform, nextTransform, lambda);
			jointTransforms.insert(std::pair<std::string, const glm::mat4x4>(it->first, currentTransform.GetLocalTransform()));
		}
	}
}