#include "io_controller.h"

#include "main.h"

void IOController::initialize() {
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

int IOController::process_events() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return 1;
		} else if (event.type == SDL_KEYDOWN && !event.key.repeat) {
			handle_keypress(event.key.keysym);
		} else if (event.type == SDL_KEYUP && !event.key.repeat) {
			handle_keyrelease(event.key.keysym);
		} else if (event.type == SDL_MOUSEMOTION) {
			// std::cout << array<int, 4>({{event.motion.xrel, event.motion.yrel, event.motion.x, event.motion.y}})[0] << std::endl;
			game.get_logic().add_mouse_motion(array<int, 4>({{event.motion.xrel, event.motion.yrel, event.motion.x, event.motion.y}}));
		}
	}
	return 0;
}

void IOController::handle_keypress(SDL_Keysym key_info) {
	if (key_info.sym == SDLK_ESCAPE) {
		game.exit_program(0);
	}
}

void IOController::handle_keyrelease(SDL_Keysym key_info) {

}

Uint8 IOController::get_keypressed(SDL_Keycode key_info) {
	return SDL_GetKeyboardState(NULL)[SDL_GetScancodeFromKey(key_info)];
}