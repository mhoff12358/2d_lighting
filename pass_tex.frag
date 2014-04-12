#version 330

uniform sampler2D in_texture;

in vec2 frag_tex_coord;
layout(location = 0) out vec4 final_color;

void main()
{
   	final_color = texture2D(in_texture, frag_tex_coord);
}