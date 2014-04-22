#include "main.h"
#include <iostream>
#include <SDL2/SDL.h>


void Game::exit_program(int exit_code) {
	vd.deinitialize();

	vs.destroy_renderer();
	vs.destroy_window();
	vs.deinitialize();

	exit(exit_code);
}

void Game::mainloop() {
	vs.initialize();
	vs.create_window();
	vs.create_renderer();
	vs.create_GL_context();

	vd.initialize();

	vs.load_images();
	vs.create_shaders();

	vd.TEMP_VBO_SHIT();

	ioc.initialize();


	while (1) {
		if (ioc.process_events() != 0){
			exit_program(1);
		}

		gl.update_world();

		vd.draw_screen();
		vs.render_screen();
	}
}

GameLogic& Game::get_logic() {
	return gl;
}

IOController& Game::get_controller() {
	return ioc;
}

ViewState& Game::get_state() {
	return vs;
}

ViewDrawer& Game::get_drawer() {
	return vd;
}

int main(int argc, char ** argv) {
	Game g;

	g.mainloop();

	return 0;

}