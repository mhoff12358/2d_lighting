
#ifndef __ACTOR_H_INCLUDED__
#define __ACTOR_H_INCLUDED__

#include <GL/glew.h>

#include <array>
#include <vector>

#include "texture_handler.h"
#include "shader.h"

using std::array;
using std::vector;

class Actor {
private:
	array<float, 2> world_coord;
	array<float, 2> world_size;
	TH_tex_id tex_id;
	SH_prog_id shad_id;

public:
	Actor(array<float, 2> wc, array<float, 2> is, TH_tex_id ti, SH_prog_id si) : world_coord(wc), world_size(is), tex_id(ti), shad_id(si) {};

	array<float, 2>& get_world_coord();
	array<float, 2>& get_world_size();
	TH_tex_id get_texture_id();
	SH_prog_id get_shader_id();

};

#endif