#include "stdafx.h"
#include "SunLight.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../../GameConstants.h"

#include <algorithm>

SunLight::SunLight() :
	mCurrentHour(0),
	mShouldSort(false),
	mCurrentFrameIndex(-1),
	mCurrentBlendFactor(0.0f)
{
}


SunLight::~SunLight()
{
}

void SunLight::AddFrame(float hour, const float rotationAngle, const glm::vec3& color,
						const glm::vec3& fogColor, float fogDensity, float fogGradient, const std::string& cubemapName)
{
	bool found = std::find_if(mKeyFrames.begin(), mKeyFrames.end(), [&](KeyFrame* keyFrame) { return keyFrame->mHour == hour; }) != mKeyFrames.end();
	if (!found)
	{
		KeyFrame* keyFrame = new KeyFrame(hour, rotationAngle, color, fogColor, fogDensity, fogGradient, cubemapName);
		mKeyFrames.push_back(keyFrame);
		mShouldSort = true;
	}
}

const SunLight::KeyFrame* SunLight::GetCurrentFrame() const
{
	return mKeyFrames.empty() ? nullptr : mKeyFrames[mCurrentFrameIndex];
}

const SunLight::KeyFrame* SunLight::GetPreviousFrame() const
{
	return mKeyFrames.empty() ? nullptr : mKeyFrames[(mCurrentFrameIndex-1) % mKeyFrames.size()];
}

void SunLight::CalculateCurrentFrame()
{
	if (mShouldSort)
	{
		std::sort(mKeyFrames.begin(), mKeyFrames.end(), [](const KeyFrame* a, const KeyFrame* b) -> bool
		{
			return a->mHour < b->mHour;
		});
		mShouldSort = false;
	}

	mCurrentFrameIndex = 0;

	for (unsigned int frameIndex = 0; frameIndex < mKeyFrames.size(); ++frameIndex)
	{
		KeyFrame* keyFrame = mKeyFrames[frameIndex];
		if (keyFrame->mHour >= mCurrentHour)
		{
			mCurrentFrameIndex = frameIndex;
			break;
		}
	}
}

float SunLight::GetSunLightBlendFactor() const
{
	return mCurrentBlendFactor;
}

void SunLight::SetDayHour(float hour)
{
	mCurrentBlendFactor = 0.0f;
	mCurrentHour = hour * 100.0f;
	assert(mCurrentHour >= 0 && mCurrentHour <= 2400);
	CalculateCurrentFrame();

	const KeyFrame* currentKeyFrame = GetCurrentFrame();
	if (currentKeyFrame != nullptr)
	{
		const KeyFrame* previousFrame = GetPreviousFrame();
		float fromHour = previousFrame->mHour;
		float toHour = currentKeyFrame->mHour;
		if (fromHour > toHour)
		{
			toHour = toHour + 2400.0f - fromHour;
			fromHour = 0;
		}
		mCurrentBlendFactor = (mCurrentHour - fromHour) /(toHour - fromHour);
		mCurrentBlendFactor = glm::clamp(glm::abs<float>(mCurrentBlendFactor), 0.0f, 1.0f);
	}
}

glm::vec3 SunLight::GetSunLightDirection() const
{
	const KeyFrame* currentKeyFrame = GetCurrentFrame();
	if (currentKeyFrame != nullptr)
	{
		const KeyFrame* previousFrame = GetPreviousFrame();
		float fromAngle = previousFrame->mRotationAngle;
		float toAngle = currentKeyFrame->mRotationAngle;
		if (fromAngle > toAngle)
		{
			toAngle += 360.0f;
		}
		float angle = glm::mix(fromAngle, toAngle, mCurrentBlendFactor);
		glm::mat4 matrix(1.0f);
		matrix = glm::rotate(matrix, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 direction = glm::vec4(SUN_POSITION_DEFAULT, 1.0f) * matrix;

		return direction;
	}
	else
	{
		return SUN_POSITION_DEFAULT;
	}
}

glm::vec3 SunLight::GetSunLightColor() const
{
	const KeyFrame* currentKeyFrame = GetCurrentFrame();
	if (currentKeyFrame != nullptr)
	{
		const KeyFrame* previousFrame = GetPreviousFrame();
		return glm::mix(previousFrame->mColor, currentKeyFrame->mColor, mCurrentBlendFactor);
	}
	else
	{
		return glm::vec3(1.0f);
	}
}

glm::vec3 SunLight::GetFogColor() const
{
	const KeyFrame* currentKeyFrame = GetCurrentFrame();
	if (currentKeyFrame != nullptr)
	{
		const KeyFrame* previousFrame = GetPreviousFrame();
		return glm::mix(previousFrame->mFogColor, currentKeyFrame->mFogColor, mCurrentBlendFactor);
	}
	else
	{
		return glm::vec3(0.0f);
	}
}

float SunLight::GetFogDensity() const
{
	const KeyFrame* currentKeyFrame = GetCurrentFrame();
	if (currentKeyFrame != nullptr)
	{
		const KeyFrame* previousFrame = GetPreviousFrame();
		return glm::mix(previousFrame->mFogDensity, currentKeyFrame->mFogDensity, mCurrentBlendFactor);
	}
	else
	{
		return 0.0f;
	}
}

float SunLight::GetFogGradient() const
{
	const KeyFrame* currentKeyFrame = GetCurrentFrame();
	if (currentKeyFrame != nullptr)
	{
		const KeyFrame* previousFrame = GetPreviousFrame();
		return glm::mix(previousFrame->mFogGradient, currentKeyFrame->mFogGradient, mCurrentBlendFactor);
	}
	else
	{
		return 0.0f;
	}
}


const std::string SunLight::GetSkyBoxCubemapOrigin() const
{
	const KeyFrame* previousFrame = GetPreviousFrame();
	if (previousFrame != nullptr)
	{
		return previousFrame->mCubemap;
	}
	else
	{
		return "";
	}
}

const std::string SunLight::GetSkyBoxCubemapDestination() const
{
	const KeyFrame* currentKeyFrame = GetCurrentFrame();
	if (currentKeyFrame != nullptr)
	{
		return currentKeyFrame->mCubemap;
	}
	else
	{
		return "";
	}
}

void SunLight::SetCurrentFrameColor(const glm::vec3& color)
{
	const KeyFrame* currentKeyFrame = GetCurrentFrame();
	if (currentKeyFrame != nullptr)
	{
		currentKeyFrame->mColor = color;
	}
}