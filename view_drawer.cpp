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

	projection_set_screen(game.get_state().screen_width(), game.get_state().screen_height());
	glViewport(0, 0, game.get_state().screen_width(), game.get_state().screen_height());

	light_bundles.emplace_back(array<float, 2>({{0.0, 0.0}}));
	light_bundles[0].add_light(400.0, array<float, 2>({{M_PI, M_PI/2.0}}), array<float, 3>({{0.0, 0.0, 1.0}}));
	light_bundles[0].get_lights()[0].set_rotation_scale(1.0);
	light_bundles[0].add_light(200.0, array<float, 2>({{M_PI, 0.0}}), array<float, 3>({{0.0, 1.0, 0.0}}));
	light_bundles[0].get_lights()[1].set_distance_scale(1.0);
	light_bundles[0].get_lights()[1].set_distance_modification_max(0.5);

	// SDL_GL_SetSwapInterval(1);
}

void ViewDrawer::deinitialize() {
}

void ViewDrawer::projection_set_screen(float width, float height) {
	//Set the camera to an orthogonal projection so that it is the whole screen 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width/2.0, width/2.0, -height/2.0, height/2.0, .1, 10.0);
	glMatrixMode(GL_MODELVIEW);
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

	//Could increase the number of points in the triangle fan, which would
	//decrease the number of erraneous pixels sent to the fragment shader but
	//increase the number of vertex points sent. Probably should be a pretty
	//number of vertices, I unno. Also, the x value is the theta, y value the
	//radius. z does nothing. Should probably find a way to just send the 2
	//points.
	float arcvals[10*3] = {
		0.0, 0.0, -1.0,
		M_PI*0.0/4.0, 1.0, -1.0,
		M_PI*1.0/4.0, 2*pow(2,0.5), -1.0,
		M_PI*2.0/4.0, 1.0, -1.0,
		M_PI*3.0/4.0, 2*pow(2,0.5), -1.0,
		M_PI*4.0/4.0, 1.0, -1.0,
		M_PI*5.0/4.0, 2*pow(2,0.5), -1.0,
		M_PI*6.0/4.0, 1.0, -1.0,
		M_PI*7.0/4.0, 2*pow(2,0.5), -1.0,
		M_PI*8.0/4.0, 1.0, -1.0,
	};
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*10*3, arcvals, GL_STATIC_DRAW);

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

void ViewDrawer::render_light(SH_prog_id shader, TH_tex_id shadow_map, float max_distance, vector<Light>& lights) {
	game.get_state().use_program(shader);

	glActiveTexture(GL_TEXTURE1);
	game.get_state().bind_texture(shadow_map);

	GLfloat mvmat[16], promat[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mvmat);
	glGetFloatv(GL_PROJECTION_MATRIX, promat);

	glUniformMatrix4fv(game.get_state().get_uniform_loc(shader, "view_matrix"), 1, false, mvmat);
	glUniformMatrix4fv(game.get_state().get_uniform_loc(shader, "proj_matrix"), 1, false, promat);
	glUniform1i(game.get_state().get_uniform_loc(shader, "shadow_texture"), 1);

	glBindBuffer(GL_ARRAY_BUFFER, arc_vbo);
	glEnableVertexAttribArray(game.get_state().get_shader_attrs(shader)[0].first);
	glVertexAttribPointer(game.get_state().get_shader_attrs(shader)[0].first, 3, GL_FLOAT, GL_FALSE, 0, 0);

	for (auto light_cone = lights.begin(); light_cone != lights.end(); ++light_cone) {
		array<float, 3>& light_color = light_cone->get_color();
		array<float, 2>& light_arc = light_cone->get_arc();
		glUniform3f(game.get_state().get_uniform_loc(shader, "light_color"), light_color[0], light_color[1], light_color[2]);
		glUniform1f(game.get_state().get_uniform_loc(shader, "total_theta"), light_arc[0]);
		glUniform1f(game.get_state().get_uniform_loc(shader, "offset_theta"), light_arc[1]);
		glUniform1f(game.get_state().get_uniform_loc(shader, "distance_scale"), max_distance/light_cone->get_active_distance());
		glDrawArrays(GL_TRIANGLE_FAN, 0, 10);
	}
}

void ViewDrawer::render_actor(Actor& actor) {
	glPushMatrix();
	glTranslatef(actor.get_world_coord()[0], actor.get_world_coord()[1], 0.0);
	glScalef(actor.get_world_size()[0], actor.get_world_size()[1], 1.0);
	render_texture(actor.get_shader_id(), actor.get_texture_id(), square1_vbo, 4);
	glPopMatrix();
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

void ViewDrawer::draw_occluders(array<float, 2>& center_loc, float render_size) {
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

void ViewDrawer::compress_shadows(float light_size) {
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
	projection_set_screen(game.get_state().screen_width(), game.get_state().screen_height());
	glViewport(0, 0, game.get_state().screen_width(), game.get_state().screen_height());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ViewDrawer::draw_light(LightBundle& light_bundle) {
	array<float, 2>& light_center = light_bundle.get_center();
	draw_occluders(light_center, light_bundle.get_max_distance()*2);
	compress_shadows(light_bundle.get_max_distance()*2);
	
	setup_screen_render();
	
	//Testing stuff
	glPushMatrix();
	glScalef(game.get_state().screen_height(), game.get_state().screen_height(), 1);
	// render_texture(SH_TEX_1_PASS, TH_SHADOW, square1_vbo, 4);
	// render_texture(SH_PASS, TH_OCCLUDER, square1_vbo, 4);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(light_center[0], light_center[1], 0);
	glScalef(light_bundle.get_max_distance(), light_bundle.get_max_distance(), 1);
	render_light(SH_SHADOW_LIGHT, TH_SHADOW, light_bundle.get_max_distance(), light_bundle.get_lights());
	glPopMatrix();

	GLuint error;
	error = glGetError();
	if (error != 0) {
		std::cerr << "GL ERROR DRAWING SHADOW TEXTURE: " << error << " " << gluErrorString(error) << std::endl;
	}
}

void ViewDrawer::draw_background() {
	GLuint error;

	setup_screen_render();

	glPushMatrix();
	// glTranslatef(-300, -400, 0);
	glScalef(600, 600, 1);
	render_texture(SH_PASS, TH_GROUND, square1_vbo, 4);
	glPopMatrix();

	error = glGetError();
	if (error != 0) {
		std::cerr << "GL ERROR DRAWING BACKGROUND TEXTURES: " << error << " " << gluErrorString(error) << std::endl;
	}
}

void ViewDrawer::draw_lights() {
	for (auto lb = light_bundles.begin(); lb != light_bundles.end(); ++lb) {
		draw_light(*lb);
	}
}

void ViewDrawer::draw_visuals() {
	GLuint error;
	
	setup_screen_render();
	render_grumps(SH_PASS, square1_vbo, 4);
	for (auto actor_to_render = game.get_game_state().get_actors().begin(); actor_to_render != game.get_game_state().get_actors().end(); ++actor_to_render) {
		render_actor(*actor_to_render);
	}

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

vector<LightBundle>& ViewDrawer::get_light_bundles() {
	return light_bundles;
}