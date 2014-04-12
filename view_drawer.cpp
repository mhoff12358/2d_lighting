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
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	projection_set_screen();

	TEMP_VBO_SHIT();
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
		0.0, 800.0, -1.0,
		1000.0, 800.0, -1.0,
		1000.0, 0.0, -1.0,
		//Texture locations
		0.0, 1.0,
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*grump_vbo_num*(3+2), vals, GL_STATIC_DRAW);

	//Generate the frame buffer, texture, 
	glGenFramebuffers(1, &shadow_frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_frame_buffer);

	glGenTextures(1, &shadow_texture);
	glBindTexture(GL_TEXTURE_2D, shadow_texture);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL, 1024, 1024, 0, GL, GL_FLOAT, 0);


}

void ViewDrawer::draw_screen() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLuint error;
	game.get_state().use_program(SH_PASS);

	glActiveTexture(GL_TEXTURE0);
	game.get_state().bind_texture(TH_GRUMP);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLfloat mvmat[16], promat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mvmat);
	glGetFloatv(GL_PROJECTION_MATRIX, promat);

	glUniformMatrix4fv(game.get_state().get_uniform_loc(SH_PASS, "view_matrix"), 1, false, mvmat);
	glUniformMatrix4fv(game.get_state().get_uniform_loc(SH_PASS, "proj_matrix"), 1, false, promat);
	glUniform1i(game.get_state().get_uniform_loc(SH_PASS, "in_texture"), 0);

	glBindBuffer(GL_ARRAY_BUFFER, grump_vbo);
	glEnableVertexAttribArray(game.get_state().get_shader_attrs(SH_PASS)[0].first);
	glVertexAttribPointer(game.get_state().get_shader_attrs(SH_PASS)[0].first, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(game.get_state().get_shader_attrs(SH_PASS)[1].first);
	glVertexAttribPointer(game.get_state().get_shader_attrs(SH_PASS)[1].first, 2, GL_FLOAT, GL_FALSE, 0, (char*)NULL+sizeof(float)*grump_vbo_num*3);
	glDrawArrays(GL_QUADS, 0, grump_vbo_num);
	error = glGetError();
	if (error != 0) {
		std::cerr << "GL ERROR DRAWING GRUMP: " << error << " " << gluErrorString(error) << std::endl;
	}

	glUseProgram(0);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 100.0, -1.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(100.0, 100.0, -1.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(100.0, 0.0, -1.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.0, -1.0);
	glEnd();
}