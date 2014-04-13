#include "texture_handler.h"
#include <iostream>

Texture::Texture() {
	glGenTextures(1, &name);
}

Texture::~Texture() {
	glDeleteTextures(1, &name);
}

void Texture::bind() {
	if (mode == 1) {
		glBindTexture(GL_TEXTURE_1D, name);
	} else {
		glBindTexture(GL_TEXTURE_2D, name);
	}
}

void Texture::load_file(const char * file_name) {
	SDL_Surface * in_surface = IMG_Load(file_name);
	if (!in_surface) {
		std::cout << "Error loading image file: " << file_name << " into texture " << name << std::endl;
	} else {
		bind();
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, in_surface->w, in_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, in_surface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	SDL_FreeSurface(in_surface);
}

void Texture::load_empty(unsigned int width, unsigned int height) {
	mode = 2;
	bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::load_empty(unsigned int width) {
	mode = 1;
	bind();
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::set_mode(unsigned int m) {
	mode = m;
}

unsigned int Texture::get_width() {
	return size[0];
}

unsigned int Texture::get_height() {
	return size[1];
}

GLuint Texture::get_name() {
	return name;
}

Texture& TextureHandler::access_texture(TH_tex_id tex_id) {
	if (tex_id == TH_NUM_TEX) {
		std::cerr << "Attempted to access an invalid texture" << std::endl;
	}
	return tex_holder[tex_id];
}