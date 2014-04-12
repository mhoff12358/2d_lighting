
#ifndef __IO_CONTROLLER_H_INCLUDED__
#define __IO_CONTROLLER_H_INCLUDED__

#include <iostream>
#include <array>

#include <SDL2/SDL.h>

using std::array;

class Game;

class IOController {
private:
	Game& game;

	void handle_keypress(SDL_Keysym);
	void handle_keyrelease(SDL_Keysym);

public:
	IOController(Game& g) : game(g) {}

	void initialize();

	int process_events();
	Uint8 get_keypressed(SDL_Keycode);
	
	//Movement key bindings: back, left, forward, right
	const array<SDL_Keycode, 4> MOVE_KEYS = {{SDLK_d, SDLK_s, SDLK_a, SDLK_w}};
	const array<float, 2> LOOK_SENSITIVITY {{0.75f, 0.25}};
};

#endif