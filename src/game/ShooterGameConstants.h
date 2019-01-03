#pragma once
#include <glm/glm.hpp>
#include <string>


const std::string GAME_SCENE_NAME("gameScene");

const std::string GUI_QUAD_MODEL("gui_quad");

static const float VIEW_ANGLE = 45.0f;
static const float FAR_PLANE = 1000.0f;
static const float NEAR_PLANE = 0.1f;

const float TERRAIN_SCALE = 10.66f;
static glm::vec3 MARS_GRAVITY_VALUE(0.0f, -3.711f, 0.0f);

static const float PLAYER_UPWARDS_HEIGHT = 2.0f;
static const float PLAYER_RUN_SPEED = 3.0f;
static const float PLAYER_TURN_SPEED = 0.2f;
static const float PLAYER_ZOOM_SPEED = 10.0f;
static const float PLAYER_PITCH = 10.0f;
static const float PLAYER_PITCH_SPEED = 10.0f;


const int ITEM_SIZE = 64;

static const unsigned int NUM_ITEMS_INVENTORY = 5;
static const float PLAYER_PICKUP_RADIUS = 1.5f;

static const float MAX_PLAYER_LIFE = 100.0f;