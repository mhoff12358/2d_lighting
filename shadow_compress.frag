#version 330

uniform sampler2D in_texture;
uniform float resolution;

in vec2 frag_tex_coord;
out vec4 final_color;

void main()
{
   	float distance = 1.0;

   	for (float y = 0.0; y < resolution; y+= 1.0) {
   		float cd = y/resolution;
   		float is = texture2D(in_texture, vec2(frag_tex_coord.x, cd)).r;
   		distance = min(distance, cd+is);
   	}

   	//final_color = vec4(resolution/1600, 0.0, 1.0, 1.0);
   	final_color = vec4(distance, distance, distance, 1.0);
}