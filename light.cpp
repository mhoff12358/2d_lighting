#include "light.h"

Light::Light(float dist, array<float, 2> arc_spec, array<float, 3> light_color) : distance(dist), arc(arc_spec), color(light_color) {
}

void Light::update(float time_mod) {
	arc[1] += time_mod*rotation_scale;
	arc[1] = fmod(arc[1]+2*PI, 2*PI);

	distance_modification += time_mod*distance_scale;
	distance_modification = fmod(distance_modification+2*PI, 2*PI);
}

float Light::get_distance() {
	return distance;
}

float Light::get_active_distance() {
	return distance*(1-distance_modification_max*(sin(distance_modification)+1)/2.0);
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

void Light::set_distance(float d) {
	distance = d;
}

void Light::set_distance_scale(float ds) {
	distance_scale = ds;
}

void Light::set_distance_modification_max(float dmm) {
	distance_modification_max = dmm;
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