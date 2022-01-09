#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "Core.hpp"
#include <iostream>
#include "Sprite.hpp"

class Level;
class Game;
class Texture;

class Entity : public Sprite
{
	protected:
		void updatePosition();
	public:
		std::string identifier = "E";
		virtual void update();
		void postUpdate();
		virtual void collide();
		virtual void draw();
		Rect getHitbox();
		FlipSprite flipSpr = FLIP_NONE;
		float imgX = 0;
		float imgY = 0;
		Vector2 spawnPosition;
		Vector2 position;
		Vector2 positionLast;
		Vector2 spd;
		Vector2 vel;
		Texture* texture;
		bool bumpedHead = false;

		// Animations
		float curFrame = 0;
		Animation curAnim;

		int depth = 0;
};

#endif // !ENTITY_HPP