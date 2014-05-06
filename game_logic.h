
#ifndef __GAME_LOGIC_H_INCLUDED__
#define __GAME_LOGIC_H_INCLUDED__

#include <iostream>
#include <time.h>
#include <queue>
#include <array>

#define _USE_MATH_DEFINES
#include <math.h>

#include "my_math.h"

using std::queue;
using std::array;

class Game;

class GameLogic {
private:
	Game& game;

	unsigned int game_clock;

	queue<array<int, 4>> mouse_motion_queue; //xrel, yrel, x, y

public:
	GameLogic(Game& g) : game(g) {}

	void initialize();
	void deinitialize();

	void update_world();

	void add_mouse_motion(array<int, 4>);
};

#endif