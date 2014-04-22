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
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	projection_set_screen(1000, 800);

	// SDL_GL_SetSwapInterval(1);
}

void ViewDrawer::deinitialize() {
}

void ViewDrawer::projection_set_screen(int width, int height) {
	//Set the camera to an orthogonal projection so that it is the whole screen 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width/2, width/2, -height/2, height/2, .1, 10);
	glViewport(0, 0, width, height);
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

	glGenBuffers(1, &square4_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, square4_vbo);

	float sq4vals[4*4*(3+2)] = {
		//Vertex locations
		//Bottom left, bottom right, top left, top right
		-0.5,-0.5, -1.0,
		 0.0,-0.5, -1.0,
		 0.0, 0.0, -1.0,
		-0.5, 0.0, -1.0,
		 0.0,-0.5, -1.0,
		 0.5,-0.5, -1.0,
		 0.5, 0.0, -1.0,
		 0.0, 0.0, -1.0,
		-0.5, 0.0, -1.0,
		 0.0, 0.0, -1.0,
		 0.0, 0.5, -1.0,
		-0.5, 0.5, -1.0,
		 0.0, 0.0, -1.0,
		 0.5, 0.0, -1.0,
		 0.5, 0.5, -1.0,
		 0.0, 0.5, -1.0,
		 //Texture locations
		0.0, 1.0,
		0.5, 1.0,
		0.5, 0.5,
		0.0, 0.5,
		0.5, 1.0,
		1.0, 1.0,
		1.0, 0.5,
		0.5, 0.5,
		0.0, 0.5,
		0.5, 0.5,
		0.5, 0.0,
		0.0, 0.0,
		0.5, 0.5,
		1.0, 0.5,
		1.0, 0.0,
		0.5, 0.0,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*4*(3+2), sq4vals, GL_STATIC_DRAW);
	
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

void ViewDrawer::draw_texture(SH_prog_id shader, TH_tex_id texture, GLuint vbo, unsigned int vbo_num) {
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

void ViewDrawer::draw_light(SH_prog_id shader, TH_tex_id shadow_map) {
	game.get_state().use_program(shader);

	glActiveTexture(GL_TEXTURE1);
	game.get_state().bind_texture(shadow_map);

	GLfloat mvmat[16], promat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mvmat);
	glGetFloatv(GL_PROJECTION_MATRIX, promat);

	glUniformMatrix4fv(game.get_state().get_uniform_loc(shader, "view_matrix"), 1, false, mvmat);
	glUniformMatrix4fv(game.get_state().get_uniform_loc(shader, "proj_matrix"), 1, false, promat);
	glUniform3f(game.get_state().get_uniform_loc(shader, "light_color"), 1.0, 1.0, 1.0);
	glUniform1i(game.get_state().get_uniform_loc(shader, "shadow_texture"), 1);

	glBindBuffer(GL_ARRAY_BUFFER, square1_vbo);
	glEnableVertexAttribArray(game.get_state().get_shader_attrs(shader)[0].first);
	glVertexAttribPointer(game.get_state().get_shader_attrs(shader)[0].first, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_QUADS, 0, 4);
}

void ViewDrawer::draw_grumps(SH_prog_id shader_id, GLuint vbo, unsigned int vbo_num) {
	// glPushMatrix();
	// glTranslatef(400, 200, 0);
	// glScalef(100, 100, 1);
	// draw_texture(shader_id, TH_GRUMP);
	// glPopMatrix();

	glPushMatrix();
	glTranslatef(-300, -400, 0);
	glScalef(200, 200, 1);
	draw_texture(shader_id, TH_GRUMP, vbo, vbo_num);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(250, 0, 0);
	glScalef(100, 300, 1);
	draw_texture(shader_id, TH_GRUMP, vbo, vbo_num);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 150, 0);
	glScalef(50, 50, 1);
	draw_texture(shader_id, TH_GRUMP, vbo, vbo_num);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 300, 0);
	glScalef(150, 150, 1);
	draw_texture(shader_id, TH_GRUMP, vbo, vbo_num);
	glPopMatrix();
}

#define light_rad 1024

void ViewDrawer::draw_screen() {
	GLuint error;
	GLenum gca0 = GL_COLOR_ATTACHMENT0;

	//Prerender to an occlusion texture
	projection_set_screen(light_rad, light_rad);
	glBindFramebuffer(GL_FRAMEBUFFER, occluder_frame_buffer);
	glDrawBuffers(1, &gca0);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	// glScalef(1, -1, 1);
	// draw_grumps(SH_OCCLUDER, square1_vbo, 4);
	draw_grumps(SH_OCCLUDER, square4_vbo, 16);
	glPopMatrix();

	error = glGetError();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "INCOMPLETE FRAME BUFFER STATUS DURING BUILD" << std::endl;
	}
	if (error != 0) {
		std::cerr << "GL ERROR DRAWING FRAMEBUFFER: " << error << " " << gluErrorString(error) << std::endl;
	}

	//Render the occlusion texture to the shadow texture
	// projection_set_screen(light_rad, 1);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_frame_buffer);
	glDrawBuffers(1, &gca0);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glScalef(light_rad, light_rad, 1);
	glProgramUniform1f(game.get_state().get_program(SH_SHADOW_COMPRESS), game.get_state().get_uniform_loc(SH_SHADOW_COMPRESS, "resolution"), light_rad);
	draw_texture(SH_SHADOW_COMPRESS, TH_OCCLUDER, square1_vbo, 4);
	// draw_texture(SH_PASS, TH_GRUMP, square1_vbo, 4);
	glPopMatrix();
	error = glGetError();
	if (error != 0) {
		std::cerr << "GL ERROR DRAWING SHAODWMAP TEXTURES: " << error << " " << gluErrorString(error) << std::endl;
	}

	//Actual render to screen
	projection_set_screen(1000, 800);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	glScalef(800, 800, 1);
	// draw_texture(SH_TEX_1_PASS, TH_SHADOW, square1_vbo, 4);
	// draw_texture(SH_PASS, TH_OCCLUDER, square1_vbo, 4);
	glPopMatrix();
	glPushMatrix();
	glScalef(light_rad, light_rad, 1);
	draw_light(SH_SHADOW_LIGHT, TH_SHADOW);
	glPopMatrix();
	draw_grumps(SH_PASS, square1_vbo, 4);

	error = glGetError();
	if (error != 0) {
		std::cerr << "GL ERROR DRAWING FINAL TEXTURES: " << error << " " << gluErrorString(error) << std::endl;
	}
}