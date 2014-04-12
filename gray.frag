#version 330

uniform sampler2D in_texture;

in vec2 frag_tex_coord;
out vec4 final_color;

void main()
{
   	final_color = texture2D(in_texture, frag_tex_coord);
   	float val = (final_color.r+final_color.g+final_color.b)/3*final_color.a;
   	final_color = vec4(val, val, val, final_color.a);
}