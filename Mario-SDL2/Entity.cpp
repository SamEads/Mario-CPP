#include "Entity.hpp"
#include <iostream>
#include "Game.hpp"

void Entity::updatePosition()
{
	position.x += spd.x;
	position.y += spd.y;
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
	int curLWidthMaxFocus = ceil((position.x + texWidth + texHeight) / texWidth);
	int curLWidthMinFocus = floor((position.x - texHeight) / texWidth);
	Rect hitbox;
	int topClip = 8;
	int leftClip = 3;
	int rightClip = 3;
	hitbox.x = position.x + leftClip;
	hitbox.y = position.y + topClip;
	hitbox.w = texWidth - leftClip - rightClip;
	hitbox.h = texHeight - topClip;
	for (int i = hitbox.x - 4 - fabsf(spd.x); i <= hitbox.x + 1 + hitbox.w + fabsf(spd.x); i++)
	{
		for (int j = hitbox.y - 1 - fabsf(spd.y); j <= hitbox.y + 1 + hitbox.h + fabsf(spd.y); j++)
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
			tileRealPosition.x = xCheck * 16;
			tileRealPosition.y = yCheck * 16;
			tileRealPosition.w = 16;
			tileRealPosition.h = 16;

			// Down
			if (spd.y >= 0)
			{
				if ((hitbox.getBottom() >= tileRealPosition.getTop())
				&& (hitbox.getBottom() < tileRealPosition.getTop() + 1 + fabsf(spd.y))
				&& (hitbox.getRight() - 1 >= tileRealPosition.getLeft())
				&& (hitbox.getLeft() + 1 <= tileRealPosition.getRight()))
				{
					position.y = tileRealPosition.getTop() - texHeight;
					vel.y = 0;
					spd.y = 0;
				}
			}
			// Up
			else if (spd.y < 0)
			{
				if ((hitbox.getTop() <= tileRealPosition.getBottom())
				&& (hitbox.getTop() > tileRealPosition.getBottom() - 8)
				&& (hitbox.getRight() - 1 >= tileRealPosition.getLeft() + spd.x)
				&& (hitbox.getLeft() + 1 <= tileRealPosition.getRight() + spd.x))
				{
					position.y = tileRealPosition.getBottom() - topClip;
					spd.y = 0;
				}
			}
			hitbox.x = position.x + leftClip;
			hitbox.y = position.y + topClip;
			hitbox.w = texWidth - leftClip - rightClip;
			hitbox.h = texHeight - topClip;
			// Left
			if (spd.x <= 0)
			{
				if ((hitbox.getLeft() <= tileRealPosition.getRight())						// If the player's left side is going into the solid's right side
				&& (hitbox.getLeft() > tileRealPosition.getRight() - fabsf(spd.x) - 2)		// If the player player is not too far into the tile (speed & extra leeway)
				&& (hitbox.getBottom() > tileRealPosition.getTop() + fabsf(spd.y) + 1)		// If the player is above the tile enough for it not to be the tile they're standing on
				&& (hitbox.getTop() < tileRealPosition.getBottom() + spd.y))				// If the player's top is low enough to not refer to the tile above the player
				{
					position.x = tileRealPosition.getRight() - leftClip;
					spd.x = 0;
					vel.x = 0;
				}
			}
			// Right
			else if (spd.x >= 0)
			{
				if ((hitbox.getRight() >= tileRealPosition.getLeft())
				&& (hitbox.getRight() < tileRealPosition.getLeft() + 2 + fabsf(spd.x))
				&& (hitbox.getBottom() > tileRealPosition.getTop() + fabsf(spd.y) + 1)
				&& (hitbox.getTop() < tileRealPosition.getBottom() + spd.y))
				{
					position.x = tileRealPosition.getLeft() - texWidth + rightClip;
					spd.x = 0;
					vel.x = 0;
				}
			}
		}
	}
}


void Entity::draw(SDL_Texture* texture, Game* game, float x, float y)
{
	SDL_Rect srcRect;
	SDL_Rect sizeRect;
	SDL_Point centerPoint;
	srcRect.x = floor(imgX) * 32;
	srcRect.y = floor(imgY) * 32;
	sizeRect.w = srcRect.w = 32;
	sizeRect.h = srcRect.h = 32;
	sizeRect.x = (position.x) - game->level->camPos.x - 8;
	sizeRect.y = (position.y) + 1;
	centerPoint.x = 16;
	centerPoint.y = 16;
	SDL_RenderCopyEx(game->renderer, texture, &srcRect, &sizeRect, 0, NULL, flipSpr);

}