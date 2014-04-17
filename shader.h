
#ifndef __SHADER_H_INCLUDED__
#define __SHADER_H_INCLUDED__

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <array>

#include <GL/glew.h>

using std::string;
using std::ifstream;
using std::vector;
using std::pair;
using std::array;

enum SH_prog_id {
	SH_PASS,
	SH_PASS_TEX,
	SH_TEX_1_PASS,
	SH_UNROLL_PASS,
	SH_GRAY,
	SH_OCCLUDER,
	SH_SHADOW_COMPRESS,
	SH_SHADOW_LIGHT,
	SH_NUM_SHAD
};

class ShaderProgram {
private:
	// GLuint vert_shad, frag_shad, tess_shad, geo_shad;	
	int load_shader_file(const string&, string&);

	GLuint program;
	vector<pair<GLuint, GLenum>> shaders;

public:
	int add_shader(const GLenum, const string&);

	void create_program();
	void link_program();
	GLint get_program();

	void add_attribute(string);

	void use();

	vector<pair<unsigned int, string>> shader_attributes;

};

class ShaderHandler {
private:
	array<ShaderProgram, SH_NUM_SHAD> shaders;

public:

	ShaderHandler();
	~ShaderHandler();

	ShaderProgram& access_program(SH_prog_id);
};

#endif