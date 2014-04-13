#version 330

uniform sampler2D in_texture;

in vec2 frag_tex_coord;
out vec4 final_color;

void main()
{
   	final_color = texture2D(in_texture, vec2(frag_tex_coord.x, frag_tex_coord.y));
   	float visibility = 1-final_color.a;
   	final_color = vec4(visibility, visibility, visibility, 1.0);
}