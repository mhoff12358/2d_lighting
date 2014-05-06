#include "actor.h"

array<float, 2>& Actor::get_world_coord() {
	return world_coord;
}

array<float, 2>& Actor::get_world_size() {
	return world_size;
}

TH_tex_id Actor::get_texture_id() {
	return tex_id;
}

SH_prog_id Actor::get_shader_id() {
	return shad_id;
}