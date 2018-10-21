#pragma once
#include "glm/glm.hpp"
#include <vector>
#include <string>

class SunLight
{
public:
	typedef struct KeyFrame
	{
		float mHour;//hhmm
		float mRotationAngle;
		mutable glm::vec3 mColor;
		std::string mCubemap;
		//shadow??
		glm::vec3 mFogColor;
		float mFogDensity;
		mutable float mFogGradient;

		KeyFrame(float hour, float angle, const glm::vec3& color,
			const glm::vec3& fogColor, float fogDensity, float fogGradient, const std::string& cubemap) :
			mHour(hour),
			mRotationAngle(angle),
			mColor(color),
			mFogColor(fogColor),
			mFogDensity(fogDensity),
			mFogGradient(fogGradient),
			mCubemap(cubemap)
		{
		}
	} KeyFrame;

private:
	std::vector<KeyFrame*> mKeyFrames;
	float mCurrentHour;
	bool mShouldSort;
	unsigned int mCurrentFrameIndex;
	float mCurrentBlendFactor;

public:
	SunLight();
	~SunLight();

	void AddFrame(	float hour, float rotationAngle, const glm::vec3& color, 
					const glm::vec3& fogColor, float fogDensity, float fogGradient, const std::string& cubemapName);

	void SetDayHour(float hour);
	float GetSunLightBlendFactor() const;
	glm::vec3 GetSunLightDirection() const;
	glm::vec3 GetSunLightColor() const;
	const std::string GetSkyBoxCubemapOrigin() const;
	const std::string GetSkyBoxCubemapDestination() const;
	glm::vec3 GetFogColor() const;
	float GetFogDensity() const;
	float GetFogGradient() const;

	//debug
	void SetCurrentFrameColor(const glm::vec3& color);
	void SetCurrentFrameFogGradient(float gradient);

private:
	const KeyFrame* GetCurrentFrame() const;
	const KeyFrame* GetPreviousFrame() const;
	void CalculateCurrentFrame();
};

