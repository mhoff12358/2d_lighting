#include "light.h"

Light::Light(float dist, array<float, 2> arc_spec, array<float, 3> light_color) : distance(dist), arc(arc_spec), color(light_color) {
}

void Light::update(float time_mod) {
	arc[1] += time_mod*rotation_scale;
	arc[1] = fmod(arc[1]+2*M_PI, 2*M_PI);
}

float Light::get_distance() {
	return distance;
}

array<float, 2>& Light::get_arc() {
	return arc;
}

array<float, 3>& Light::get_color() {
	return color;
}

void Light::set_rotation_scale(float rs) {
	rotation_scale = rs;
}

LightBundle::LightBundle() {
	max_distance = 0.0;
}

LightBundle::LightBundle(array<float, 2> center_point) : center(center_point) {
	max_distance = 0.0;
}

void LightBundle::add_light(float distance, array<float, 2> arc, array<float, 3> color) {
	lights.emplace_back(distance, arc, color);
	max_distance = fmax(distance, max_distance);
}

void LightBundle::set_center(array<float, 2> new_center) {
	center = new_center;
}

void LightBundle::update(float time_mod) {
	for (auto light = lights.begin(); light != lights.end(); ++light) {
		light->update(time_mod);
	}
}

float LightBundle::get_max_distance() {
	return max_distance;
}

array<float, 2>& LightBundle::get_center() {
	return center;
}

vector<Light>& LightBundle::get_lights() {
	return lights;
}