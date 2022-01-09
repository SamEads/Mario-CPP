#include "Core.hpp"
#include "Game.hpp"
#include "AssetManager.hpp"

Game *game = NULL;
unsigned int palShader = 0;
void initCore(Game *_game)
{
	game = _game;
	std::srand(std::time(nullptr));
}

void initShaders()
{
	std::string vertex = 
		"#version 330 core\n"
		"layout (location = 0) in vec2 inPosition;\n"
		"layout (location = 1) in vec4 inColor;\n"
		"layout (location = 2) in vec2 inTextureCoord;\n"
		"out vec2 outPosition;\n"
		"out vec4 outColor;\n"
		"out vec2 outTexcoord;\n"
		"void main()\n"
		"{\n"
		"	outPosition = inPosition;\n"
		"	outColor = inColor;\n"
		"	outTexcoord = inTextureCoord;\n"
		"}";
	std::string fragment =
		"#version 330 core\n"
		"in vec2 outPosition;\n"
		"in vec4 outColor;\n"
		"in vec2 outTexcoord;\n"
		"out vec4 fragColor;\n"
		"uniform sampler2D baseTexture;\n"
		"void main()\n"
		"{\n"
		"	vec4 src = texture2D(baseTexture, outTexcoord);\n"
		"	fragColor = src;\n"
		"}";
	std::cout << vertex << std::endl;
	std::cout << fragment << std::endl;
	palShader = createShader(vertex, fragment);
}

void drawTile(uint16_t x, uint16_t y, uint16_t tileX, uint16_t tileY, bool followCam)
{
	Rect srcRect;
	Rect sizeRect;
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
	renderCopy(getTexture("tiles/tiles"), &srcRect, &sizeRect, FLIP_NONE);
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

int renderCopy(Texture* texture, const Rect* srcRect, const Rect* destRect, FlipSprite flipSpr)
{
	GLfloat minx = destRect->x;
	GLfloat miny = destRect->y;
	GLfloat maxx = destRect->x + destRect->w;
	GLfloat maxy = destRect->y + destRect->h;
	GLfloat minu = (GLfloat)srcRect->x / texture->width;
	GLfloat maxu = (GLfloat)(srcRect->x + srcRect->w) / texture->width;
	GLfloat minv = (GLfloat)srcRect->y / texture->height;
	GLfloat maxv = (GLfloat)(srcRect->y + srcRect->h) / texture->height;

	Point *centerPoint = new Point();
	centerPoint->x = (destRect->w / 2);
	centerPoint->y = (destRect->h / 2);

	if (flipSpr & FLIP_HORIZONTAL)
	{
		minx = destRect->w - centerPoint->x;
		maxx = -centerPoint->x;
	}
	else
	{
		minx = -centerPoint->x;
		maxx = destRect->w - centerPoint->x;
	}

	if (flipSpr & FLIP_VERTICAL)
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
	glRotated(0, (GLdouble)1.0, (GLdouble)0.0, (GLdouble)0.0);
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
}

void trueMouseCoordinates(int* logicalMouseX, int* logicalMouseY)
{
	int glW, glH;
	SDL_GetMouseState(logicalMouseX, logicalMouseY);
	SDL_GL_GetDrawableSize(game->window, &glW, &glH);
	float ratio = (float)glH / game->gameHeight;
	float dispWidth = 0;
	float dispHeight = 0;
	if (game->gameWidth * ratio < glW)
	{
		dispWidth = game->gameWidth * ratio;
		glViewport((glW - dispWidth) / 2, 0, dispWidth, glH);
		*logicalMouseX = (*logicalMouseX /= ratio) - (int)(((float)glW / (float)ratio) - (float)game->gameWidth) / 2.f;
		*logicalMouseY /= ratio;
	}
	else
	{
		ratio = (float)glW / game->gameWidth;
		dispHeight = game->gameHeight * ratio;
		glViewport(0, (glH - dispHeight) / 2, glW, dispHeight);
		*logicalMouseX /= ratio;
		*logicalMouseY = (*logicalMouseY /= ratio) - (int)(((float)glH / (float)ratio) - (float)game->gameHeight) / 2.f;
	}
}

float lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

float clamp(float value, float min, float max)
{
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}