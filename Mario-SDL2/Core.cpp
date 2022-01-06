#include "Core.hpp"
#include "Game.hpp"
#include "AssetManager.hpp"

Game *game = NULL;
void initFunctions(Game *_game)
{
	game = _game;
}

void drawTile(int x, int y, int tileX, int tileY, bool followCam)
{
	SDL_Rect srcRect;
	SDL_Rect sizeRect;
	srcRect.x = tileX * 16;
	srcRect.y = tileY * 16;
	sizeRect.w = srcRect.w = 16;
	sizeRect.h = srcRect.h = 16;
	sizeRect.x = x;
	sizeRect.y = y;
	if (6 == tileX && 0 == tileY)
	{
		int floatTick = floor(game->shineTick);
		srcRect.x += floatTick * 16;
	}
	renderCopy(getTexture("tiles"), &srcRect, &sizeRect, SDL_FLIP_NONE);
}

void resetView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, game->gameWidth, game->gameHeight, 0, -1, 1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int renderCopy(Texture* texture, const SDL_Rect* srcRect, const SDL_Rect* destRect, SDL_RendererFlip flipSpr)
{
	#if !USEOPENGL
	return SDL_RenderCopy(game->renderer, texture->sdlTexture, srcRect, destRect);
	#else
	GLfloat minx = destRect->x;
	GLfloat miny = destRect->y;
	GLfloat maxx = destRect->x + destRect->w;
	GLfloat maxy = destRect->y + destRect->h;
	GLfloat minu = (GLfloat)srcRect->x / texture->width;
	GLfloat maxu = (GLfloat)(srcRect->x + srcRect->w) / texture->width;
	GLfloat minv = (GLfloat)srcRect->y / texture->height;
	GLfloat maxv = (GLfloat)(srcRect->y + srcRect->h) / texture->height;

	SDL_FPoint *centerPoint = new SDL_FPoint();
	centerPoint->x = (destRect->w / 2);
	centerPoint->y = (destRect->h / 2);

	if (flipSpr & SDL_FLIP_HORIZONTAL)
	{
		minx = destRect->w - centerPoint->x;
		maxx = -centerPoint->x;
	}
	else
	{
		minx = -centerPoint->x;
		maxx = destRect->w - centerPoint->x;
	}

	if (flipSpr & SDL_FLIP_VERTICAL)
	{
		miny = destRect->h - centerPoint->y;
		maxy = -centerPoint->y;
	}
	else
	{
		miny = -centerPoint->y;
		maxy = destRect->h - centerPoint->y;
	}

	glBindTexture(GL_TEXTURE_2D, texture->id);
	glPushMatrix();
	glTranslatef((GLfloat)destRect->x + centerPoint->x, (GLfloat)destRect->y + centerPoint->y, (GLfloat)0.0);
	glRotated(0, (GLdouble)0.0, (GLdouble)0.0, (GLdouble)1.0);
	glBegin(GL_QUADS);
	glTexCoord2f(maxu, minv);
	glVertex2f(maxx, miny);
	glTexCoord2f(minu, minv);
	glVertex2f(minx, miny);
	glTexCoord2f(minu, maxv);
	glVertex2f(minx, maxy);
	glTexCoord2f(maxu, maxv);
	glVertex2f(maxx, maxy);
	glEnd();
	glPopMatrix();
	delete centerPoint;
	return 0;
	#endif
}

void trueMouseCoordinates(SDL_Renderer* renderer, SDL_Window* window, int* logicalMouseX, int* logicalMouseY)
{
	int realMouseX, realMouseY;
	int wWidth, wHeight;
	int rLogicalWidth, rLogicalHeight;
	int rRealWidth, rRealHeight;
	float rScaleX, rScaleY;
	int rMidpointY, wMidpointY;
	int rMidpointX, wMidpointX;
	int rY, rX;

	SDL_GetMouseState(&realMouseX, &realMouseY);
	SDL_GetWindowSize(window, &wWidth, &wHeight);
	wMidpointY = wHeight / 2;
	wMidpointX = wWidth / 2;

	int glW, glH;
	SDL_GL_GetDrawableSize(window, &glW, &glH);
	float ratio = (float)glH / game->gameHeight;
	rLogicalWidth = game->gameWidth;
	rLogicalHeight = game->gameHeight;
	rScaleX = rScaleY = ratio;
	rRealWidth = (float)rLogicalWidth * (float)rScaleX;
	rRealHeight = (float)rLogicalHeight * (float)rScaleY;
	rMidpointY = rRealHeight / 2;
	rMidpointX = rRealWidth / 2;
	rY = wMidpointY - rMidpointY;
	rX = wMidpointX - rMidpointX;

	int adjustedMouseY = realMouseY - rY; // takes into account any border when keeping aspect ratio
	int adjustedMouseX = realMouseX - rX;
	*logicalMouseX = (float)adjustedMouseX / (float)rRealWidth * (float)rLogicalWidth;
	*logicalMouseY = (float)adjustedMouseY / (float)rRealHeight * (float)rLogicalHeight;

}

float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}