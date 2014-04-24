
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

class Game;

class ViewDrawer {
private:
	Game& game;

	//Temporary stuff, should be moved out into actors
	GLuint grump_vbo;
	GLuint square1_vbo;
	GLuint arc_vbo;
	GLuint occluder_frame_buffer;
	GLuint shadow_frame_buffer;

public:
	ViewDrawer(Game& g) : game(g) {}

	float light_x = 0.0; 
	float light_y = 0.0;
	float light_size = 800.0;
	array<float, 3> light_color = {{1.0, 1.0, 1.0}};
	void TEMP_VBO_SHIT();

	void initialize();
	void deinitialize();

	void projection_set_screen(int, int);

	void render_texture(SH_prog_id shader, TH_tex_id texture, GLuint, unsigned int);
	void render_grumps(SH_prog_id, GLuint, unsigned int);
	void render_light(SH_prog_id, TH_tex_id, array<float, 2>);
	
	void draw_screen();	
	void draw_light(array<float, 2> center_loc, unsigned int render_size, array<float, 2> arc);
	void draw_occluders(array<float, 2> center_loc, unsigned int render_size);
	void compress_shadows();
	void setup_screen_render();
	void draw_background();
	void draw_visuals();
	void draw_lights();
};

#endif