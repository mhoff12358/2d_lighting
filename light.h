
#ifndef __LIGHT_H_INCLUDED__
#define __LIGHT_H_INCLUDED__

#include <vector>
#include <array>

#define M_PI 3.14159265358979323846
#include <cmath>

using std::vector;
using std::array;
using std::fmax;

class Light {
private:
	float distance; //The max distance that the light reaches
	//Needs some sort of way of defining which light dropoff function to use
	array<float, 2> arc; //total arc range, arc offset
	array<float, 3> color;

	float rotation_scale; //radians per second of rotation

public:
	Light(float, array<float, 2>, array<float, 3>);

	void update(float);

	float get_distance();
	array<float, 2>& get_arc();
	array<float, 3>& get_color();

	void set_rotation_scale(float);
};

class LightBundle {
private:
	vector<Light> lights;
	float max_distance;
	array<float, 2> center;

public:
	LightBundle();
	LightBundle(array<float, 2>);

	void add_light(float, array<float, 2>, array<float, 3>);
	void set_center(array<float, 2>);

	void update(float);
	
	array<float, 2>& get_center();
	float get_max_distance();
	
	vector<Light>& get_lights();
};

#endif