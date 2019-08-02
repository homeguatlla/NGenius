#pragma once
#include <string>

static const float PLAYER_UPWARDS_HEIGHT = 2.0f;
static const float PLAYER_RUN_SPEED = 3.0f;
static const float PLAYER_TURN_SPEED = 0.2f;
static const float PLAYER_ZOOM_SPEED = 10.0f;
static const float PLAYER_PITCH = 10.0f;
static const float PLAYER_PITCH_SPEED = 10.0f;
static const float VIEW_ANGLE = 45.0f;
static const float FAR_PLANE = 100.0f;
static const float NEAR_PLANE = 0.1f;
static const float SCREEN_WIDTH = 1920.0f;// 1024.0f;
static const float SCREEN_HEIGHT = 1080.0f;// 768.0f;

static const float SKYBOX_ROTATION_SPEED = 0.01f;
static const float MIN_FPS_ALLOWED = 30.0f;
static const float TERRAIN_SCALE = 10.0f;

static const int SHADOWS_PFC_COUNTER = 5;

const glm::vec3 SUN_POSITION_DEFAULT(1.0f, 0.0f, 0.0f);


const std::string TEXT_FREE_MODE("Free Mode");
const std::string TEXT_PLAYER_MODE("Player Mode");