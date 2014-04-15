#version 330

uniform sampler1D in_texture;

in vec2 frag_tex_coord;
out vec4 final_color;

void main()
{
   	final_color = texture1D(in_texture, frag_tex_coord.x);
}