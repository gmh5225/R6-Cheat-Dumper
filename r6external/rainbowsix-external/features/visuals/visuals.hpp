#include <Windows.h>

namespace visuals {
	void esp();
	void player_fov(float fov);
	void gunmodel_fov(float fov);
	void glow(float red, float green, float blue, float alpha, float opacity, float distance);
	void noflash(byte value);
	void spotted(bool enabled);
	void gravity(bool enable);
	void thirdperson(int value);
	void drone(bool enabled);

	extern bool state;
	extern int real_team;
}
