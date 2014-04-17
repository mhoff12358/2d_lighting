#version 330

#define M_PI 3.1415926535897932384626433832795

uniform sampler1D shadow_texture;
uniform vec3 light_color;

in vec2 frag_coord;
out vec4 final_color;

void main()
{
	final_color = vec4(texture1D(shadow_texture, frag_coord.x).x, 1.0, 0.0, 1.0);

	vec2 location = vec2(frag_coord.x, frag_coord.y);
	float distance = pow(location.x*location.x+location.y*location.y, 0.5);
	float theta = atan(location.y, location.x)/(2*M_PI)+0.5;

	if (distance > texture1D(shadow_texture, theta)) {
		final_color = vec4(0.0, 0.0, 0.0, 0.0);
	} else {
		final_color = vec4(1.0, 1.0, 1.0, 1.0);
	}

	//final_color = vec4(distance, theta, 0.0, 1.0);
}