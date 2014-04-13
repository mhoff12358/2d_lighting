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

	projection_set_screen();

	// SDL_GL_SetSwapInterval(1);
}

void ViewDrawer::deinitialize() {
}

void ViewDrawer::projection_set_screen() {
	//Set the camera to an orthogonal projection so that it is the whole screen 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, game.get_state().screen_width(), 0, game.get_state().screen_height(), .1, 10);
}

void ViewDrawer::TEMP_VBO_SHIT() {
	glGenBuffers(1, &grump_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, grump_vbo);

	float vals[grump_vbo_num*(3+2)] = {
		//Vertex locations
		0.0, 0.0, -1.0,
		1.0, 0.0, -1.0,
		1.0, 1.0, -1.0,
		0.0, 1.0, -1.0,
		//Texture locations
		0.0, 1.0,
		1.0, 1.0,
		1.0, 0.0,
		0.0, 0.0,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*grump_vbo_num*(3+2), vals, GL_STATIC_DRAW);

	//Generate the frame buffer, texture, 
	glGenFramebuffers(1, &occluder_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, occluder_frame_buffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, game.get_state().get_texture_name(TH_OCCLUDER), 0);
	glDrawBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "INCOMPLETE OCCLUSION FRAME BUFFER STATUS DURING BUILD" << std::endl;
	}

	game.get_state().bind_texture(TH_SHADOW);
	glGenFramebuffers(1, &shadow_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_frame_buffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, game.get_state().get_texture_name(TH_SHADOW), 0);
	glDrawBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "INCOMPLETE SHADOW FRAME BUFFER STATUS DURING BUILD" << std::endl;
	}
}

void ViewDrawer::draw_texture(SH_prog_id shader, TH_tex_id texture) {
	game.get_state().use_program(shader);

	glActiveTexture(GL_TEXTURE1);
	game.get_state().bind_texture(texture);

	GLfloat mvmat[16], promat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mvmat);
	glGetFloatv(GL_PROJECTION_MATRIX, promat);

	glUniformMatrix4fv(game.get_state().get_uniform_loc(shader, "view_matrix"), 1, false, mvmat);
	glUniformMatrix4fv(game.get_state().get_uniform_loc(shader, "proj_matrix"), 1, false, promat);
	glUniform1i(game.get_state().get_uniform_loc(shader, "in_texture"), 1);

	glBindBuffer(GL_ARRAY_BUFFER, grump_vbo);
	glEnableVertexAttribArray(game.get_state().get_shader_attrs(shader)[0].first);
	glVertexAttribPointer(game.get_state().get_shader_attrs(shader)[0].first, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(game.get_state().get_shader_attrs(shader)[1].first);
	glVertexAttribPointer(game.get_state().get_shader_attrs(shader)[1].first, 2, GL_FLOAT, GL_FALSE, 0, (char*)NULL+sizeof(float)*4*3);
	glDrawArrays(GL_QUADS, 0, 4);
}

void ViewDrawer::draw_grumps(SH_prog_id shader_id) {
	glPushMatrix();
	glScalef(100, 100, 1);
	draw_texture(shader_id, TH_GRUMP);
	glPopMatrix();
}

void ViewDrawer::draw_screen() {
	GLuint error;
	GLenum gca0 = GL_COLOR_ATTACHMENT0;

	//Prerender to an occlusion texture
	glBindFramebuffer(GL_FRAMEBUFFER, occluder_frame_buffer);
	glDrawBuffers(1, &gca0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	glScalef(1, -1, 1);
	glTranslatef(0, -800, 0);
	draw_grumps(SH_OCCLUDER);
	glPopMatrix();

	error = glGetError();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "INCOMPLETE FRAME BUFFER STATUS DURING BUILD" << std::endl;
	}
	if (error != 0) {
		std::cerr << "GL ERROR DRAWING FRAMEBUFFER: " << error << " " << gluErrorString(error) << std::endl;
	}

	//Render the occlusion texture to the shadow texture
	// glBindFramebuffer(GL_FRAMEBUFFER, shadow_frame_buffer);
	// glDrawBuffers(1, &gca0);
	// glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// glClear(GL_COLOR_BUFFER_BIT);

	// glPushMatrix();
	// glScalef(1000, 1, 1);
	// draw_texture(SH_SHADOW_COMPRESS, TH_OCCLUDER);
	// glPopMatrix();

	//Actual render to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();
	glScalef(1000, 800, 1);
	draw_texture(SH_PASS, TH_OCCLUDER);
	glPopMatrix();

	error = glGetError();
	if (error != 0) {
		std::cerr << "GL ERROR DRAWING FINAL TEXTURES: " << error << " " << gluErrorString(error) << std::endl;
	}
}