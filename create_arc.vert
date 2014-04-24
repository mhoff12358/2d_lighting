#version 330

#define M_PI 3.1415926535897932384626433832795

uniform mat4 view_matrix;
uniform mat4 proj_matrix;
uniform float total_theta;
uniform float offset_theta;

in vec3 in_position;
out vec2 frag_coord;

void main()
{
	float theta = in_position.x;
	float dist = in_position.y;
	theta = theta*total_theta/(2*M_PI)+offset_theta;
	in_position = vec3(dist*cos(theta), dist*sin(theta), in_position.z);
	gl_Position = proj_matrix*vec4(in_position, 1.0);
	frag_coord = vec2(in_position.x, in_position.y);
}