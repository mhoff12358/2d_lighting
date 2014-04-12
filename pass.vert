#version 330

uniform mat4 view_matrix;
uniform mat4 proj_matrix;
uniform sampler2D in_texture;

in vec3 in_position;
in vec2 tex_coord;
out vec2 frag_tex_coord;

void main()
{
	gl_Position = proj_matrix*vec4(in_position, 1.0);
	frag_tex_coord = tex_coord;
}