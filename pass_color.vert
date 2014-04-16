#version 330

uniform mat4 view_matrix;
uniform mat4 proj_matrix;

in vec3 in_position;
out vec2 frag_coord;

void main()
{
	gl_Position = proj_matrix*vec4(in_position, 1.0);
	frag_coord = vec2(in_position.x, in_position.y);
}