
#ifndef __VIEW_STATE_H_INCLUDED__
#define __VIEW_STATE_H_INCLUDED__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <string>
#include <cstdio>

#include "shader.h"
#include "texture_handler.h"

using std::string;

class ViewState {
private:
	const int SCREEN_WIDTH = 1000;
	const int SCREEN_HEIGHT = 800;

	SDL_Window * window = NULL;
	SDL_Renderer * renderer = NULL;
	SDL_GLContext context;
	std::string creation_errors;

	TextureHandler th;
	ShaderHandler sh;

public:
	SDL_Window * get_window() {return window;};
	SDL_Renderer * get_renderer() {return renderer;};

	int initialize();
	void deinitialize();

	void create_window();
	void destroy_window();
	void create_renderer();
	void destroy_renderer();
	void create_GL_context();
	void create_shaders();
	int check_valid();

	void load_images();
	void unload_images();

	void render_screen();

	int screen_width();
	int screen_height();

	void bind_texture(TH_tex_id);
	void gen_texture(TH_tex_id);
	void load_texture_image(TH_tex_id, const char *);
	void load_texture_empty(TH_tex_id, unsigned int, unsigned int);
	void load_texture_empty(TH_tex_id, unsigned int);
	GLuint get_texture_name(TH_tex_id);

	void use_program(SH_prog_id);
	GLuint get_uniform_loc(SH_prog_id, const char *);
	vector<pair<unsigned int, string>>& get_shader_attrs(SH_prog_id);
};

#endif