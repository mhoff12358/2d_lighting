#version 330

#define M_PI 3.1415926535897932384626433832795

uniform mat4 view_matrix;
uniform mat4 proj_matrix;
uniform sampler2D in_texture;

in vec3 in_position;
in vec2 tex_coord;
out vec2 frag_tex_coord;
out float dist;
out float theta;

void main()
{
	vec3 proj_point = proj_matrix*view_matrix*vec4(in_position, 1.0);
	dist = pow((proj_point.x*proj_point.x+proj_point.y*proj_point.y), 0.5);
	theta = atan(proj_point.y, proj_point.x)/M_PI;
	gl_Position = vec4(theta, dist*2-1, proj_point.z, 1.0);
	frag_tex_coord = tex_coord;
}