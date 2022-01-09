#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "Core.hpp"

enum Powerup
{
	SMALL = 0,
	BIG = 1
};

enum Character
{
	MARIO,
	LUIGI
};

class Player : public Entity
{
	public:
		// Constructor
		Player(Level* _level);
		// Methods
		void animate();
		void update();
		// Variables
		bool isSkidding = false;
		bool isCrouching = false;
		bool isJumping = false;
		int runTime = 0;
		int pmeterLevel = 112;
		int pmeterMax = 112;
		Powerup powerup = SMALL;
		Character character = MARIO;
		bool fullRun = false;
		bool noClip = false;
		// Animations
		Animation idleAnim;
		Animation walkAnim;
		Animation runAnim;
		Animation runJumpAnim;
		Animation skidAnim;
		Animation jumpAnim;
		Animation crouchAnim;
};

#endif // !PLAYER_HPP