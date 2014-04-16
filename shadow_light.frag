#version 330

uniform sampler1D shadow_texture;
uniform vec3 light_color;

in vec2 frag_coord;
out vec4 final_color;

void main()
{
	final_color = vec4(texture1D(shadow_texture, frag_coord.x).x, 1.0, 0.0, 1.0);

	if (1-frag_coord.y > texture1D(shadow_texture, frag_coord.x)) {
		final_color = vec4(0.0, 0.0, 0.0, 0.0);
	} else {
		final_color = vec4(1.0, 1.0, 1.0, 1.0);
	}
}