
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
#include "light.h"

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

	vector<LightBundle> light_bundles;

public:
	ViewDrawer(Game& g) : game(g) {}

	void TEMP_VBO_SHIT();

	void initialize();
	void deinitialize();

	void projection_set_screen(float, float);

	void render_texture(SH_prog_id shader, TH_tex_id texture, GLuint, unsigned int);
	void render_grumps(SH_prog_id, GLuint, unsigned int);
	void render_light(SH_prog_id, TH_tex_id, float, vector<Light>&);
	
	void draw_screen();	
	void draw_light(LightBundle&);
	void draw_occluders(array<float, 2>& center_loc, float render_size);
	void compress_shadows(float);
	void setup_screen_render();
	void draw_background();
	void draw_visuals();
	void draw_lights();

	vector<LightBundle>& get_light_bundles();
};

#endif