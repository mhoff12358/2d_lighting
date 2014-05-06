
#ifndef __MAIN_H_INCLUDED__
#define __MAIN_H_INCLUDED__

#include <SDL2/SDL.h>

#include <stdlib.h>

#include "view_state.h"
#include "view_drawer.h"
#include "io_controller.h"
#include "game_logic.h"
#include "game_state.h"

class Game {
private:
	ViewState vs;
	ViewDrawer vd = ViewDrawer(*this);
	IOController ioc = IOController(*this);
	GameLogic gl = GameLogic(*this);
	GameState gs = GameState(*this);

public:
	void mainloop();

	void exit_program(int);
	GameLogic& get_logic();
	IOController& get_controller();
	ViewState& get_state();
	GameState& get_game_state();
	ViewDrawer& get_drawer();
};

#endif