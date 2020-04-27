#pragma once
#include <memory>

class CharacterComponent;
class PhysicsComponent;
class Player;

class PlayerContext
{
public:
	PlayerContext(std::shared_ptr<Player> player);
	~PlayerContext() = default;

	std::shared_ptr<Player> GetPlayer() { return mPlayer; }
	std::shared_ptr<CharacterComponent> GetCharacterComponent() { return mCharacterComponent; }
	std::shared_ptr<PhysicsComponent> GetPhysicsComponent() { return mPhysicsComponent; }

private:
	std::shared_ptr<Player> mPlayer;
	std::shared_ptr<CharacterComponent> mCharacterComponent;
	std::shared_ptr<PhysicsComponent> mPhysicsComponent;
};

