
#ifndef __VIEW_DRAWER_H_INCLUDED__
#define __VIEW_DRAWER_H_INCLUDED__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <GL/glu.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "texture_handler.h"
#include "shader.h"

#define grump_vbo_num 4 //Number of vertices

class Game;

class ViewDrawer {
private:
	Game& game;

	//Temporary stuff, should be moved out into actors
	GLuint grump_vbo;
	GLuint shadow_frame_buffer;
	GLuint shadow_texture;

public:
	ViewDrawer(Game& g) : game(g) {}

	void TEMP_VBO_SHIT();

	void initialize();
	void deinitialize();

	void projection_set_screen();

	void draw_screen();
};

#endif