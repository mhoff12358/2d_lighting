
#include "game_logic.h"

#include "main.h"


void set_rgb(ViewDrawer& vd, float r, float g, float b) {
	vd.light_color[0] = r;
	vd.light_color[1] = g;
	vd.light_color[2] = b;
}

float mod(float a, float b) {
	return a-floor(a/b)*b;
}

void GameLogic::update_world() {
	unsigned int time_diff = clock() - game_clock;
	game_clock += time_diff;

	float time_scale = ((float)time_diff)/CLOCKS_PER_SEC;

	IOController& ioc = game.get_controller();
	#define move_scale 80.0
	if (ioc.get_keypressed(SDLK_d)) {
		game.get_drawer().light_x += time_scale*move_scale;
	}
	if (ioc.get_keypressed(SDLK_a)) {
		game.get_drawer().light_x += -time_scale*move_scale;
	}
	if (ioc.get_keypressed(SDLK_w)) {
		game.get_drawer().light_y += time_scale*move_scale;
	}
	if (ioc.get_keypressed(SDLK_s)) {
		game.get_drawer().light_y += -time_scale*move_scale;
	}

	if (ioc.get_keypressed(SDLK_RIGHTBRACKET)) {
		game.get_drawer().light_size += time_scale*move_scale*4;
	}
	if (ioc.get_keypressed(SDLK_LEFTBRACKET)) {
		game.get_drawer().light_size += -time_scale*move_scale*4;
	}

	int h = ((float)game_clock/CLOCKS_PER_SEC*80.0);

	float c = 1.0;
	float x = (1-fabs(mod((float)h/60.0, 2)-1));

	if (h < 60) set_rgb(game.get_drawer(), c, x, 0);
	else if (h < 120) set_rgb(game.get_drawer(), x, c, 0);
	else if (h < 180) set_rgb(game.get_drawer(), 0, c, x);
	else if (h < 240) set_rgb(game.get_drawer(), 0, x, c);
	else if (h < 300) set_rgb(game.get_drawer(), x, 0, c);
	else set_rgb(game.get_drawer(), c, 0, x);
}

void GameLogic::add_mouse_motion(array<int, 4> new_motion) {
	//The motions that are added should be in the form of xrel, yrel, x, y
	mouse_motion_queue.emplace(new_motion);
}