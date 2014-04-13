#include "view_state.h"
#include <iostream>

char* filetobuf(char *file, int * length)
{
    FILE *fptr;
    char *buf;

    fptr = fopen(file, "r"); /* Open file for reading */
    if (!fptr) /* Return NULL on failure */
        return NULL;
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    *length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = new char[*length+1]; /* Allocate a buffer for the entire length of the file and a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, *length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[*length] = 0; /* Null terminator */

    return buf; /* Return the buffer */
}

int ViewState::initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		return 1;
	}

	return 0;
}

void ViewState::deinitialize() {
	SDL_Quit();
}

void ViewState::create_window() {
	window = SDL_CreateWindow("_", 50, 50, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	if (window == NULL) {
		// creation_errors << SDL_GetError() << std::endl;
	}
}

void ViewState::destroy_window() {
	if (window != NULL) {
		SDL_DestroyWindow(window);
		window = NULL;
	}
}

void ViewState::create_renderer() {
	if (window == NULL) {
		renderer = NULL;
	} else {
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (renderer == NULL) {
			// creation_errors << SDL_GetError() << std::endl;
		}
	}
}
void ViewState::destroy_renderer() {
	if (renderer != NULL) {
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
}

void ViewState::create_GL_context() {
	context = SDL_GL_CreateContext(window);
}

void ViewState::create_shaders() {
	ShaderProgram& pass_shad = sh.access_program(SH_PASS);
	pass_shad.create_program();
	pass_shad.add_shader(GL_VERTEX_SHADER, "pass.vert");
	pass_shad.add_shader(GL_FRAGMENT_SHADER, "pass.frag");
	pass_shad.add_attribute("in_position");
	pass_shad.add_attribute("tex_coord");
	pass_shad.link_program();

	ShaderProgram& pass_tex_shad = sh.access_program(SH_PASS_TEX);
	pass_tex_shad.create_program();
	pass_tex_shad.add_shader(GL_VERTEX_SHADER, "pass.vert");
	pass_tex_shad.add_shader(GL_FRAGMENT_SHADER, "pass_tex.frag");
	pass_tex_shad.add_attribute("in_position");
	pass_tex_shad.add_attribute("tex_coord");
	pass_tex_shad.link_program();

	ShaderProgram& gray_shad = sh.access_program(SH_GRAY);
	gray_shad.create_program();
	gray_shad.add_shader(GL_VERTEX_SHADER, "pass.vert");
	gray_shad.add_shader(GL_FRAGMENT_SHADER, "gray.frag");
	gray_shad.add_attribute("in_position");
	gray_shad.add_attribute("tex_coord");
	gray_shad.link_program();

	ShaderProgram& occ_shad = sh.access_program(SH_OCCLUDER);
	occ_shad.create_program();
	occ_shad.add_shader(GL_VERTEX_SHADER, "pass.vert");
	occ_shad.add_shader(GL_FRAGMENT_SHADER, "occluder.frag");
	occ_shad.add_attribute("in_position");
	occ_shad.add_attribute("tex_coord");
	occ_shad.link_program();

	ShaderProgram& comp_shad = sh.access_program(SH_SHADOW_COMPRESS);
	comp_shad.create_program();
	comp_shad.add_shader(GL_VERTEX_SHADER, "pass.vert");
	comp_shad.add_shader(GL_FRAGMENT_SHADER, "shadow_compress.frag");
	comp_shad.add_attribute("in_position");
	comp_shad.add_attribute("tex_coord");
	comp_shad.link_program();
}

int ViewState::check_valid() {
	if (window != NULL && renderer != NULL) {
		return 0;
	}
	return 1;
}

void ViewState::render_screen() {
	SDL_GL_SwapWindow(window);
	// SDL_RenderPresent(renderer);
}

void ViewState::load_images() {
	std::cerr << "Beginning to load textures" << std::endl;

	load_texture_image(TH_GRUMP, "images/arin_grump.png");
	load_texture_empty(TH_OCCLUDER, SCREEN_WIDTH, SCREEN_HEIGHT);
	load_texture_empty(TH_SHADOW, SCREEN_WIDTH);
}

void ViewState::unload_images() {
}

int ViewState::screen_width() {
	return SCREEN_WIDTH;
}

int ViewState::screen_height() {
	return SCREEN_HEIGHT;
}

void ViewState::bind_texture(TH_tex_id tex_id) {
	th.access_texture(tex_id).bind();
}

void ViewState::load_texture_image(TH_tex_id tex_id, const char * file_name) {
	th.access_texture(tex_id).load_file(file_name);
}

void ViewState::load_texture_empty(TH_tex_id tex_id, unsigned int width, unsigned int height) {
	th.access_texture(tex_id).load_empty(width, height);
}

void ViewState::load_texture_empty(TH_tex_id tex_id, unsigned int width) {
	th.access_texture(tex_id).load_empty(width);
}

GLuint ViewState::get_texture_name(TH_tex_id tex_id) {
	return th.access_texture(tex_id).get_name();
}

void ViewState::use_program(SH_prog_id shad_id) {
	sh.access_program(shad_id).use();
}

GLuint ViewState::get_uniform_loc(SH_prog_id shad_id, const char * uniform_name) {
	return glGetUniformLocation(sh.access_program(shad_id).get_program(), uniform_name);
}

vector<pair<unsigned int, string>>& ViewState::get_shader_attrs(SH_prog_id shad_id) {
	return sh.access_program(shad_id).shader_attributes;
}