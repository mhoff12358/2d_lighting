#include <iostream>

#include "view_drawer.h"
#include "main.h"

void out_44ma2(float * mat, const string & desc) {
	if (desc.size() != 0) {
		std::cout << desc.data() << std::endl;
	}
	std::cout << mat[0] << "\t" << mat[4] << "\t" << mat[8] << "\t" << mat[12] << "\t" << std::endl; 
	std::cout << mat[1] << "\t" << mat[5] << "\t" << mat[9] << "\t" << mat[13] << "\t" << std::endl; 
	std::cout << mat[2] << "\t" << mat[6] << "\t" << mat[10] << "\t" << mat[14] << "\t" << std::endl; 
	std::cout << mat[3] << "\t" << mat[7] << "\t" << mat[11] << "\t" << mat[15] << "\t" << std::endl; 
}

void ViewDrawer::initialize() {
	glewInit();

	const int SCREEN_WIDTH = game.get_state().screen_width(); 
	const int SCREEN_HEIGHT = game.get_state().screen_height(); 

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	glShadeModel(GL_SMOOTH);

	//Enables depth stuff
	glClearDepth(1.0f);
	// glEnable(GL_DEPTH_TEST);
	// glDepthFunc(GL_LEQUAL);

	//Enables textures and alpha
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_1D);
	// glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	projection_set_screen(1000, 800);
	glViewport(0, 0, 1000, 800);

	// SDL_GL_SetSwapInterval(1);
}

void ViewDrawer::deinitialize() {
}

void ViewDrawer::projection_set_screen(int width, int height) {
	//Set the camera to an orthogonal projection so that it is the whole screen 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width/2.0, width/2.0, -height/2.0, height/2.0, .1, 10.0);
}

void ViewDrawer::TEMP_VBO_SHIT() {
	glGenBuffers(1, &square1_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, square1_vbo);

	float sq1vals[4*(3+2)] = {
		//Vertex locations
		-0.5,-0.5, -1.0,
		 0.5,-0.5, -1.0,
		 0.5, 0.5, -1.0,
		-0.5, 0.5, -1.0,
		//Texture locations
		0.0, 1.0,
		1.0, 1.0,
		1.0, 0.0,
		0.0, 0.0,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*(3+2), sq1vals, GL_STATIC_DRAW);

	glGenBuffers(1, &arc_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, arc_vbo);

	float arcvals[4*3] = {
		0.0,0.0, -1.0,
		1.0,0.0, -1.0,
		1.0,1.0, -1.0,
		0.0,1.0, -1.0,
	};
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*3, arcvals, GL_STATIC_DRAW);

	// game.get_state().bind_texture(TH_SHADOW);
	glGenFramebuffers(1, &shadow_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_frame_buffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, game.get_state().get_texture_name(TH_SHADOW), 0);
	glDrawBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "INCOMPLETE SHADOW FRAME BUFFER STATUS DURING BUILD" << std::endl;
	}

	//Generate the frame buffer, texture, 
	glGenFramebuffers(1, &occluder_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, occluder_frame_buffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, game.get_state().get_texture_name(TH_OCCLUDER), 0);
	glDrawBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "INCOMPLETE OCCLUSION FRAME BUFFER STATUS DURING BUILD" << std::endl;
	}
}

void ViewDrawer::render_texture(SH_prog_id shader, TH_tex_id texture, GLuint vbo, unsigned int vbo_num) {
	game.get_state().use_program(shader);

	glActiveTexture(GL_TEXTURE1);
	game.get_state().bind_texture(texture);

	GLfloat mvmat[16], promat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mvmat);
	glGetFloatv(GL_PROJECTION_MATRIX, promat);

	glUniformMatrix4fv(game.get_state().get_uniform_loc(shader, "view_matrix"), 1, false, mvmat);
	glUniformMatrix4fv(game.get_state().get_uniform_loc(shader, "proj_matrix"), 1, false, promat);
	glUniform1i(game.get_state().get_uniform_loc(shader, "in_texture"), 1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(game.get_state().get_shader_attrs(shader)[0].first);
	glVertexAttribPointer(game.get_state().get_shader_attrs(shader)[0].first, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(game.get_state().get_shader_attrs(shader)[1].first);
	glVertexAttribPointer(game.get_state().get_shader_attrs(shader)[1].first, 2, GL_FLOAT, GL_FALSE, 0, (char*)NULL+sizeof(float)*vbo_num*3);
	glDrawArrays(GL_QUADS, 0, vbo_num);
}

void ViewDrawer::render_light(SH_prog_id shader, TH_tex_id shadow_map, array<float, 2> theta_range) {
	game.get_state().use_program(shader);

	glActiveTexture(GL_TEXTURE1);
	game.get_state().bind_texture(shadow_map);

	GLfloat mvmat[16], promat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mvmat);
	glGetFloatv(GL_PROJECTION_MATRIX, promat);

	glUniformMatrix4fv(game.get_state().get_uniform_loc(shader, "view_matrix"), 1, false, mvmat);
	glUniformMatrix4fv(game.get_state().get_uniform_loc(shader, "proj_matrix"), 1, false, promat);
	glUniform3f(game.get_state().get_uniform_loc(shader, "light_color"), light_color[0], light_color[1], light_color[2]);
	glUniform1i(game.get_state().get_uniform_loc(shader, "shadow_texture"), 1);

	glBindBuffer(GL_ARRAY_BUFFER, arc_vbo);
	glEnableVertexAttribArray(game.get_state().get_shader_attrs(shader)[0].first);
	glVertexAttribPointer(game.get_state().get_shader_attrs(shader)[0].first, 3, GL_FLOAT, GL_FALSE, 0, 0);

	float offset_t = theta_range[0];
	float total_t = theta_range[1]-offset_t;
	while (total_t > 0) {
		//Draw an arc with range of total_theta offset by offset_theta
		glUniform1f(game.get_state().get_uniform_loc(shader, "total_theta"), fmin(total_t, M_PI/2.0));
		glUniform1f(game.get_state().get_uniform_loc(shader, "offset_theta"), offset_t);
		glDrawArrays(GL_QUADS, 0, 4);
		offset_t += M_PI/2;
		total_t -= M_PI/2;
	}
}

void ViewDrawer::render_grumps(SH_prog_id shader_id, GLuint vbo, unsigned int vbo_num) {
	// glPushMatrix();
	// glTranslatef(400, 200, 0);
	// glScalef(100, 100, 1);
	// render_texture(shader_id, TH_GRUMP);
	// glPopMatrix();

	glPushMatrix();
	glTranslatef(-300, -400, 0);
	glScalef(200, 200, 1);
	render_texture(shader_id, TH_GRUMP, vbo, vbo_num);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(250, 0, 0);
	glScalef(100, 300, 1);
	render_texture(shader_id, TH_GRUMP, vbo, vbo_num);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 150, 0);
	glScalef(50, 50, 1);
	render_texture(shader_id, TH_GRUMP, vbo, vbo_num);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 300, 0);
	glScalef(150, 150, 1);
	render_texture(shader_id, TH_GRUMP, vbo, vbo_num);
	glPopMatrix();
}

void ViewDrawer::draw_occluders(array<float, 2> center_loc, unsigned int render_size) {
	GLuint error;
	GLenum gca0 = GL_COLOR_ATTACHMENT0;

	//Render all the occluder objects to the occlusion texture with the objects
	//shifted so they center around center_loc and only rendering those in a
	//render_size wide square
	projection_set_screen(render_size, render_size);
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, occluder_frame_buffer);
	glDrawBuffers(1, &gca0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(-center_loc[0], -center_loc[1], 0);
	render_grumps(SH_OCCLUDER, square1_vbo, 4);
	glPopMatrix();

	error = glGetError();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "INCOMPLETE FRAME BUFFER STATUS DURING BUILD" << std::endl;
	}
	if (error != 0) {
		std::cerr << "GL ERROR DRAWING FRAMEBUFFER: " << error << " " << gluErrorString(error) << std::endl;
	}
}

void ViewDrawer::compress_shadows() {
	GLuint error;
	GLenum gca0 = GL_COLOR_ATTACHMENT0;

	//Render the occlusion texture to the shadow texture by shrinking it down
	//vertically
	projection_set_screen(1, 1);
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_frame_buffer);
	glDrawBuffers(1, &gca0);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glProgramUniform1f(game.get_state().get_program(SH_SHADOW_COMPRESS), game.get_state().get_uniform_loc(SH_SHADOW_COMPRESS, "resolution"), light_size);
	render_texture(SH_SHADOW_COMPRESS, TH_OCCLUDER, square1_vbo, 4);
	// render_texture(SH_PASS, TH_GRUMP, square1_vbo, 4);
	glPopMatrix();
	error = glGetError();
	if (error != 0) {
		std::cerr << "GL ERROR DRAWING SHAODWMAP TEXTURES: " << error << " " << gluErrorString(error) << std::endl;
	}
}

void ViewDrawer::setup_screen_render() {
	//Set up actual render to screen
	projection_set_screen(1000, 800);
	glViewport(0, 0, 1000, 800);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ViewDrawer::draw_light(array<float, 2> center_loc, unsigned int render_size) {
	draw_occluders(center_loc, render_size);
	compress_shadows();
	
	setup_screen_render();
	
	//Testing stuff
	glPushMatrix();
	glScalef(800, 800, 1);
	// render_texture(SH_TEX_1_PASS, TH_SHADOW, square1_vbo, 4);
	// render_texture(SH_PASS, TH_OCCLUDER, square1_vbo, 4);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(center_loc[0], center_loc[1], 0);
	glScalef(render_size/2.0, render_size/2.0, 1);
	render_light(SH_SHADOW_LIGHT, TH_SHADOW, array<float, 2>({{M_PI/4.0, M_PI}}));
	glPopMatrix();

	GLuint error;
	error = glGetError();
	if (error != 0) {
		std::cerr << "GL ERROR DRAWING SHADOW TEXTURE: " << error << " " << gluErrorString(error) << std::endl;
	}
}

void ViewDrawer::draw_background() {

}

void ViewDrawer::draw_lights() {
	// draw_light(array<float, 2>({{light_x-200, light_y}}), light_size);
	// draw_light(array<float, 2>({{light_x+200, light_y}}), light_size);
	// draw_light(array<float, 2>({{425+light_x, light_y}}), light_size);
	draw_light(array<float, 2>({{light_x, light_y}}), light_size);
}

void ViewDrawer::draw_visuals() {
	GLuint error;
	
	setup_screen_render();
	render_grumps(SH_PASS, square1_vbo, 4);

	error = glGetError();
	if (error != 0) {
		std::cerr << "GL ERROR DRAWING FINAL TEXTURES: " << error << " " << gluErrorString(error) << std::endl;
	}
}

void ViewDrawer::draw_screen() {
	setup_screen_render();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_background();
	draw_lights();
	draw_visuals();
}