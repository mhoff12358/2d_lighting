
#include "game_logic.h"

#include "main.h"

void GameLogic::update_world() {
	unsigned int time_diff = clock() - game_clock;
	game_clock += time_diff;

	float time_scale = ((float)time_diff)/CLOCKS_PER_SEC;
}

void GameLogic::add_mouse_motion(array<int, 4> new_motion) {
	//The motions that are added should be in the form of xrel, yrel, x, y
	mouse_motion_queue.emplace(new_motion);
}