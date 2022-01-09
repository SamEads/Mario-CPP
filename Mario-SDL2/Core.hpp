#ifndef CORE_HPP
#define CORE_HPP

#include <vector>
#include <string>

#define DEBUG true;

class Game;
class Texture;
struct Rect;

/* Enumerators */
enum FlipSprite
{
	FLIP_NONE,
	FLIP_HORIZONTAL,
	FLIP_VERTICAL
};
enum BlockType 
{
	SOLID,
	SEMISOLID,
};

/* Extern Variables */
extern unsigned int palShader;
extern Game* game;

/* Functions */
void drawTile(uint16_t x, uint16_t y, uint16_t tileX, uint16_t tileY, bool followCam = true);
void initCore(Game* game);
void initShaders();
void trueMouseCoordinates(int* logicalMouseX, int* logicalMouseY);
int renderCopy(Texture* texture, const Rect* srcRect, const Rect* destRect, FlipSprite flipSpr = FLIP_NONE);
float lerp(float a, float b, float t);
float clamp(float value, float min, float max);
void resetView();

/* Structs */
struct Vector2
{
	Vector2()
	{
		x = 0;
		y = 0;
	}
	Vector2(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	float x;
	float y;
	void operator += (Vector2 &v2)
	{
		this->x += v2.x;
		this->y += v2.y;
	}
	void operator -= (Vector2& v2)
	{
		this->x -= v2.x;
		this->y -= v2.y;
	}
};
struct Point
{
	Point()
	{
		x = 0;
		y = 0;
	}
	Point(int _x, int _y)
	{
		x = _x;
		y = _y;
	}
	int x;
	int y;
	void operator += (Point& v2)
	{
		this->x += v2.x;
		this->y += v2.y;
	}
	void operator -= (Point& v2)
	{
		this->x -= v2.x;
		this->y -= v2.y;
	}
};
struct Rect
{
	Rect()
	{
		x = 0;
		y = 0;
		w = 0;
		h = 0;
	}
	Rect(float _x, float _y, float _w, float _h)
	{
		x = _x;
		y = _y;
		w = _w;
		h = _h;
	}
	float x, y, w, h;
	float getLeft() { return x; }
	float getRight() { return x + w; }
	float getTop() { return y; }
	float getBottom() { return y + h; }
	Vector2 getCenter() { return Vector2((float) (x + (w / 2)), (float) (y + (h / 2))); }
	bool intersects(Rect rect) { return !(rect.getLeft() > getRight() || rect.getRight() < getLeft() || rect.getTop() > getBottom() || rect.getBottom() < getTop()); }
	bool pointInside(Point point) { return !(point.x > getRight() || point.x < getLeft() || point.y > getBottom() || point.y < getTop()); }
	bool vector2Inside(Vector2 vector) { return !(vector.x > getRight() || vector.x < getLeft() || vector.y > getBottom() || vector.y < getTop()); }
};
struct Recti
{
	int x, y, w, h;
	int getLeft() { return x; }
	int getRight() { return x + w; }
	int getTop() { return y; }
	int getBottom() { return y + h; }
	Point getCenter() { return Point((int) x + (w / 2), (int) y + (h / 2)); }
	bool intersects(Recti rect) { return !(rect.getLeft() > getRight() || rect.getRight() < getLeft() || rect.getTop() > getBottom() || rect.getBottom() < getTop()); }
	bool pointInside(Point point) { return !(point.x > getRight() || point.x < getLeft() || point.y > getBottom() || point.y < getTop()); }
	bool vector2Inside(Vector2 vector) { return !(vector.x > getRight() || vector.x < getLeft() || vector.y > getBottom() || vector.y < getTop()); }
};
struct Animation
{
	std::vector<int> frames;
};
struct Tile
{
	Tile(int _cellX = -1, int _cellY = -1, int _width = 16, int _height = 16)
	{
		cellX = _cellX;
		cellY = _cellY;
		width = _width;
		height = _height;
	}
	int cellX,cellY,width,height;
};
struct Block
{
	Tile tileTexture = { -1, -1 };
	BlockType blockType;
};

#endif // !CORE_HPP