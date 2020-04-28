#pragma once
#include <memory>

class CharacterComponent;
class PhysicsComponent;
class CollisionComponent;
class Player;

class PlayerContext
{
public:
	PlayerContext(std::shared_ptr<Player> player);
	~PlayerContext() = default;

	std::shared_ptr<Player> GetPlayer() { return mPlayer; }
	std::shared_ptr<CharacterComponent> GetCharacterComponent() { return mCharacterComponent; }
	std::shared_ptr<PhysicsComponent> GetPhysicsComponent() { return mPhysicsComponent; }
	std::shared_ptr<CollisionComponent> GetCollisionComponent() { return mCollisionComponent; }
private:
	std::shared_ptr<Player> mPlayer;
	std::shared_ptr<CharacterComponent> mCharacterComponent;
	std::shared_ptr<PhysicsComponent> mPhysicsComponent;
	std::shared_ptr<CollisionComponent> mCollisionComponent;
};

