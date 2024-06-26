#include "Entity.hpp"
#include <iostream>
#include "Game.hpp"
#include "AssetManager.hpp"

void Entity::updatePosition()
{
	position += spd;
}

void Entity::update()
{

}

void Entity::postUpdate()
{
	positionLast = position;
}

/**
* INCREDIBLY UNOPTIMIZED WITH NON-AUTOMATED CODE!!! Go back to this!
* */
void Entity::collide()
{
	if (game == nullptr)
	{
		std::cout << "ENTITY DOES NOT HAVE LEVEL AND OR GAME REFERENCE" << std::endl;
		return;
	}
	int curLWidthMaxFocus = (int) ceil((position.x + texWidth + texHeight) / texWidth);
	int curLWidthMinFocus = (int) floor((position.x - texHeight) / texWidth);
	Rect hitbox = getHitbox();

	bool leftCollide = false;
	bool rightCollide = false;
	bool upCollide = false;
	bool downCollide = false;
	bumpedHead = false;

	for (int i = (int) (hitbox.getLeft() - 4 - fabsf(spd.x)); i <= hitbox.getRight() + fabsf(spd.x) + 4; i++)
	{
		for (int j = (int)(hitbox.y - 1 - fabsf(spd.y)); j <= hitbox.y + 1 + hitbox.h + fabsf(spd.y); j++)
		{
			int xCheck = (int)floor(i / 16);
			int yCheck = (int)floor(j / 16);

			if ((xCheck >= game->level->levelWidth)
				|| (yCheck >= game->level->levelHeight)
				|| (xCheck < 0)
				|| (yCheck < 0))
				continue;

			Tile* tile = game->level->tiles[xCheck][yCheck];

			if (tile->cellX == -1 || tile->cellY == -1)
				continue;

			Recti tileRealPosition { xCheck * 16, yCheck * 16, tile->width, tile->height };

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
				&& (hitbox.getLeft() > tileRealPosition.getRight() - 2 - fabsf(spd.x))	// If the player player is not too far into the tile (speed & extra leeway)
				&& (hitbox.getBottom() > tileRealPosition.getTop() + 1 + fabsf(spd.y))	// If the player is above the tile enough for it not to be the tile they're standing on
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
				&& (hitbox.getBottom() > tileRealPosition.getTop() + 1 + fabsf(spd.y))
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
	Rect hitbox;
	hitbox.x = position.x + leftClip;
	hitbox.y = position.y + topClip;
	hitbox.w = (int) (texWidth - leftClip - rightClip);
	hitbox.h = (int) (texHeight - topClip - bottomClip);
	return hitbox;
}

void Entity::draw()
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
	int curAnimFrameSize = curAnim.frames.size();
	if ((int)floor(curFrame) < curAnimFrameSize)
		imgX = curAnim.frames[(int)floor(curFrame)];
	else
		imgX = 0;
	Rect srcRect;
	Rect sizeRect;
	Vector2 centerPoint;
	srcRect.x = (int)floor(imgX) * texWidth;
	srcRect.y = (int)floor(imgY) * texHeight;
	sizeRect.w = srcRect.w = texWidth;
	sizeRect.h = srcRect.h = texHeight;
	sizeRect.x = ceil(position.x);
	sizeRect.y = (position.y) + 1;
	centerPoint.x = texWidth / 2;
	centerPoint.y = texHeight / 2;
	//glDisable(GL_BLEND);
	//glDisable(GL_TEXTURE_2D);
	//glUseProgram(palShader);
	//glUniform1f(glGetUniformLocation(palShader, "baseTexture"), texture->id);
	renderCopy(texture, &srcRect, &sizeRect, flipSpr);
	//glUseProgram(0);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
}