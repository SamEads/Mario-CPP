#ifndef SPRITE_HPP
#define SPRITE_HPP

/*
 * Underling class for several sprite-based classes
 */
class Sprite
{
	protected:
		// Texture size
		int texWidth = 32;
		int texHeight = 32;
		// Hitbox
		int topClip = 18;
		int leftClip = 11;
		int rightClip = 11;
		int bottomClip = 0;
};

#endif // !SPRITE_HPP