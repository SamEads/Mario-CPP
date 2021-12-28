#include "Entity.hpp"
#include <iostream>
#include "Game.hpp"

void Entity::updatePosition()
{
	position += spd;
}

/**
* INCREDIBLY UNOPTIMIZED WITH NON-AUTOMATED CODE!!! Go back to this!
* */
void Entity::collide()
{
	if (level == nullptr || game == nullptr)
	{
		std::cout << "FATAL ERROR!!! ENTITY DOES NOT HAVE LEVEL AND OR GAME REFERENCE" << std::endl;
		return;
	}
	int texWidth = 16;
	int texHeight = 32;
	int curLWidthMaxFocus = (int) ceil((position.x + texWidth + texHeight) / texWidth);
	int curLWidthMinFocus = (int) floor((position.x - texHeight) / texWidth);
	Rect hitbox = getHitbox();

	bool leftCollide = false;
	bool rightCollide = false;
	bool upCollide = false;
	bool downCollide = false;
	bumpedHead = false;

	for (int i = (int) (hitbox.x - 4 - fabsf(spd.x)); i <= hitbox.x + 1 + hitbox.w + fabsf(spd.x); i++)
	{
		for (int j = (int) (hitbox.y - 1 - fabsf(spd.y)); j <= hitbox.y + 1 + hitbox.h + fabsf(spd.y); j++)
		{
			int xCheck = (int)floor(i / 16);
			int yCheck = (int)floor(j / 16);

			if ((xCheck >= level->levelWidth)
				|| (yCheck >= level->levelHeight)
				|| (xCheck < 0)
				|| (yCheck < 0))
				continue;

			Tile* tile = level->tiles[xCheck][yCheck];

			if (tile->cellX == -1 || tile->cellY == -1)
				continue;

			Rect tileRealPosition;
			tileRealPosition.x = (int) (xCheck * 16);
			tileRealPosition.y = (int) (yCheck * 16);
			tileRealPosition.w = 16;
			tileRealPosition.h = 16;

			// Down
			if (spd.y >= 0 && !downCollide && !upCollide)
			{
				if ((hitbox.getBottom() >= tileRealPosition.getTop())
				&& (hitbox.getBottom() < tileRealPosition.getTop() + 1 + fabsf(spd.y))
				&& (hitbox.getRight() - 1 >= tileRealPosition.getLeft())
				&& (hitbox.getLeft() + 1 <= tileRealPosition.getRight()))
				{
					position.y = tileRealPosition.getTop() - texHeight;
					vel.y = 0;
					spd.y = 0;
					downCollide = true;
				}
			}
			// Up
			else if (spd.y < 0 && !upCollide && !downCollide)
			{
				if ((hitbox.getTop() <= tileRealPosition.getBottom())
				&& (hitbox.getTop() > tileRealPosition.getBottom() - 8)
				&& (hitbox.getRight() - 1 >= tileRealPosition.getLeft() + spd.x)
				&& (hitbox.getLeft() + 1 <= tileRealPosition.getRight() + spd.x))
				{
					position.y = tileRealPosition.getBottom() - topClip;
					spd.y = 0;
					upCollide = true;
					bumpedHead = true;
				}
			}
			hitbox = getHitbox();
			// Left
			if (spd.x <= 0 && !leftCollide && !rightCollide)
			{
				if ((hitbox.getLeft() <= tileRealPosition.getRight())					// If the player's left side is going into the solid's right side
				&& (hitbox.getLeft() > tileRealPosition.getRight() - fabsf(spd.x) - 2)	// If the player player is not too far into the tile (speed & extra leeway)
				&& (hitbox.getBottom() > tileRealPosition.getTop() + fabsf(spd.y) + 1)	// If the player is above the tile enough for it not to be the tile they're standing on
				&& (hitbox.getTop() < tileRealPosition.getBottom() + spd.y))			// If the player's top is low enough to not refer to the tile above the player
				{
					position.x = tileRealPosition.getRight() - leftClip;
					spd.x = 0;
					vel.x = 0;
					leftCollide = true;
				}
			}
			// Right
			else if (spd.x >= 0 && !rightCollide && !leftCollide)
			{
				if ((hitbox.getRight() >= tileRealPosition.getLeft())
				&& (hitbox.getRight() < tileRealPosition.getLeft() + 2 + fabsf(spd.x))
				&& (hitbox.getBottom() > tileRealPosition.getTop() + fabsf(spd.y) + 1)
				&& (hitbox.getTop() < tileRealPosition.getBottom() + spd.y))
				{
					position.x = tileRealPosition.getLeft() - texWidth + rightClip;
					spd.x = 0;
					vel.x = 0;
					rightCollide = true;
				}
			}
		}
	}
}

Rect Entity::getHitbox()
{
	int texWidth = 16;
	int texHeight = 32;
	Rect hitbox;
	hitbox.x = position.x + leftClip;
	hitbox.y = position.y + topClip;
	hitbox.w = (int) (texWidth - leftClip - rightClip);
	hitbox.h = (int) (texHeight - topClip);
	return hitbox;
}

void Entity::draw(SDL_Texture* texture, Game* game, float x, float y)
{
	int frameVectorSize = (int) curAnim.frames.size();
	if (frameVectorSize <= 1)
	{
		curFrame = 0;
	}
	else if (curFrame >= frameVectorSize - 1)
	{
		if (frameVectorSize > 1)
		{
			while (curFrame >= frameVectorSize)
				curFrame -= frameVectorSize;
		}
		else
		{
			curFrame = 0;
		}
	}
	if (curFrame < 0)
		curFrame = 0;

	// Get the sprite to display in the proper vector position
	imgX = curAnim.frames[(int) floor(curFrame)];

	SDL_Rect srcRect;
	SDL_Rect sizeRect;
	SDL_Point centerPoint;
	srcRect.x = (int) floor(imgX) * 32;
	srcRect.y = (int) floor(imgY) * 32;
	sizeRect.w = srcRect.w = 32;
	sizeRect.h = srcRect.h = 32;
	sizeRect.x = (position.x) - game->level->camPos.x - 8;
	sizeRect.y = (position.y) + 1;
	centerPoint.x = 16;
	centerPoint.y = 16;
	SDL_RenderCopyEx(game->renderer, texture, &srcRect, &sizeRect, 0, NULL, flipSpr);

}