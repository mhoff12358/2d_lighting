
#include "game_logic.h"

#include "main.h"

void GameLogic::update_world() {
	unsigned int time_diff = clock() - game_clock;
	game_clock += time_diff;

	float time_scale = ((float)time_diff)/CLOCKS_PER_SEC;

	IOController& ioc = game.get_controller();
	bool moved = false;
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
	// for (int dir = 0; dir < 4; dir++) {
	// 	if (ioc.get_keypressed(ioc.MOVE_KEYS[dir])) {
	// 		player_loc[0] += time_scale*move_scale*cos(M_PI*(player_ori[0]-90*dir)/180);
	// 		player_loc[1] += time_scale*move_scale*sin(M_PI*(player_ori[0]-90*dir)/180);
	// 		moved = true;
	// 	}
	// }
	// if (moved) {
	// 	player_loc[2] = game.get_terrain().get_height(player_loc[0], player_loc[1], 0.06f);
	// }
}

void GameLogic::add_mouse_motion(array<int, 4> new_motion) {
	//The motions that are added should be in the form of xrel, yrel, x, y
	mouse_motion_queue.emplace(new_motion);
}