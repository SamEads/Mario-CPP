#ifndef HUD_HPP
#define HUD_HPP

class Level;

class HUD
{
	private:
		Level* level;
	public:
		HUD(Level* _level);
		void draw();
		int pFlash = 0;
};

#endif // !HUD_HPP