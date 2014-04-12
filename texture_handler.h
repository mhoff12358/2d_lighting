
#ifndef __TEXTURE_HANDLER_H_INCLUDED__
#define __TEXTURE_HANDLER_H_INCLUDED__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>

#include <string>
#include <cstdio>
#include <array>
#include <string>

using std::array;
using std::string;

enum TH_tex_id {
	TH_GRUMP,
	TH_NUM_TEX
};

class Texture {
private:
	GLuint name;
	array<unsigned int, 2> size;

public:
	Texture();
	~Texture();

	void load_file(const char* file_name);
	void bind();
	
	GLuint get_name();
	unsigned int get_width();
	unsigned int get_height();
};

class TextureHandler {
private:
	array<Texture, TH_NUM_TEX> tex_holder;

public:
	Texture& access_texture(TH_tex_id);
};

#endif