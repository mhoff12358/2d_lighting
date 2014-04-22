#version 330

#define M_PI 3.1415926535897932384626433832795

uniform sampler2D in_texture;
uniform float resolution;

in vec2 frag_tex_coord;
out vec4 final_color;

void main()
{
   	float distance = 1.0;

   	//Fails when the occluder straddles in and outside the radius.
   	//The clipping phase leaves in things with a distance larger than 1 (outside the circle, inside a square)
   	//for (float y = 0.0; y < resolution; y+= 1.0) {
   		//float cd = y/resolution;
   		//float is = texture2D(in_texture, vec2(frag_tex_coord.x, cd)).r;

   		//distance = min(distance, cd+is);
   	//}

   	//for (float y = 0.0; y < resolution; y+= 1.0) {
   		//float cd = y/resolution;
   		//if (texture2D(in_texture, vec2(frag_tex_coord.x, cd)).r < 0.5) {
   			//distance = cd;
   			//y = resolution+1.0;
   		//}
   	//}

      float theta = (frag_tex_coord.x*2-1)*M_PI;
      for (float y = 0.0; y < resolution; y+= 1.0) {
         float cd = y/resolution;
         if (texture2D(in_texture, vec2((cd*cos(theta)+1)/2, (cd*sin(theta)+1)/2)).r < 0.5) {
            distance = cd;
            y = resolution+1.0;
         }

      }

   	final_color = vec4(distance, distance, distance, 1.0);
}